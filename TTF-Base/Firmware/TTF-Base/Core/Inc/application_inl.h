#ifndef APPLICATION_H
#define APPLICATION_H
#include "application.h"
#include "udec_c_inl.h"
#include "udec_cx_inl.h"

/** Pulsante di reset: up/down su pannello, o su COP */
inline bool is_reset_btn(void) {
    return ((DIn.In_1 && DIn.In_2)
            /* Reset remoto da COP */
            || (udec_c.in_good && udec_c.in.sb_pc0 && udec_c.in.sb_pc1));
}

/** Presenza al P0 */
inline bool is_at_P0(void) {
    return bot_unlock_zone();
}

/** Presenza al P1 */
inline bool is_at_P1(void) {
    return top_unlock_zone();
}
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
