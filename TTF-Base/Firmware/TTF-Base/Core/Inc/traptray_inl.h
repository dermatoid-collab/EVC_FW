#ifndef TRAPTRAY_INL_H
#define TRAPTRAY_INL_H
#include <stdbool.h>
#include "traptray.h"
#include "run_mode.h"
#include "udec_cx.h"

/** Predicato: la commutazione dei bypass è completa. In emergenza il
 * controllo è bypassato. */
inline bool traptray_in_position(void) {
    return run_mode.bypass_manutenzione ||
            (traptray.state == traptray.req_state);
}

/** Richiede un cambio stato. */
inline void traptray_req(enum traptray_state req) {
    traptray.req_state = req;
}

/** Predicato: il trap non è premuto. In emergenza viene comunque
 * bypassato. */
inline bool is_trap_ok(void) {
    return run_mode.bypass_manutenzione
            || (udec_cx.in_good && udec_cx.in.s3c);
}

/** Predicato: il tray non è premuto. In emergenza viene comunque
 * bypassato. */
inline bool is_tray_ok(void) {
    return run_mode.bypass_manutenzione
            || (udec_cx.in_good && udec_cx.in.s4);
}
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
