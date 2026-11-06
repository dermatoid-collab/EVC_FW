#include <stdbool.h>
#include "trips.h"
#include "door.h"
#include "udec_c_inl.h"
#include "udec_cx_inl.h"
#include "l2d.h"

/** Stato ciclo porte */
enum door_status door_status;
/** Richiesta apertura porte */
bool open_door_request;

/** Passa allo stato di porta ferma, a seconda dello stato delle
 * sicurezze */
void door_reset(void)
{
    if (door_is_closed() && door_is_locked())
    {
        door_status = DOOR_LOCKED;
    }
    else
    {
        door_status = DOOR_STOPPED;
    }
}

/** Porta ferma, non chiusa. Stato iniziale. */
static void door_stopped(enum door_cmd cmd)
{
    reopen_lamp(false);
    door_set_neutral();
    l2d_set_neutral();
    door_unlock();

    switch (cmd)
    {
    case DOOR_CMD_CLOSE:
        /* Passa comunque in chiusura, la porta potrebbe essere a mezza
         * via */
        door_status = DOOR_CLOSING;
        break;

    case DOOR_CMD_OPEN:
        if (door_is_closed() || door_is_locked())
        {
            door_status = DOOR_UNLOCKING;
        }
        break;

    case DOOR_CMD_NONE:
        /* Siamo già in questo stato */
        break;

    case DOOR_CMD_RESET:
        door_reset();
        break;
    }
}

/** Porta ferma, chiusa e bloccata. */
static void door_locked(enum door_cmd cmd)
{
    reopen_lamp(false);
    door_set_neutral();
    l2d_set_neutral();
    door_lock();

    switch (cmd)
    {
    case DOOR_CMD_OPEN:
        door_status = DOOR_UNLOCKING;
        break;

    case DOOR_CMD_NONE:
    case DOOR_CMD_CLOSE:
        /* Siamo già in questo stato */
        break;

    case DOOR_CMD_RESET:
        door_reset();
        break;
    }

    if (!door_is_closed() || !door_is_locked())
    {
        /* Si è mosso qualcosa di anomalo */
        door_reset();
    }
}

/** Porta in chiusura. */
static void door_closing(enum door_cmd cmd)
{
    reopen_lamp(false);
    door_unlock();
    door_set_close();
    l2d_set_close();
    switch (cmd)
    {
    case DOOR_CMD_NONE:
    case DOOR_CMD_CLOSE:
        if (tmr_update(TMR_ID_LOCK_DELAY_CL, !door_is_closed()))
        {
//            trip_assert(TRIP_DOOR, true);
            /* XXX Timeout chiusura */
        }
        else if (tmr_update(TMR_ID_DOOR_APPROACH, door_is_closed()))
        {
            /* Attende l'accostamento per poi bloccare la porta */
            door_status = DOOR_LOCKING;
        }
        break;

    case DOOR_CMD_OPEN:
        /* Retromarcia */
        door_status = DOOR_OPENING;
        break;

    case DOOR_CMD_RESET:
        door_reset();
        break;
    }
}

/** Porta in chiusura finale (rilascio scrocco). */
static void door_locking(enum door_cmd cmd)
{
    reopen_lamp(false);
    door_lock();
    door_set_close();
    l2d_set_close();
    switch (cmd)
    {
    case DOOR_CMD_NONE:
    case DOOR_CMD_CLOSE:
        /* se non riesce a bloccare la serratura */
        if (tmr_update(TMR_ID_UNLOCK_TIMEOUT, !door_is_locked()))
        {
            /* XXX timeout blocco */
//            trip_assert(TRIP_DOOR, true);
            door_status = DOOR_STOPPED;
        }
        else if (door_is_closed() && door_is_locked())
        {
            /* chiusura effettuata correttamente */
            door_status = DOOR_LOCKED;
        }
        break;

    case DOOR_CMD_OPEN:
        /* La porta potrebbe già essere sufficientemente chiusa */
        door_status = DOOR_UNLOCKING;
        break;

    case DOOR_CMD_RESET:
        door_reset();
        break;
    }
}

/** Porta in sblocco prima dell'apertura. */
static void door_unlocking(enum door_cmd cmd)
{
    reopen_lamp(true);
    door_unlock();
    door_set_neutral();
    l2d_set_neutral();

    switch (cmd)
    {
    case DOOR_CMD_NONE:
    case DOOR_CMD_OPEN:
        /* se si inceppa la serratura resetto il ciclo porte */
        if (tmr_update(TMR_ID_UNLOCK_TIMEOUT, door_is_locked()))
        {
            /* XXX timeout sblocco */
//            trip_assert(TRIP_DOOR, true);
            door_status = DOOR_STOPPED;
        }
        else if (!door_is_locked())
        {
            door_status = DOOR_OPENING;
        }
        break;

    case DOOR_CMD_CLOSE:
        door_status = DOOR_CLOSING;
        break;

    case DOOR_CMD_RESET:
        door_reset();
        break;
    }
}

/** Porta in apertura. A tempo dato che non c'è feedback di fine
 * corsa */
void door_opening(enum door_cmd cmd)
{
    reopen_lamp(true);
    door_unlock();
    door_set_open();
    l2d_set_open();

    switch (cmd)
    {
    case DOOR_CMD_NONE:
    case DOOR_CMD_OPEN:
        if (tmr_update(TMR_ID_MACH_DOOR_TIME, !door_is_closed()))
        {
            /* Non è un timeout, è il tempo di manovra */
            open_door_request = false;
            door_status = DOOR_STOPPED;
        }
        break;

    case DOOR_CMD_CLOSE:
        open_door_request = false;
        door_status = DOOR_CLOSING;
        break;

    case DOOR_CMD_RESET:
        door_reset();
        break;
    }
}

/** Macchina comando porte automatiche e serratura. */
void door_loop(enum door_cmd cmd)
{
    /* Porta aperta -> comando chiusura, tengo serratura aperta
     * Porta chiusa -> inizio la battuta per liberare lo scrocco della serratura
     * Porta bloccata -> fine ciclo */

    /* Fuori zona sblocco lo scrocco è chiuso e non e neppure possibile
     * chiudere */
    if (!udec_cx.in.top_uzone && !udec_cx.in.bot_uzone)
    {
        /* Fuori zona sblocco, non è possile aprire */
        door_reset();
    }

    /* Caso speciale, richiesta di apertura forzata */
    if (open_door_request)
    {
        cmd = DOOR_CMD_OPEN;
    }
    switch (door_status)
    {
    case DOOR_STOPPED:
        door_stopped(cmd);
        break;

    case DOOR_CLOSING:
        door_closing(cmd);
        break;

    case DOOR_LOCKING:
        door_locking(cmd);
        break;

    case DOOR_OPENING:
        door_opening(cmd);
        break;

    case DOOR_UNLOCKING:
        door_unlocking(cmd);
        break;

    case DOOR_LOCKED:
        door_locked(cmd);
        break;
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
