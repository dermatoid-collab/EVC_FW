#include <stdlib.h>
#include "vfd_inl.h"
#include "application.h"
#include "can_inl.h"
#include "trips_inl.h"
#include "run_mode.h"
#include "timer_inl.h"

struct vfd_info vfd;

/* Inlines */
bool vfd_in_fault(void);
bool vfd_in_pseudofault(void);
bool vfd_is_ready(void);
bool vfd_is_running(void);
bool vfd_output_is_open(void);
bool vfd_output_is_closed(void);
void vfd_output_emergency_open(void);
void vfd_output_open(void);
void vfd_output_close(void);
void vfd_output_reset(void);
void vfd_stop(uint8_t time_dec);
void vfd_halt(void);

/* Ricezione frame di risposta da VFD; trasmette su 4 indirizzi.
   Da chiamare nel dispatch di ricezione CAN (che ha già risolto gli indirizzi)

   C'è blanking nel periodo di restart per ignorare eventuali schifezze in coda.
*/

/** TPDO1: tanti valori di varia utilità */
void vfd_process_can_tpdo1(const struct can_frame *frame)
{
    if (!vfd.restarting)
    {
        vfd.motor_status = frame->data[0];
        vfd.motor_voltage = (int8_t)frame->data[1];
        vfd.motor_current = (int8_t)frame->data[2];
        vfd.overload = frame->data[3];
        vfd.input_voltage = frame->data[4];
        vfd.heatsink_temperature = frame->data[5];
        vfd.motor_speed = can_extract_int16(frame->data + 6);
        /* Cessato allarme se il motore è fermo */
        if (abs(vfd.motor_speed) < 10)
        {
            trip_retract(TRIP_STOP_TIMEOUT, true);
        }
    }
}

/** Il TPDO2 essenzialmente contiene il codice di fault */
void vfd_process_can_tpdo2(const struct can_frame *frame)
{
    if (!vfd.restarting) {
        vfd.fault = frame->data[7];
        trip_assign(TRIP_VFD, vfd_in_fault());
    }
}

/** Nel TPDO3 non c'è nulla di interessante, ma lo riceviamo lo stesso.
 * La posizione motore è sostituita dalla lettura dell'encoder. */
void vfd_process_can_tpdo3(const struct can_frame *frame)
{
    if (!vfd.restarting) {
        vfd.motor_position = can_extract_int16(frame->data);
    }
}

/** Il beat garantisce il funzionamento del VFD (lo stato NMT
 * è fisso!)*/
void vfd_process_can_beat(const struct can_frame *frame)
{
    if (!vfd.restarting)
    {
        tmr_reset(TMR_ID_VFD_OFFLINE);
        vfd.online = true;
    } else {
        /* Sta ripartendo */
        --vfd.restarting;
    }
}


/** Invia un comando di run al VFD. Per fermarlo si chiedono zero rpm */
static bool vfd_rpdo1(uint16_t speed, enum vfd_run_req direction,
        uint8_t time_acc, uint8_t time_dec, uint8_t cmds)
{
    /* Prepara il messaggio CAN da inviare */
    struct can_frame txbuf;
    txbuf.id = VFD_CAN_ID_RPDO1;
    txbuf.IDE = CAN_ID_STD;
    txbuf.RTR = false;
    txbuf.DLC = 8;
    if (direction == VFD_RUN_STOP) {
        txbuf.data[0] = 0;
        txbuf.data[1] = 0;
        txbuf.data[2] = 0;
        /* Avvia il timeout di arresto. Importante: viene calcolato dal
         * punto in cui viene richiesto lo stop, non da quando viene
         * inviato, in modo da tener conto di eventuali ritardi di
         * trasmissione */

        /* Tempo specificato in decimi + 200ms per la comunicazione,
           ma non meno di 500ms */
        tmr_sv_set(TMR_ID_STOP_TIMEOUT,
                time_dec + VFD_STOP_TIME_ADD);
        tmr_sv_extend(TMR_ID_STOP_TIMEOUT, VFD_STOP_TIME_MIN);
        tmr_reset(TMR_ID_STOP_TIMEOUT);
    }
    else
    {
        can_store_uint16(txbuf.data, speed);
        txbuf.data[2] = direction;
    }
    txbuf.data[3] = time_acc;
    txbuf.data[4] = time_dec;
    txbuf.data[5] = 0;
    txbuf.data[6] = 0;
    txbuf.data[7] = cmds;
    return can_tx_buffer_push(&txbuf, CAN250);
}

/** Invia i parametri di configurazione al VFD via SDO expedited.
 Ritorna true quando ha finito */
bool vfd_param_setup(void)
{
    /* Per il momento invia solo l'abilitazione del timeout su mancanza
     * di comunicazione, se proprio non è stato configurato */
    struct can_frame txbuf;
    txbuf.id = VFD_CAN_ID_TSDO;
    txbuf.IDE = CAN_ID_STD;
    txbuf.RTR = false;
    txbuf.DLC = 8;
    txbuf.data[0] = 0b00101011;
    const uint16_t od_idx = 0x3000 + 46;
    const uint16_t ov_val = 200;
    txbuf.data[1] = od_idx >> 8;
    txbuf.data[2] = od_idx & 0xff;
    txbuf.data[3] = 0;
    txbuf.data[4] = ov_val >> 8;
    txbuf.data[5] = ov_val & 0xff;
    return can_tx_buffer_push(&txbuf, CAN250);
}

/** La condizione di ready è la più stringente. Il VFD deve:
   - essere raggiungibile
   - con il DC link carico
   - non in fault
   - non in spegnimento
   - con il relè principale in posizione
*/
bool vfd_is_ready(void)
{
    return vfd.online &&
            (vfd.input_voltage > VFD_DCLINK_MIN) &&
            !DIn.otr_fb &&
            !vfd.fault &&
            /* in PMODE_ST_WAIT_CHARGER possiamo ancora chiedere la
               commutazione; in PMODE_ST_TO_CHARGER è possibile che non
               sia stabile la logica ed è meglio far completare il
               ciclo */
            (pmode.state != PMODE_ST_TO_CHARGER);
}


/** Macchina a stati di commutazione del relè di uscita

    Politica iniziale: il contattore di uscita si apre quando il
    VFD è in fault o c'è sovratensione pericolosa. Non fa parte
    del circuito di sicurezza e quindi può rimanere collegato.

    L'unico motivo per cui è presente è proteggersi dalla
    sovratensione sull'uscita. Per salvare l'uscita del VFD si
    mantiene collegato per altri 100 ms dopo la segnalazione di
    errore (a meno di errore sovratensione!)

    Regole:
    1) Il relè impiega circa 100 ms sia ad aprirsi che a chiudersi;

    2) Il relè (normalmente) può essere aperto solo dopo 10 s da un
       fault o da velocità zero sul motore;

    3) In caso di sovratensione (emergenza) può essere aperto
       istantaneamente (ignorando la regola 2) ma bisogna sempre
       considerare il tempo di commutazione (vale la regola 1);

    4) Il timer di assestamento contatti è lo stesso per apertura
       e chiusura quindi va resettato in caso di inversione
       del movimento.

    5) Una volta in emergenza rimane così fino al reset (gestita nelle
       inline)
*/

/** Controllo relè di uscita: è aperto in attesa di richiesta.
    Non ci sono prerequisiti alla chiusura */
static void vfd_output_handle_open(void)
{
    vfd.output_coil = false;
    switch (vfd.output_req)
    {
    case VFD_OUTPUT_EMERGENCY_REQ:
    case VFD_OUTPUT_OPEN_REQ:
        /* Siamo già aperti, nulla da fare */
        break;
    case VFD_OUTPUT_CLOSE_REQ:
        /* Avvia la chiusura */
        vfd.output_state = VFD_OUTPUT_CLOSING;
        break;
    }
}

/** Controllo relè di uscita: si sta chiudendo, tempo meccanico dei
    contatti */
static void vfd_output_handle_closing(void)
{
    vfd.output_coil = true;
    switch (vfd.output_req)
    {
    case VFD_OUTPUT_CLOSE_REQ:
        if (tmr_update(TMR_ID_VFD_OUTPUT_SWITCH, true))
        {
            vfd.output_state = VFD_OUTPUT_CLOSED;
        }
        break;
    case VFD_OUTPUT_OPEN_REQ:
    case VFD_OUTPUT_EMERGENCY_REQ:
        /* Inversione di marcia */
        tmr_reset(TMR_ID_VFD_OUTPUT_SWITCH);
        vfd.output_state = VFD_OUTPUT_OPENING;
        break;
    }
}

/** Controllo relè di uscita: è chiuso in attesa di richiesta e/o in
    attesa di consenso alla riapertura */
static void vfd_output_handle_closed(void)
{
    vfd.output_coil = true;

    /* Predicato: il relè di uscita si può aprire (non in emergenza).
    Vedi regola 2 sopra.

    Importante: il timer di ritardo all'apertura conta sempre, non
    necessariamente quando è richiesta l'apertura. Per esempio se viene
    richiesta l'apertura dopo un secondo dallo stop non è necessario
    attendere. */
    bool open_en = tmr_update(TMR_ID_VFD_OUTPUT_DELAY, !vfd_is_running());
    switch (vfd.output_req)
    {
    case VFD_OUTPUT_OPEN_REQ:
        /* Riapertura normale, aspetta il consenso */
        if (open_en)
        {
            vfd.output_state = VFD_OUTPUT_OPENING;
        }
        break;
    case VFD_OUTPUT_CLOSE_REQ:
        /* Siamo già chiusi, nulla da fare */
        break;
    case VFD_OUTPUT_EMERGENCY_REQ:
        /* Riapertura di emergenza, ignora il consenso */
        vfd.output_state = VFD_OUTPUT_OPENING;
        break;
    }
}

/** Controllo relè di uscita: in apertura, tempo meccanico dei
    contatti */
static void vfd_output_handle_opening(void)
{
    vfd.output_coil = false;
    switch (vfd.output_req)
    {
    case VFD_OUTPUT_EMERGENCY_REQ:
    case VFD_OUTPUT_OPEN_REQ:
        /* Già in manovra */
        if (tmr_update(TMR_ID_VFD_OUTPUT_SWITCH, true))
        {
            vfd.output_state = VFD_OUTPUT_OPEN;
        }
        break;
    case VFD_OUTPUT_CLOSE_REQ:
        /* Inversione di marcia */
        tmr_reset(TMR_ID_VFD_OUTPUT_SWITCH);
        vfd.output_state = VFD_OUTPUT_CLOSING;
        break;
    }
}

/** Dispatch principale macchina a stati relè di uscita */
static void vfd_output_scan_cycle(void)
{
    switch (vfd.output_state)
    {
    case VFD_OUTPUT_OPEN:
        vfd_output_handle_open();
        break;
    case VFD_OUTPUT_CLOSING:
        vfd_output_handle_closing();
        break;
    case VFD_OUTPUT_CLOSED:
        vfd_output_handle_closed();
        break;
    case VFD_OUTPUT_OPENING:
        vfd_output_handle_opening();
        break;
    }
    /* (Dis)attiva il contattore di uscita (è collegato su una GPO) */
    HAL_GPIO_WritePin(OUT3_1_GPIO_Port, OUT3_1_Pin,  vfd.output_coil);
}

/** Recupero da timeout comando: lo ripete e speriamo per il meglio
 */
static void vfd_timed_out(void)
{
    /* Primo tentativo: rimanda il comando */
    if (!is_trip_active(TRIP_STOP_TIMEOUT))
    {
        /* L'errore non è bloccante ma viene memorizzato */
        trip_assert(TRIP_STOP_TIMEOUT, true);
        vfd.request_pending = true;
    }
    else
    {
        /* È duro d'orecchi */
        pmode_request(PMODE_REQ_CHARGER_NOW);
        vfd_reboot();
    }
}

/* Ciclo di scansione, sezione del VFD; controlla che non sia in timeout
   e, se pronto, invia un eventuale frame di configurazione; richiama
   anche la commutazione del relè di uscita sulle fasi. */
void vfd_scan_cycle(void)
{
    /* Tragedia: in sovratensione si stacca alla svelta il sezionatore
     * d'uscita e che il VFD vada a quel paese. Nota che l'apertura di
     * emergenza è latchata, non basta una vfd_close */
    if (battery.overvoltage)
    {
        vfd_output_emergency_open();
        /* In teoria un reboot è il modo migliore per aprire le uscite,
           finché non abbiamo il segnale di enable cablato */
        vfd_reboot();
        trip_assert(TRIP_OVERVOLTAGE, true);
    }
    /* Tempo di restart del VFD (e inhibit comunicazioni)*/
    if (tmr_update(TMR_ID_VFD_OFFLINE, !vfd.restarting))
    {
        vfd.online = false;
    }
    /* Timeout arresto motore */
    if (tmr_update(TMR_ID_STOP_TIMEOUT,
                    (abs(vfd.motor_speed) >= 10)
                    && pmode.output_coil
                    && vfd.current_req.direction == VFD_RUN_STOP))
    {
        vfd_timed_out();
        tmr_reset(TMR_ID_STOP_TIMEOUT);
    }
    /* EARLY EXIT: Se non è online non si può fare niente che
     * aspettare */
    if (!vfd.online)
    {
        return;
    }
    /* Priorita ai comandi */
    if (vfd_is_ready())
    {
        if (vfd.current_req.cmds)
        {
            if (vfd_rpdo1(0, VFD_RUN_STOP, 0, 0,
                            vfd.current_req.cmds))
            {
                /* Li abbiamo spediti tutti */
                vfd.current_req.cmds = 0;
            }
        }
        else if (vfd.request_pending && vfd_output_is_closed())

            /* Richieste di movimento, solo con l'uscita collegata.
               Lo stop necessita di decelerazione e l'uscita di può
               aprire solo se il VFD è fermo o in emergenza. Se è in
               emergenza è bloccata quindi il problema non si pone */
        {
            if (vfd_rpdo1(vfd.current_req.speed,
                            vfd.current_req.direction,
                            vfd.current_req.time_acc,
                            vfd.current_req.time_dec,
                            vfd.current_req.cmds))
            {
                /* A questo punto la richiesta è in coda per la spedizione */
                vfd.request_pending = false;
                vfd.last_req = vfd.current_req;
            }
        }
    }
    /* se sono in errore e ci sono dati da spedire serve un reset.
       Attenzione, ci sono dei falsi fault che devono semplicemente essere resettati */
    else if (vfd_in_pseudofault()
            && (pmode.state == PMODE_ST_DRIVE
                    || pmode.state == PMODE_ST_TO_DRIVE)
            && vfd.online
            && vfd_output_is_closed()
            && (vfd.request_pending
                    || vfd.current_req.cmds))
    {
        vfd_reboot();
    }

    /* Check cambio modalità: se deve stare fermo e non ha altro da
       fare; In collaudo paracadute deve rimanere collegato per poter
       alimentare l'apertura manuale del freno */
    if (((vfd.current_req.speed == 0)
            || (vfd.current_req.direction == VFD_RUN_STOP))
            && (run_mode.state != MACHINE_STATE_FALL_TEST)
            && !vfd.current_req.cmds)
    {
    	/* Comincia il tempo di grazia quindi ci sta la decelerazione */
        pmode_request(PMODE_REQ_CHARGER);
        vfd_output_open();
    }
    else
    {
        /* Transizione di routine, non viene fatta se siamo in emergenza */
        pmode_request(PMODE_REQ_DRIVE);
        vfd_output_close();
    }
    /* Relè di uscita, è una macchina a stati a parte */
    vfd_output_scan_cycle();
}

/** Richiede un cambio stato al VFD; viene memorizzato e inviato alla
 * prima occasione utile, se c'è qualcosa di variato */
void vfd_move_request(uint16_t speed, enum vfd_run_req direction,
        uint8_t time_acc, uint8_t time_dec)
{
    if ((speed != vfd.last_req.speed) ||
            (direction != vfd.last_req.direction) ||
            (vfd.current_req.time_acc != vfd.last_req.time_acc) ||
            (vfd.current_req.time_dec != vfd.last_req.time_dec)) {
        vfd.request_pending = true;
        vfd.current_req.speed = speed;
        vfd.current_req.direction = direction;
        vfd.current_req.time_acc = time_acc;
        vfd.current_req.time_dec = time_dec;
    }
}

/** Come sopra ma per i comandi; tutti i comandi lavorano in regime di
   stop, quindi la vita è più semplice */
void vfd_cmd_request(uint8_t cmd)
{
    /* XXX Cosa fare se c'è più di un comando in pending? si mandano
       tutti assieme o uno alla volta? per il momento il problema non si
       pone, sono tutte funzioni speciali */
    vfd.current_req.cmds |= cmd;
}

/* Nota su reset e reboot: in teoria il reset fa solo pulizia errori
 * mentre il reboot rifà tutto da capo, in pratica sono entrambe
 * lunghette. Viene usato lo stesso giro controllato dal campo restart.
 * In pratica cambia solo il comando iniziale. */

/** Parte comune per reset e reboot: comando a parte la gestione
 * è identica */
static void vfd_restart(uint8_t cmd)
{
    if (!vfd.restarting)
    {
        /* Prevede quello che sarà lo stato a fine riavvio */
        vfd.current_req.speed = 0;
        vfd.current_req.direction = VFD_RUN_STOP;
        vfd.current_req.time_acc = 0;
        vfd.current_req.time_dec = 0;
        vfd.fault = VFD_FAULT_NO;
        trip_retract(TRIP_VFD, true);
        /* Se non uso RPDO1 ma solo la mode request vado in ricorsione infinita in quanto usa questa funzione per togliere il fault */
        vfd_rpdo1(0, VFD_RUN_STOP, 1, 1, cmd);
        vfd.online = false;
        vfd.restarting = VFD_RESET_BEATS;
    }
}

/** Reset del VFD, principalmente stop rapido e fault clear */
void vfd_reset(void)
{
    vfd_restart(VFD_COMMAND_FAULT_RESET);
}

/** Invia una richiesta di reboot al VFD. Bisogna aspettare un po' per
    essere sicuri che sia ripartito */
void vfd_reboot(void)
{
    vfd_restart(VFD_COMMAND_REBOOT);
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
