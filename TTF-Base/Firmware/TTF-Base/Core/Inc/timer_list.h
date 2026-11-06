#ifndef TIMER_LIST_H
#define TIMER_LIST_H
/** Definizione dei timer */
enum tmr_id {
    TMR_ID_VFD_OFFLINE,         	/* Timeout comunicazione da VFD */
    TMR_ID_VFD_MASTER_HB,         	/* Timeout master heartbeat */
    TMR_ID_NPB_SCAN,            	/* Tempo fra richieste a NPB */
    TMR_ID_UDEC_C_OFFLINE,      	/* Timeout comunicazione con UDEC.C */
    TMR_ID_UDEC_CX_OFFLINE,      	/* Timeout comunicazione con UDEC.CX */
    TMR_ID_PMODE_TO_CHARGER,    	/* Timeout commutazione VFD → NPB */
    TMR_ID_PMODE_TO_DRIVE,      	/* Timeout commutazione NPB → VFD */
    TMR_ID_PMODE_WAIT_TO_CHARGER, 	/* Tempo di attesa prima della commutazione VFD → NPB */
    TMR_ID_MACH_PARK_TIME,        	/* Tempo parcheggio macchina */
    TMR_ID_MACH_DOOR_TIME,        	/* Tempo manovra porte */
    TMR_ID_HMI_RESET_TIMEOUT,		/* Timeout aggiornamento led caricabatteria */
    TMR_ID_COP_RESET_TIMEOUT,		/* Timeout aggiornamento led caricabatteria */
    TMR_ID_OCC_TIMEOUT,				/* timeout luce occupato */
    TMR_ID_LOCK_DELAY_OP,
    TMR_ID_LOCK_DELAY_CL,
    TMR_ID_VFD_RESET_DELAY,
    TMR_ID_NMT_PERIOD,				/* timeout invio NMT */
    TMR_ID_VFD_OUTPUT_DELAY,        /* attesa riapertura contattore in uscita */
    TMR_ID_VFD_OUTPUT_SWITCH,       /* attesa posizionamento contattore in uscita */
    TMR_ID_NPB_CURRENT_REG,         /* Regolazione corrente di carica */
    TMR_ID_OTR_REL,					/* attendo rele in posizione */
    TMR_ID_BRK_REL,					/* attendo rele in posizione */
    TMR_ID_FBK_BOT_REL,				/* attendo rele in posizione */
    TMR_ID_FBK_TOP_REL,				/* attendo rele in posizione */
    TMR_ID_ERR_SYNC,				/* timer trigger errori */
    TMR_ID_DOORUNLOCK_DELAY,		/* delay per sblocco serratura */
    TMR_ID_BUTTONS_LED,				/* lampeggio pulsanti */
    TMR_ID_UNLOCK_TIMEOUT,			/* timeout sblocco serratura */
    TMR_ID_LOCK_TIMEOUT,			/* timeout blocco serratura */
    TMR_ID_STOP_TIMEOUT,            /* timeout arresto comandato motore */
    TMR_ID_TRIP_DISPLAY,            /* Intervallo visualizzazione errori */
    TMR_ID_L2D_0_OFFLINE,			/* timeout comunicazione l2d 0 */
    TMR_ID_L2D_8_OFFLINE, 			/* timeout comunicazione l2d 8 */
    TMR_ID_BRAKE_RELEASE,           /* Tempo di ritardo a 10 rpm per rilascio freno */
    TMR_ID_DOOR_APPROACH,           /* Ritardo da accostamento a blocco */
    TMR_ID_SIZE,                    /* MARKER */
};

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
