#include "main.h"
#include "pmode.h"
#include "udec_cx.h"
#include "traptray_inl.h"
#include "trips.h"
struct traptray_info traptray;

/* Inlines */
bool traptray_in_position(void);
void traptray_req(enum traptray_state req);
bool is_trap_ok(void);
bool is_tray_ok(void);

/** Ciclo di scan per commutazione di trap e tray */
void traptray_scan_cycle(void)
{
    /* Se UDEC.CX non è attiva nulla si può dire o fare */
    if (udec_cx.online && udec_cx.in_good)
    {
        /* Determina lo stato corrente (il feedback è negato) */
        unsigned v = 0;
        if (!udec_cx.in.fbk_byp_top) v |= 1;
        if (!udec_cx.in.fbk_byp_bot) v |= 2;
        traptray.state = v;
        if (traptray_in_position())
        {
            /* Nulla da fare, è in posizione */
            return;
        }
        /* Determina la prossima manovra, deve passare dal no bypass */
        enum traptray_state next_state;
        if (traptray.state != TRAPTRAY_NO_BYPASS)
        {
            next_state = TRAPTRAY_NO_BYPASS;
        }
        else
        {
            next_state = traptray.req_state;
        }
        /* Comanda le uscite */
        switch (next_state)
        {
        case TRAPTRAY_TRAP_BYPASS:
            udec_cx.out.en_byp_top = true;
            udec_cx.out.en_byp_bot = false;
            break;
        case TRAPTRAY_TRAY_BYPASS:
            udec_cx.out.en_byp_top = false;
            udec_cx.out.en_byp_bot = true;
            break;
        default:
            udec_cx.out.en_byp_top = false;
            udec_cx.out.en_byp_bot = false;
        }
    }
}

/* Local Variables: */
/* tab-width: 4 */
/* End: */
