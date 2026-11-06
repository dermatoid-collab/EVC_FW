#include "application.h"
#include "npb.h"
#include "battery.h"
#include "ltc2959.h"
#include "battery.h"

struct battery_info battery;

/** Step per accesso al power monitor */
enum {
    BATSCAN_CONFIG,
    BATSCAN_VOLTAGE,
    BATSCAN_CURRENT,
    BATSCAN_TEMPERATURE,
    BATSCAN_CHARGE,
} battery_scan_step;

/* Prototipi */
static void npb_chrg_enter_wait_disch(void);

/** Imposta tensione e corrente d'uscita al minimo gestito, almeno per
 * far ripartire la logica; l'idea è di non caricare la batteria */
static void npb_chrg_minimum_power(void)
{
    npb.cmd.set_current = NPB_MIN_CURRENT;
    npb.cmd.set_voltage = NPB_MIN_VOLTAGE;
}

/** Operazioni a batteria completamente carica: reset contatore di
    carica e passa in scarica
*/
static void battery_fully_charged(void)
{
    bat_reset_charge_counter_full();
    /* Azzera il conteggio di cicli in float, dopo il completamento del
       boost la deriva è recuperata */
    battery.float_cycle_counter = 0;
    battery.charge_timer = 0;
    npb_chrg_enter_wait_disch();
}

/* Regolatore a singola rampa; sono troppo pigro/non ho tempo per tarare
 * un PID e per questo scopo è sufficiente. Gira ad ogni secondo. */

/** Condizioni ad avvio ricarica */
static void npb_chrg_current_reset(void)
{
    battery.current_cv = NPB_MAX_CURRENT;
    battery.charge_timer = 0;
    npb.cmd.set_current = battery.current_cv;
}

/** Regolatore bang-bang isteretico, funziona abbastanza bene per l'applicazione */
static void npb_chrg_current_step(void)
{
    /* Lo 0.1 è arbitrario ed è il deadband del regolatore */
    if ((battery.current > BAT_TARGET_CURRENT + 0.1)
            && (battery.current_cv >= NPB_CV_CURRENT_STEP)) {
        battery.current_cv -= NPB_CV_CURRENT_STEP;
    }
    else if ((battery.current < BAT_TARGET_CURRENT - 0.1)
            && (battery.current_cv <= NPB_MAX_CURRENT - NPB_CV_CURRENT_STEP))
    {
        battery.current_cv += NPB_CV_CURRENT_STEP;
    }
    npb.cmd.set_current = battery.current_cv;

    if (++battery.charge_timer >= NPB_CHARGE_TIMEOUT) {
        /* Timeout di carica. Cosa fare? XXX facciamo finta che la
           batteria ormai sia completamente carica */
        battery_fully_charged();
    }
}

/** Passa in modalità ricarica rapida completa: termina su taper di
 * corrente (o su timeout come backup) */
void npb_chrg_enter_boost(void)
{
    battery.charge_state = BATCHA_BOOST;
    npb.cmd.set_voltage = NPB_BOOST_VOLTAGE;
    npb_chrg_current_reset();
    battery.taper_timer = 0;
}

/** Passa in modalità ricarica lenta: termina su contatore di cariche (e
 * su backup, come sempre) */
static void npb_chrg_enter_float(void)
{
    battery.charge_state = BATCHA_FLOAT;
    npb.cmd.set_voltage = NPB_FLOAT_VOLTAGE;
    npb_chrg_current_reset();
}

/** Passa in carica; se in boost o float dipende da un paio di condizioni */
void npb_chrg_enter_charge(void)
{
    /* Carica completa di ricalibrazione se ha fatto 10 cicli completi
     * in float. Oppure batteria orribilmente scarica dalla tensione
     * sotto carico. */
    if (battery.low_battery)
    {
        battery.float_cycle_counter = NPB_FLOAT_CHARGE_COUNT;
    }
    if (battery.float_cycle_counter >= NPB_FLOAT_CHARGE_COUNT)
    {
        npb_chrg_enter_boost();
    }
    else
    {
        npb_chrg_enter_float();
    }
}

/** Passa in modalità attesa scarica */
static void npb_chrg_enter_wait_disch(void)
{
    npb_chrg_minimum_power();
    battery.charge_state = BATCHA_WAIT_DISCH;
}

/** Batteria non in ricarica, scarica naturale fino a soglia

    In questo stato il caricabatterie è a potenza minima.
    La batteria si scarica lentamente con l'elettronica. */
static void battery_charge_wait_discharge(void)
{

    /* Monitoriamo la carica usata: se scende sotto il 50% (3.6 Ah
       consumati ovvero <= -3.6) facciamo un ciclo in boost. Lo stesso
       vale se la batteria è orribilmente scarica o dopo un certo numero
       di cariche in float. Il float_cycle_counter viene azzerato solo
       alla fine della ricarica completa; la condizione in questo ramo
       permette di riprendere una carica completa dopo
       l'interruzione. */
    if (battery.low_battery
            || (battery.float_cycle_counter >= NPB_FLOAT_CHARGE_COUNT)
            || (battery.charge < BAT_BOOST_START_CHARGE))
    {
        npb_chrg_enter_boost();
    }
    /* Quando si vede un consumo accumulato di 720 mAh (ovvero -0.72 Ah
       dallo zero): decide se entrare in float o in boost a seconda del
       numero di cicli passati (per compensare la deriva del contatore
       di cariche). In aggiunta la condizione di backup è il DC link
       sotto ad una soglia di batteria visibilmente (ma non
       orribilmente) scarica. */
    else if ((battery.charge < BAT_FLOAT_START_CHARGE)
            || (battery.dclink < BAT_FLOAT_START_VOLTAGE))
    {
        npb_chrg_enter_charge();
    }
    /* Se non ha meno di 720 mAh di scarica la lasciamo stare,
       in modo da poter assorbire l'energia rigenerata */
}

/** Rabbocco batteria in float (a bassa tensione) */
static void battery_charge_float(void)
{
    /* In rabbocco la fine carica è governata dal contatore di
     * cariche. Sfortunatamente le batterie AGM non hanno
     * rendimento costante di assorbimento quindi ogni tanto
     * devono essere caricate completamente con l'algoritmo di
     * taper */
    if (battery.charge >= 0.0f)
    {
        /* Un ciclo di float è stato completato, prima o poi ripartirà in
         * boost */
        ++battery.float_cycle_counter;
        battery.charge_timer = 0;
        npb_chrg_enter_wait_disch();
    }
    else
    {
        npb_chrg_current_step();
    }
}

/** Carica batteria completa in boost (ad alta tensione) */
static void battery_charge_boost(void)
{
    /* Prerequisito: deve stare caricando; in linea teorica
     * è possibile che il carico di cabina sia eccessivo. */
    if (battery.current > 0)
    {
        /* se la corrente assorbita dalle batterie scende sotto
           soglia di ripristino posso rimettere in servizio */
        if (battery.current <= BAT_RESTART_CURRENT)
        {
            battery.low_battery = false;
        }
        /* Fine carica rilevata quando il consumo scende sotto 1.2A */
        if (battery.current <= BAT_TAPER_CURRENT)
        {
            if (battery.taper_timer >= NPB_TAPER_TIME)
            {
                battery_fully_charged();
            }
            else
            {
                ++battery.taper_timer;
            }
        }
        else
        {
            battery.taper_timer = 0;
        }
        npb_chrg_current_step();
    }
    else
    {
        /* Corrente negativa sulla batteria, prova a regolarla */
        npb_chrg_current_step();
        battery.taper_timer = 0;
    }
}

/** Macchina a stati per ricarica batterie, vedere documentazione di
 * progetto */
static void battery_charge(void)
{
    /* Gestione macchina a stati principale. Anche il fault non indica
     * necessariamente un guasto al caricabatteria quindi tanto vale
     * tenerla in efficienza. */
    if (pmode.state == PMODE_ST_CHARGER)
    {
        /* Tutto è comandato da un tick a 1 Hz */
        if (tmr_update(TMR_ID_NPB_CURRENT_REG, true))
        {
            switch (battery.charge_state)
            {
            case BATCHA_INIT:
                npb_chrg_enter_wait_disch();
                break;
            case BATCHA_WAIT_DISCH:
                battery_charge_wait_discharge();
                break;

            case BATCHA_FLOAT:
                battery_charge_float();
                break;

            case BATCHA_BOOST:
                battery_charge_boost();
                break;
            }
        }
    }
    else
    {
        /* Questa sezione non è governata dal tick batteria! siamo in
         * drive o in condizioni non abilitate alla carica. */
        npb_chrg_minimum_power();
        /* Con batteria sotto la soglia di scarica (sotto carico) la
         * macchina va fuori servizio e la dichiariamo orribilmente
         * scarica. */
        if (battery.dclink <= BAT_DISABLE_VOLTAGE)
        {
            battery.low_battery = true;
            bat_reset_charge_counter_empty();
        }
        /* Al ritorno tornerà nella modalità più adatta */
        battery.charge_state = BATCHA_WAIT_DISCH;
    }
}

/** Azzera il contatore di carica */
void bat_reset_charge_counter_full(void)
{
    /* Il power monitor è alimentato direttamente dalla batteria ed
     * è indipendente da resto della scheda; quindi continua a contare
     * anche in caso di reset. In questo caso si emula il centro scala
     * di default */
    ltc_writereg32(LTC_ACC_CHARGE_ADDR, 0x80000000);
}

void bat_reset_charge_counter_empty(void)
{
    /* Il power monitor è alimentato direttamente dalla batteria ed
     * è indipendente da resto della scheda; quindi continua a contare
     * anche in caso di reset. In questo caso si emula il centro scala
     * di default */
    ltc_writereg32(LTC_ACC_CHARGE_ADDR, 0x80000000 - BATTERY_DISCHARGED_MAH);
}

/** Macchina a stati lettura parametri power monitor */
void battery_scan(void)
{
    uint16_t rawdata;
    int16_t rawdata_signed;
    switch (battery_scan_step)
    {
    case BATSCAN_CONFIG:
        // devo attivare il sensore adc per fare le misurazioni
        ltc_writereg8(LTC_ADC_CONTROL_ADDR, 0b11011100); // read continua
        ++battery_scan_step;
        break;
    case BATSCAN_VOLTAGE:
        rawdata = ltc_readreg16(LTC_VOLTAGE_ADDR);
        battery.dclink = (float)rawdata * LTC_V_PER_LSB;
        ++battery_scan_step;
        break;
    case BATSCAN_CURRENT:
        rawdata = ltc_readreg16(LTC_CURRENT_ADDR);
        //cast a intero con segno (gestisce il bit RESULT[15])
        rawdata_signed = (int16_t)rawdata;
        // calcolo ottimizzato senza divisioni (48.75 / 32768 = 0.00148773)
        battery.current = (float)rawdata_signed * LTC_A_PER_LSB;
        ++battery_scan_step;
        break;
    case BATSCAN_TEMPERATURE:
        rawdata = ltc_readreg16(LTC_TEMP_ADDR);
        battery.ttf_temperature = ((float)rawdata * LTC_KELVIN_PER_LSB) - 273.15f;
        ++battery_scan_step;
        break;
    case BATSCAN_CHARGE:
        // leggo il valore attuale, che è unsigned con offset.
        // Si considera 80000000h come centro scala (zero) in quanto
        // è il valore di reset. */
    {
        int32_t acr_current = ltc_readreg32(LTC_ACC_CHARGE_ADDR)
                - 0x80000000;
        battery.charge = (float)acr_current * LTC_AH_PER_LSB;
        battery_scan_step = BATSCAN_VOLTAGE;
    }
        break;
    }
    /* Sulla base dei valori rilevati imposto parametri di carica */
    battery_charge();
    /* Controllo situazioni di overvoltage; ha un senso solo se la
       macchina è in drive; in boost il livello tende a essere
       superato */
    if ((pmode.state == PMODE_ST_DRIVE)
            && (battery.dclink > DCLINK_OVERVOLTAGE)) {
        battery.overvoltage = true;
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
