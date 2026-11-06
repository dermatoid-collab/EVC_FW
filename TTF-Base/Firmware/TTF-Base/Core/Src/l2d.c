/*
 * l2d.c
 *
 *  Created on: May 27, 2026
 *      Author: abertazza
 */
#include "main.h"
#include "can.h"
#include "timer.h"
#include "l2d.h"
#include "string.h"

static L2D_Payload_t l2d_write_payload[N_OPERATORI_MAX],
    l2d_write_payload_old[N_OPERATORI_MAX];

static L2D_Payload_t l2d_read_ack[N_OPERATORI_MAX],
    l2d_write_ack[N_OPERATORI_MAX];

static L2D_StatusWord_t l2d_status_word[N_OPERATORI_MAX];

bool l2d_online[N_OPERATORI_MAX];

bool l2d_closing(uint8_t idx)
{
	L2D_StatusCode_t status = l2d_status_word[idx].status;

	if (status == L2D_MovCl)
		return true;
	else
		return false;
}

void l2d_process_status_word(const struct can_frame *frame)
{
    /* estrae l'indirizzo operatore dal frame */
    uint8_t d_addr = (frame->id & 0xF800) >> 11;

    l2d_status_word[d_addr].status  = frame->data[0] & 0x0F;
    l2d_status_word[d_addr].errCode = frame->data[1];
    l2d_status_word[d_addr].voltage = frame->data[2] | frame->data[3]<<8;
    l2d_status_word[d_addr].nOpen   = frame->data[4] | frame->data[5]<<8;
    l2d_status_word[d_addr].temp    = frame->data[6] | frame->data[7]<<8;

    /* usiamo anche come beat */
    l2d_online[d_addr] = true;

    if (d_addr == 0)
    {
    	tmr_reset(TMR_ID_L2D_0_OFFLINE);
    }
    else
    {
    	tmr_reset(TMR_ID_L2D_8_OFFLINE);
    }
}

void l2d_process_read_resp(const struct can_frame *frame)
{
    uint8_t d_addr = (frame->id & 0xF800) >> 11;

    memset(l2d_read_ack, 0, sizeof(l2d_read_ack));
    l2d_read_ack[d_addr].address = frame->data[0] | frame->data[1]<<8;
    l2d_read_ack[d_addr].command = frame->data[2] | frame->data[3]<<8;
}

void l2d_process_write_resp(const struct can_frame *frame)
{
    uint8_t d_addr = (frame->id & 0xF800) >> 11;

    memset(l2d_write_ack, 0, sizeof(l2d_write_ack));
    l2d_write_ack[d_addr].address = frame->data[0] | frame->data[1]<<8;
    l2d_write_ack[d_addr].command = frame->data[2] | frame->data[3]<<8;
}

static void l2d_write_request(unsigned idx)
{
    // preparo il buffer settando i byte dopo il 4o a 0
    uint8_t dataBuffer[8];
    memset(dataBuffer, 0, sizeof(dataBuffer));
//    l2d_write_ack[idx].w = 0;

    dataBuffer[0] = l2d_write_payload[idx].address & 0x00FF;
    dataBuffer[1] = l2d_write_payload[idx].address >> 8;
    dataBuffer[2] = l2d_write_payload[idx].command & 0x00FF;
    dataBuffer[3] = l2d_write_payload[idx].command >> 8;

    can_tx_push_fullframe_ext(UDEC_CAN_ID(UDEC_TYPE_L2D, idx, UDEC_ID_WR_PARAM_REQ), dataBuffer, CAN125);

    l2d_write_payload_old[idx].w = l2d_write_payload[idx].w;
}

void l2d_writepayload(L2D_ParamAddr_t addr, uint16_t cmd, unsigned idx)
{
	l2d_write_payload[idx].address = addr;
	l2d_write_payload[idx].command = cmd;
}

void l2d_commandrequest(uint16_t cmd, unsigned idx)
{
	l2d_write_payload[idx].address = L2D_CmdReq;
	l2d_write_payload[idx].command = cmd;
}


void l2d_scan(void)
{
    /* per ciclare tra un indirizzo e l'altro degli operatori
     * turn e' booleano che cambia ad ogni ciclo (ho due operatori)
     * index assume valore 8 o 0 a seconda di turn
     * turn | index
     *  0   |   0
     *  1   |   8
     * in questo modo ad ogni scan agisco su una scheda diversa
     */

    static bool turn = false;
    uint8_t index = (turn) ? 8 : 0;
    turn = !turn;

    L2D_StatusCode_t status = l2d_status_word[index].status;
    static bool resend[9] = {false};
    static L2D_Payload_t resend_cmd[9] = {0};


    if (tmr_update(TMR_ID_L2D_0_OFFLINE, true))
    	l2d_online[0] = false;

    if (tmr_update(TMR_ID_L2D_8_OFFLINE, true))
    	l2d_online[8] = false;

    /* in autoapprendimento non mando comandi, essendo anche il valore
     * di inizializzazione non spammo se la scheda e' offline */
    if (!l2d_online[index] || status == L2D_SelfLearn)
    {
    	return;
    }
    // variazione payload comandi o richiesta di ripetizione
    if ((l2d_write_payload[index].w != l2d_write_payload_old[index].w) || resend[index])
    {
        if (resend[index])
        {
            // preparo il buffer da ripetere
            l2d_write_payload[index].w = resend_cmd[index].w;
            resend[index] = false;
        }

        // scrittura sul frame
        l2d_write_request(index);
    }
    /* se non ci sono state nuove richieste controllo che l'ack ricevuto sia corretto
     * altrimenti ripeto il messaggio al prossimo ciclo */
    else if (l2d_write_ack[index].w != l2d_write_payload[index].w)
    {
        resend[index] = true;
        resend_cmd[index].w = l2d_write_payload[index].w;

        // Mando lo stop sulla porta
		l2d_commandrequest(L2D_NoReq, index);
    }
}


/* TODO: valutare se togliere l'alternanza indirizzi qui:
 *  se il cambio di stato nella door_loop avviene in un solo ciclo di clock
 *  rischio di non comandare entrambe le porte ma solo la prima */
void l2d_set_neutral(void)
{
    L2D_StatusCode_t status_a, status_b;
	status_a = l2d_status_word[0].status;
	status_b = l2d_status_word[8].status;

	if ((status_a > L2D_StopMid)
			|| (status_b > L2D_StopMid))
	{
		l2d_commandrequest(L2D_NoReq, 0);
		l2d_commandrequest(L2D_NoReq, 8);
	}

}

void l2d_set_open(void)
{
    /* per ciclare tra un indirizzo e l'altro degli operatori */
    static bool turn = false;
    uint8_t index = (turn) ? 8 : 0;
    turn = !turn;

    /* gestisco variabili d'appoggio per ogni indirizzo */
    static uint32_t timestamp[9] = {0};
    /* aggiungo un ritardo tra uno stop e l'invio del messaggio per
     * assicurarmi che lo stato cambi correttamente */
    static bool is_waiting[9] = {false};

    L2D_StatusCode_t status = l2d_status_word[index].status;

    // Se la porta si sta già chiudendo o è già ferma in chiusura, usciamo subito
    if (status == L2D_MovOp || status == L2D_StopOp)
    {
        is_waiting[index] = false;
        return;
    }

    // Se non siamo ancora nella fase di attesa del timer, verifichiamo se serve noReq
    if (!is_waiting[index])
    {
        switch (status)
        {
        case L2D_MovCl:
        case L2D_Obstacle:
            // La porta sta muovendo/ferma in apertura: mandiamo lo stop (NoReq)
            l2d_commandrequest(L2D_NoReq, index);
            timestamp[index] = uwTick;
            is_waiting[index] = true;
            break;

        case L2D_Init:
        case L2D_StopMid:
        case L2D_StopErr:
        case L2D_StopCl:
            /* non necessito di un no_req per accettare comandi */
            l2d_commandrequest(L2D_OpReq, index);
            break;

        case L2D_SelfLearn:
        case L2D_StopOp:
        case L2D_MovOp:
            /* NOP */
            break;
        }
    }
    else
    {
        /* Siamo nella fase di attesa (il payload è attualmente su NoReq)
         * e devo attendere la macchina a stati di L2D che comunichi
         * il nuovo stato porta */
        if (uwTick - timestamp[index] >= 500)
        {
        	/* posso aggiornare il payload dopo aver inviato no_req */
            l2d_commandrequest(L2D_OpReq, index);
            is_waiting[index] = false;
        }
    }
}

void l2d_set_close(void)
{
    /* per ciclare tra un indirizzo e l'altro degli operatori */
    static bool turn = false;
    uint8_t index = (turn) ? 8 : 0;
    turn = !turn;

    /* gestisco variabili d'appoggio per ogni indirizzo */
    static uint32_t timestamp[9] = {0};
    /* aggiungo un ritardo tra uno stop e l'invio del messaggio per
     * assicurarmi che lo stato cambi correttamente */
    static bool is_waiting[9] = {false};

    L2D_StatusCode_t status = l2d_status_word[index].status;

    // Se la porta si sta già chiudendo o è già ferma in chiusura, usciamo subito
    if (status == L2D_MovCl || status == L2D_StopCl)
    {
        is_waiting[index] = false;
        return;
    }

    // Se non siamo ancora nella fase di attesa del timer, verifichiamo se serve NoReq
    if (!is_waiting[index])
    {
        switch(status)
        {
        case L2D_MovOp:
        case L2D_Obstacle:
            // La porta sta muovendo/ferma in apertura: mandiamo lo STOP (NoReq)
            l2d_commandrequest(L2D_NoReq, index);
            timestamp[index] = uwTick;
            is_waiting[index] = true;
            break;

        case L2D_Init:
        case L2D_StopMid:
        case L2D_StopErr:
        case L2D_StopOp:
            /* non necessito di un no_req per accettare comandi */
            l2d_commandrequest(L2D_ClReq, index);
            break;

        case L2D_SelfLearn:
        case L2D_StopCl:
        case L2D_MovCl:
            /* NOP */
            break;
        }
    }
    else
    {
        // Siamo nella fase di attesa (il payload è attualmente su NoReq)
        if (uwTick - timestamp[index] >= 500)
        {
        	/* posso aggiornare il payload dopo aver inviato no_req */
            l2d_commandrequest(L2D_ClReq, index);
            is_waiting[index] = false;
        }
    }
}


/* true -> fine invio
 * false -> invio in corso */
bool l2d_param_setup(void)
{
	/*
	 * Parametri MD2
	 * V_OPEN -> 12
	 * V_CLOSE -> 12
	 * DS_OPEN -> 250
	 * DS_CLOSE -> 200
	 * DOOR_TYPE -> 3
	 * PWM -> 300
	 * */

    typedef enum {
        WAIT_ONLINE,
        V_OPEN,
        V_CLOSE,
        DS_OPEN,
        DS_CLOSE,
		DOOR_TYPE,
		PWM,
        SEND,
        END,
    }l2d_fsm;
    static unsigned timestamp, lastCopied;
    static l2d_fsm FSM_LoadParam;


    switch (FSM_LoadParam)
    {
    case WAIT_ONLINE:
        /* devono essere entrambe online */
        if (l2d_online[0] && l2d_online[8])
            FSM_LoadParam = V_OPEN;
    	break;

    case V_OPEN:
    	/* preparo il buffer */
        l2d_writepayload(L2D_OpenSpeed, 12, 0);
        l2d_writepayload(L2D_OpenSpeed, 12, 8);
        /* tengo traccia dell'ultimo stato percorso */
        lastCopied = V_OPEN;
        /* mi muovo nello stato di invio dati */
        FSM_LoadParam = SEND;
        timestamp = HAL_GetTick();
        break;

    case V_CLOSE:
        l2d_writepayload(L2D_CloseSpeed, 12, 0);
        l2d_writepayload(L2D_CloseSpeed, 12, 8);

        lastCopied = V_CLOSE;
        FSM_LoadParam = SEND;
        timestamp = HAL_GetTick();
        break;

    case DS_OPEN:
        l2d_writepayload(L2D_dsDecOp, 250, 0);
        l2d_writepayload(L2D_dsDecOp, 250, 8);
        lastCopied = DS_OPEN;
        FSM_LoadParam = SEND;
        timestamp = HAL_GetTick();
        break;

    case DS_CLOSE:
        l2d_writepayload(L2D_dsDecCl, 200, 0);
        l2d_writepayload(L2D_dsDecCl, 200, 8);
        lastCopied = DS_CLOSE;
        FSM_LoadParam = SEND;
        timestamp = HAL_GetTick();
        break;

    case DOOR_TYPE:
        l2d_writepayload(L2D_DoorType, 3, 0);
        l2d_writepayload(L2D_DoorType, 3, 8);
        lastCopied = DOOR_TYPE;
        FSM_LoadParam = SEND;
        timestamp = HAL_GetTick();
        break;

    case PWM:
    	l2d_writepayload(L2D_PWMClose, 300, 0);
    	l2d_writepayload(L2D_PWMClose, 300, 8);
        lastCopied = PWM;
        FSM_LoadParam = SEND;
        timestamp = HAL_GetTick();
        break;

    case SEND:
        /* controllo di non aver raggiunto l'ultimo stato */
    	if (lastCopied < SEND)
    	{
    		if (HAL_GetTick() - timestamp >= 200)
    		{
    			/* l2d_scan() spedisce i frame, aspetto (abbondantemente)
    			 * di averli inviati entrambi e di aver ricevuto risposta */
       			FSM_LoadParam = ++lastCopied;
                timestamp = HAL_GetTick();
    		}
    	}
    	else
    	{
    		/* ho inviato tutti i parametri */
            FSM_LoadParam = END;
    	}
        break;

    case END:
        break;
    }

    if (FSM_LoadParam == END)
    	return true;
    else
    	return false;

}

/* Local Variables: */
/* tab-width: 4 */
/* End: */
