#include "main.h"
#include "timer.h"
#include "pmode_inl.h"
#include "vfd_inl.h"
#include "trips_inl.h"
struct pmode_info pmode;
void pmode_request(enum pmode_requested new_mode);

/** Stato iniziale (di sicurezza): commutazione su caricabatteria.
    Notare che sono tutti valori a zero, quindi l'inizializzazione di
    default è corretta */
void pmode_reset(void)
{
    pmode.req_mode = PMODE_REQ_CHARGER;
    pmode.state = PMODE_ST_TO_CHARGER;
    trip_retract(TRIP_PMODE_TO_CHARGER_TOUT, true);
    trip_retract(TRIP_PMODE_TO_DRIVE_TOUT, true);
}

/** Un timeout in commutazione al caricabatteria non è grave, potrebbe
 * mancare l'alimentazione di linea */
bool pmode_in_fault(void)
{
    return trip_state_get(TRIP_PMODE_TO_CHARGER_TOUT) ||
            trip_state_get(TRIP_PMODE_TO_DRIVE_TOUT);
}

/** Aziona i relè per l'attivazione del VFD. In caso di fault di sicurezza non attiva l'OTR e tiene chiuso il freno. */
void pmode_relay_set(bool s)
{
    if (is_trip_active(TRIP_SAFETY))
    {
        s = false;
    }
    /* Questo per poter sapere altrove in che stato è la bobina */
    pmode.output_coil = s;
    /* POWER ENABLE */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, s);

    /* OTR ENABLE - comando i rele del VFD */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, s);

    /* NPB DISABLE - consenso ricarica NPB */
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, s);

     /* BRAKE ENABLE */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, s);
};

/** Scan stato — Commutazione in corso da movimento a ricarica */
static void pmode_scan_to_charger(void)
{
    pmode_relay_set(false);
    if (pmode.req_mode == PMODE_REQ_DRIVE)
    {
        /* Richiesta di commutazione */
        pmode.state = PMODE_ST_TO_DRIVE;
    }
    else if ((vfd.fault == VFD_FAULT_UNDERVOLTAGE)
            || (!vfd_is_running()))
    {
        /* se vfd va in undervoltage significa che il rele ha commutato */
        pmode.state = PMODE_ST_CHARGER;
    }
    else if (tmr_update(TMR_ID_PMODE_TO_CHARGER, true))
    {
        /* Timeout, commutazione fallita */
        trip_assert(TRIP_PMODE_TO_CHARGER_TOUT, true);
    }
}

/* Scan cycle — In ricarica */
static void pmode_scan_charger(void)
{
    pmode_relay_set(false);
    trip_retract(TRIP_PMODE_TO_CHARGER_TOUT, true);
    if (pmode.req_mode == PMODE_REQ_DRIVE)
    {
        /* Richiesta di commutazione */
        pmode.state = PMODE_ST_TO_DRIVE;
    }
    else if (vfd.current_req.direction != VFD_RUN_STOP)
    {
        /* Anomalia: siamo in ricarica e il VFD è in moto */
        pmode.state = PMODE_ST_TO_CHARGER;
    }
}

/* Scan cycle — Commutazione in corso da ricarica a movimento */
static void pmode_scan_to_drive(void)
{
    pmode_relay_set(true);
    if ((pmode.req_mode == PMODE_REQ_CHARGER) ||
            (pmode.req_mode == PMODE_REQ_CHARGER_NOW))
    {
        pmode.state = PMODE_ST_TO_CHARGER;
    }
    else if (vfd_is_ready())
    {
        /* Non solo deve essere attivo, ma anche pronto all'uso */
        pmode.state = PMODE_ST_DRIVE;
    }
    else if (vfd_in_pseudofault())
    {
        /* È uno stop atteso, manda un reset */
        vfd_reboot();
    }
    else if (tmr_update(TMR_ID_PMODE_TO_CHARGER, true))
    {
        /* Timeout, commutazione fallita */
        trip_assert(TRIP_PMODE_TO_CHARGER_TOUT, true);
    }
}

/* Scan cycle — Pronto al movimento */
static void pmode_scan_drive(void)
{
	pmode_relay_set(true);
    trip_retract(TRIP_PMODE_TO_DRIVE_TOUT, true);
    if (pmode.req_mode == PMODE_REQ_CHARGER)
	{
		/* Richiesta di passaggio ritardato a ricarica */
        pmode.state = PMODE_ST_WAIT_TO_CHARGER;
	}
    else if (pmode.req_mode == PMODE_REQ_CHARGER_NOW)
	{
		/* Richiesta di passaggio immediato a ricarica */
        pmode.state = PMODE_ST_TO_CHARGER;
	}
    else if (!vfd_is_ready())
    {
        /* Anomalia: per qualche motivo il VFD non è a posto (può essere
         * in fault o con DC link basso) */
        pmode.state = PMODE_ST_TO_DRIVE;
	}
}

/* Scan cycle — Tempo di grazia prima dello spegnimento del VFD.
 * Deve essere sufficiente per permettere il rallentamento e l'arresto
 * per evitare scossoni e fault. */
static void pmode_scan_wait_to_charger(void)
{
    pmode_relay_set(true);
    if (pmode.req_mode == PMODE_REQ_DRIVE) {
        /* Richiesta di passaggio a VFD; è ancora attivo quindi
         * immediato */
        pmode.state = PMODE_ST_DRIVE;
    } else if ((pmode.req_mode == PMODE_REQ_CHARGER_NOW) ||
            (tmr_update(TMR_ID_PMODE_WAIT_TO_CHARGER, true))) {
        /* Richiesta di passaggio immediato a ricarica, o timeout
         * tempo di grazia */
        pmode.state = PMODE_ST_TO_CHARGER;
    }
}

/** Scan cycle — Controllo power mode, entry point */
void pmode_scan_cycle(void)
{
    switch (pmode.state) {
    case PMODE_ST_TO_CHARGER:
        pmode_scan_to_charger();
        break;
    case PMODE_ST_CHARGER:
        pmode_scan_charger();
        break;
    case PMODE_ST_TO_DRIVE:
        pmode_scan_to_drive();
        break;
    case PMODE_ST_DRIVE:
        pmode_scan_drive();
        break;
    case PMODE_ST_WAIT_TO_CHARGER:
        pmode_scan_wait_to_charger();
        break;
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
