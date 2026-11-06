#ifndef __TIMER_INL_H
#define __TIMER_INL_H
#include "timer.h"
#include "main.h"

inline void tmr_sv_set(enum tmr_id id, tmr_size sv) {
    tmr_array[id].sv = sv;
}

/* Caso speciale, fa solo estensione del tempo */
inline void tmr_sv_extend(enum tmr_id id, tmr_size sv) {
    if (tmr_array[id].sv < sv) {
        tmr_array[id].sv = sv;
    }
}

inline void tmr_pv_reset(enum tmr_id id) {
    tmr_array[id].pv = 0;
}

inline tmr_size tmr_pv(enum tmr_id id) {
    return tmr_array[id].pv;
}

inline bool tmr_output(enum tmr_id id) {
    return tmr_array[id].output;
}

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
