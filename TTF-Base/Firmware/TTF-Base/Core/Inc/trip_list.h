#ifndef TRIP_LIST_H
#define TRIP_LIST_H
/** Definizione dei trip (errori) */
enum trip_id {
    TRIP_PMODE_TO_CHARGER_TOUT,  /* Timeout in commutazione */
    TRIP_PMODE_TO_DRIVE_TOUT,    /* Timeout in commutazione */
    TRIP_SAFETY,                 /* Fault generale sicurezze */
    TRIP_S1_FUSE,                /* Catena sicurezze, fusibile */
    TRIP_S2_OVERTRAVEL,          /* Catena sicurezze, extracorsa */
    TRIP_S3_BELT,                /* Catena sicurezze, allentamenti */
    TRIP_S31_OVERSPEED,          /* Catena sicurezze, paracadute */
    TRIP_S32_CAR_ESTOP,          /* Stop in cabina (non bloccante) */
    TRIP_CHAIN_CABLE,            /* Cablaggio cavo catena */
    TRIP_UDEC_C_NMT,             /* Timeout beat UDEC.C */
    TRIP_TTF_CX_NMT,             /* Timeout beat TTF.CX */
    TRIP_TTF_CX_CABLE,           /* Congruenza cablaggio di tetto */
    TRIP_UZONE,                  /* Congruenza zone sblocco */
    TRIP_OTR_FB,                 /* Congruenza contatti master relay*/
    TRIP_TRAY_FB,                /* Congruenza contatti bypass tray */
    TRIP_TRAP_FB,                /* Congruenza contatti bypass trap */
    TRIP_STOP_TIMEOUT,           /* Timeout arresto motore */
    TRIP_CAN_BUFFER,             /* CAN Buffer overflow */
    TRIP_CAN_TX,                 /* CAN FIFO overflow */
    TRIP_TIMER,                  /* Errore timer */
    TRIP_TRAPTRAY,               /* Congruenza trap/tray */
    TRIP_OVERVOLTAGE,            /* Sovratensione DC link */
    TRIP_VFD,                    /* Errore VFD */
    TRIP_NPB,                    /* Errore caricabatteria */
    TRIP_DOOR,                   /* Timeout manovra porta */
    TRIP_ID_SIZE,                /* MARKER */
    TRIP_ID_NONE = TRIP_ID_SIZE
};

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
