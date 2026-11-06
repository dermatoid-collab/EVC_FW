#include <stdint.h>
#include <stdbool.h>
#include "run_mode.h"
#include "application_inl.h"
#include "HMI.h"
#include "timer.h"
#include "vfd_inl.h"
#include "udec_c_inl.h"
#include "door_inl.h"
#include "traptray_inl.h"
#include "npb.h"
#include "l2d.h"

struct run_mode_info run_mode;

/** Test paracadute */
static void run_mode_free_fall(void)
{
    run_mode.busy = false;
    if (hmi_buttons()->w == 2 || DIn.In_4)
    {
        pmode_relay_set(true);

        if (DIn.s8)
        {
            vfd_cmd_request(VFD_COMMAND_OPEN_BRAKE);
        }
        else
        {
            vfd_reset();
        }
        tmr_reset(TMR_ID_MACH_PARK_TIME);
    }
    else
    {
        vfd_halt();
        if (vfd_in_fault())
        {
            vfd_reset();
        }
    }

    /* se per due minuti non premo nulla torno in modalita normale */
    if (tmr_update(TMR_ID_MACH_PARK_TIME,
                    (!run_mode.call && !hmi_buttons()->w)))
    {
        run_mode.state = MACHINE_STATE_NORMAL;
    }
}

/* Considerazioni generali sul movimento:
   - run_mode.busy comanda la lampada in cabina
   - run_mode.wait_for_enable è usato solo all'avvio del movimento
     quando è tutto in posizione ma l'enable non è ancora arrivato.
     Nelle altre situazioni si tiene a true

*/

/** Stop ordinario per rilascio chiamata */
static void run_mode_release_call(void)
{
    vfd_stop(VFD_ATIME_SLOW);
    run_mode.state = MACHINE_STATE_NORMAL;
}

/** Arresto rapido per inconvenienti vari. Passa in attesa rilascio
 * prenotazione (bisogna rilasciare e ripremere) */
static void run_mode_halt_call(void)
{
    vfd_halt();
    run_mode.state = MACHINE_STATE_WAIT_FOR_CALL_RELEASE;
}

/** Passa in chiusura porta se non è in posizione */
static inline bool run_mode_maybe_close_door(void)
{
    return (!door_is_closed() || !door_is_locked());
}

/** Sequenza di enable:
    - Il trap/tray deve essere in posizione
    - Si aspetta che salga l'enable
    - A questo punto ritorna true, quindi ci si può muovere
    - Se cade l'enable o se non ci sono le condizioni ritorna false
    - Una volta caduto l'enable deve essere resettata mettendo
      run_mode.wait_for_enable a true.
*/
static bool run_mode_wait_for_enable(bool ok_to_move)
{
    if (traptray_in_position())
    {
        /* Prima fase: si aspetta che arrivi l'enable */
        if (run_mode.wait_for_enable)
        {
            if (run_mode.enable)
            {
                run_mode.wait_for_enable = false;
            }
        }
        else
        {
            /* L'enable è stato latchato */
            if (run_mode.enable &&
                    (run_mode.bypass_manutenzione || ok_to_move))
            {
                return true;
            }
            else
            {
                run_mode_halt_call();
            }
        }
    }
    return false;
}

/** Fase iniziale sblocco freni. Si comanda il movimento a velocità
 * minima (10 RPM), quando si rileva l'apertura del freno si passa al
 * movimento comandato. */
static bool run_mode_brake_release(void)
{
    if (tmr_update(TMR_ID_BRAKE_RELEASE, DIn.brk_fb))
    {
        /* Freno in posizione, passa a velocità nominale */
        return true;
    }
    else
    {
        /* La marcia per aprire il freno è sempre verso l'alto, per
         * minimizzare le potenziali scivolate */
        vfd_move_request(RPM_BRAKE_RELEASE, VFD_RUN_FWD,
                ACC_BRAKE_RELEASE, ACC_BRAKE_RELEASE);
        return false;
    }
}

/** Movimento verso il basso, considera velocità e accelerazione */
static void run_mode_go_down(void)
{
    uint16_t v, a;
    /* In bypass sicuramente il minimo */
    if (run_mode.bypass_manutenzione)
    {
        v = VFD_SPEED_SLOW;
        a = VFD_ATIME_SLOW;
    }
    else if (magnet_ftc())
    {
        /* Il magnete per ora fa rallentamento */
        v = VFD_SPEED_SLOW;
        a = VFD_ATIME_SLOW;
    }
    else
    {
        v = VFD_SPEED_FAST;
        a = VFD_ATIME_FAST;
    }
    vfd_move_request(v, VFD_RUN_REV, a, a);
}

/* Il trap è a monte del tray sulla catena delle sicurezze, quindi se
   scatta cade anche il tray! */

/** Macchina in movimento verso il basso */
static void run_mode_down(void)
{
    door_loop(DOOR_CMD_RESET);
    run_mode.busy = true;
    /* Durante la discesa non si bypassa nulla */
    traptray_req(TRAPTRAY_NO_BYPASS);
    run_mode.overload = false;

    if (is_at_P0()) {
        /* A destinazione */
        vfd_stop(VFD_ATIME_SLOW);
        run_mode.state = MACHINE_STATE_OPEN_DOOR;
    }
    else if (run_mode.call == PREN_P0)
    {
        /* Corsa verso il P1 */
        if (run_mode_wait_for_enable(is_trap_ok() && is_tray_ok()) &&
                run_mode_brake_release())
        {
            run_mode_go_down();
        }
        else if (!(run_mode.bypass_manutenzione || is_trap_ok()))
        {
            run_mode_halt_call();
        }
        else if (!(run_mode.bypass_manutenzione || is_tray_ok()))
        {
            vfd_halt();
            run_mode.state = MACHINE_STATE_STUCK_UP;
        }
    }
    else
    {
        run_mode_release_call();
    }
}

/** Bloccata durante il movimento verso il basso dal trap. Per specifica
 * non è possibile muoversi in nessuna direzione fino alla rimozione
 * dell'inceppamento. Non siamo al piano, altrimenti il problema non
 * esiste. */
static void run_mode_stuck_up(void)
{
    /* Porta ferma, nessun bypass */
    door_loop(DOOR_CMD_RESET);
    traptray_req(TRAPTRAY_NO_BYPASS);
    /* Siamo fermi in mezzo al vano, teniamo la luce accesa */
    run_mode.busy = true;
    run_mode.wait_for_enable = true;
    run_mode.overload = false;

    /* Riprende su rilascio */
    if (traptray_in_position() && is_trap_ok())
    {
        /* È comunque necessario ripremere il tasto */
        run_mode.state = MACHINE_STATE_WAIT_FOR_CALL_RELEASE;
    }
}

/** Movimento verso l'alto, considera velocità e accelerazione */
static void run_mode_go_up(void)
{
    uint16_t v, a;
    /* In bypass sicuramente il minimo */
    if (run_mode.bypass_manutenzione)
    {
        v = VFD_SPEED_SLOW;
        a = VFD_ATIME_SLOW;
    }
    else if (magnet_ftc())
    {
        /* Il magnete per ora fa rallentamento */
        v = VFD_SPEED_SLOW;
        a = VFD_ATIME_SLOW;
    }
    else
    {
        v = VFD_SPEED_FAST;
        a = VFD_ATIME_FAST;
    }
    vfd_move_request(v, VFD_RUN_FWD, a, a);
}

/** Macchina in movimento verso l'alto */
static void run_mode_up(void)
{
    door_loop(DOOR_CMD_RESET);
    run_mode.busy = true;
    run_mode.overload = false;

    /* Tray bypass: non è mai necessario bloccarsi mentre si sale con il
     * fondo premuto */
    traptray_req(TRAPTRAY_TRAY_BYPASS);

    if (run_mode_maybe_close_door())
    {
        run_mode.state = MACHINE_STATE_CLOSE_DOOR;
    }
    else if (is_at_P1()) {
        /* A destinazione */
        vfd_stop(VFD_ATIME_SLOW);
        run_mode.state = MACHINE_STATE_OPEN_DOOR;
    }
    else if (run_mode.call == PREN_P1)
    {
        /* Corsa verso il P1 */
        if (run_mode_wait_for_enable(is_trap_ok()) &&
                run_mode_brake_release())
        {
            run_mode_go_up();
        }
    }
    else
    {
        run_mode_release_call();
    }
}

/** Macchina in movimento verso l'alto */
static void run_mode_wait_for_call_release(void)
{
    door_loop(DOOR_CMD_RESET);
    run_mode.busy = true;
    traptray_req(TRAPTRAY_NO_BYPASS);
    run_mode.overload = false;
    if (run_mode.call == PREN_NONE)
    {
        run_mode.state = MACHINE_STATE_NORMAL;
    }
}

/** Parte comune calcolo sovraccarico */
static bool run_mode_check_overload(void)
{
    /* L'overload si controlla solo da fermi ai piani */
    run_mode.overload = overload_in() && (is_at_P0() || is_at_P1());

    /* Forza la porta aperta in sovraccarico */
    if (run_mode.overload && !run_mode.bypass_manutenzione)
    {
        run_mode.state = MACHINE_STATE_OPEN_DOOR;
        return true;
    }
    return false;
}

/** Ciclo di apertura delle porte, fermi al piano */
static void run_mode_open_door(void)
{
    /* Siamo fermi al piano, nessun bypass */
    traptray_req(TRAPTRAY_NO_BYPASS);
    run_mode.busy = true;
    run_mode.wait_for_enable = true;
    run_mode_check_overload();

    /* Ciclo di apertura */
    if (door_is_closed() || open_door_request)
    {
        door_loop(DOOR_CMD_OPEN);
    }
    else
    {
        door_loop(DOOR_CMD_NONE);
    }
    /* Fermi al piano con porte aperte: dopo il tempo di posteggio
     * chiudo le porte */
    if ((is_at_P0() || is_at_P1()) && run_mode_maybe_close_door())
    {
        if (tmr_update(TMR_ID_MACH_PARK_TIME, true)) {
            run_mode.state = MACHINE_STATE_CLOSE_DOOR;
        }
    }
    /* Prenotazione per diverso piano. Se c'è ancora sovraccarico
     * però non può chiudere la porta quindi rimane qui */
    if (((run_mode.call == PREN_P0 && is_at_P1()) ||
                    (run_mode.call == PREN_P1 && is_at_P0())) &&
            !run_mode.overload)
    {
        run_mode.state = MACHINE_STATE_CLOSE_DOOR;
    }
}

/** Stato chiusura porte */
static void run_mode_close_door(void)
{
    /* Siamo fermi al piano, nessun bypass */
    traptray_req(TRAPTRAY_NO_BYPASS);
    run_mode.busy = false;
    run_mode.wait_for_enable = true;
    if (run_mode_check_overload())
    {
        run_mode.state = MACHINE_STATE_OPEN_DOOR;
        return;
    }

    /* Primo problema: se non ci sono le sicurezze non si possono
     * leggere i contatti */
    if (!DIn.s3)
    {
        door_loop(DOOR_CMD_RESET);
        run_mode.state = MACHINE_STATE_NORMAL;
    }
    /* Manovra di chiusura */
    else if (run_mode_maybe_close_door() ||
            tmr_update(TMR_ID_MACH_DOOR_TIME, true))
    {
        door_loop(DOOR_CMD_CLOSE);
    }
    else
    {
        door_loop(DOOR_CMD_NONE);
    }
    /* PAP durante il ciclo di chiusura oppure ripressione del tasto al
     * piano */
    if (reopen_door_button() ||
            (run_mode.call == PREN_P0 && is_at_P0()) ||
            (run_mode.call == PREN_P1 && is_at_P1()))
    {
        set_open_door_request();
        door_loop(DOOR_CMD_RESET);
        run_mode.state = MACHINE_STATE_OPEN_DOOR;
    }
    /* Fine chiusura confermata */
    else if (door_is_closed() && door_is_locked())
    {
        run_mode.state = MACHINE_STATE_NORMAL;
    }
}

/** Sequenza di inizializzazione, passa in normale a fine ciclo */
static void run_mode_init_seq(void)
{
    typedef enum {
        IO_ABILITATION,
        NPB_PARAM,
        L2D_PARAM,
        VFD_PARAM,
        INIT_END
    } fsm_init;
    static fsm_init init_state;
    door_reset();
    enable_tlc();
    enable_p71_out();

    switch (init_state)
    {
    case IO_ABILITATION:
        vfd_stop(VFD_ATIME_SLOW);
        ++init_state;
        break;
    case NPB_PARAM:
        npb_set_psumode();
        npb_disable_eeprom();
        ++init_state;
        // NON FA L2D_PARAM DEBUG XXX
        ++init_state;
        break;
    case L2D_PARAM:
        if (l2d_param_setup())
        {
            l2d_set_neutral();
            ++init_state;
        }
        break;
    case VFD_PARAM:
        if (vfd_param_setup())
        {
            ++init_state;
        }
    case INIT_END:
        run_mode.state = MACHINE_STATE_NORMAL;
        break;
    }
}

/** Prenotazione in modo normale */
static void run_mode_normal_car_call(void)
{
    /* Siamo fermi, nessun bypass */
    traptray_req(TRAPTRAY_NO_BYPASS);

    switch (run_mode.call) {
        /* Chiamata verso il basso:
           - Al P0 apre la porta
           - In altre posizioni chiude la porta se non lo è
           - Se il tray è premuto non si muove
           - In manutenzione non comanda le porte
           - In ogni caso accende la luce in cabina */
    case PREN_P0:
        if (is_at_P0() && !run_mode.bypass_manutenzione)
        {
            run_mode.state = MACHINE_STATE_OPEN_DOOR;
        }
        else if (run_mode_maybe_close_door())
        {
            run_mode.state = MACHINE_STATE_CLOSE_DOOR;
        }
        else if (is_tray_ok())
        {
            run_mode.state = MACHINE_STATE_DOWN;
        }
        break;

        /* Chiamata verso l'alto, come sopra ma ovviamente scambiato */
    case PREN_P1:
        if (is_at_P1() && !run_mode.bypass_manutenzione)
        {
            run_mode.state = MACHINE_STATE_OPEN_DOOR;
        }
        else if (run_mode_maybe_close_door())
        {
            run_mode.state = MACHINE_STATE_CLOSE_DOOR;
        }
        else if (is_trap_ok())
        {
            run_mode.state = MACHINE_STATE_UP;
        }
        break;

        /* Nessun pulsante premuto, rimane fermo ma muove le porte al piano*/
    case PREN_NONE:
        door_loop(DOOR_CMD_NONE);
        vfd_stop(VFD_ATIME_SLOW);

        /* Gestione apriporte al piano: vanno in chiusura
         * automaticamente se non si usa il PAP */
        if (is_at_P0() || is_at_P1())
        {
            if (reopen_door_button())
            {
                set_open_door_request();
                run_mode.state = MACHINE_STATE_OPEN_DOOR;
                break;
            }
            else if (run_mode_maybe_close_door())
            {
                run_mode.state = MACHINE_STATE_CLOSE_DOOR;
            }
        }
        break;
    }
}

/** Funzionamento a piattaforma ferma */
static void run_mode_normal(void)
{
    /* Macchina ferma, nessun bypass necessario */
    traptray_req(TRAPTRAY_NO_BYPASS);
    run_mode.busy = run_mode.call || reopen_door_button();
    run_mode.wait_for_enable = true;

    /* Se tengo premuti i pulsanti 5s resetto la scheda */
    if (tmr_update(TMR_ID_COP_RESET_TIMEOUT, is_reset_btn()))
    {
        run_mode.reset_request = true;
    }

    /* Se rilevo batteria scarica e sono al piano metto la macchina
     * fuori servizio fino a carica minima di servizio */
    if (battery.low_battery && (is_at_P0() || is_at_P1()))
    {
        pmode_request(PMODE_REQ_CHARGER);
        /* Su tasto riapre la porta */
        if (run_mode.call || reopen_door_button())
        {
            run_mode.state = MACHINE_STATE_OPEN_DOOR;
        }
    } else {
        /* Normale funzionamento */
        run_mode_normal_car_call();
    }
}

/** Macchina a stati principale controllo movimento */
void run_mode_scan_cycle(void)
{
    switch (run_mode.state) {
    case MACHINE_STATE_INIT_SEQ:
        run_mode_init_seq();
        break;
    case MACHINE_STATE_NORMAL:
        run_mode_normal();
        break;
    case MACHINE_STATE_DOWN:
        run_mode_down();
        break;
    case MACHINE_STATE_OPEN_DOOR:
        run_mode_open_door();
        break;
    case MACHINE_STATE_CLOSE_DOOR:
        run_mode_close_door();
        break;
    case MACHINE_STATE_UP:
        run_mode_up();
        break;
    case MACHINE_STATE_STUCK_UP:
        run_mode_stuck_up();
        break;
    case MACHINE_STATE_FALL_TEST:
        run_mode_free_fall();
        break;
    case MACHINE_STATE_WAIT_FOR_CALL_RELEASE:
        run_mode_wait_for_call_release();
        break;
    case MACHINE_STATE_MAX:
        break;
    }

    overload_lamp(run_mode.overload);
    /* Luminosa di occupato (lampada di cabina) */
    set_occ(tmr_update(TMR_ID_OCC_TIMEOUT, run_mode.busy));
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
