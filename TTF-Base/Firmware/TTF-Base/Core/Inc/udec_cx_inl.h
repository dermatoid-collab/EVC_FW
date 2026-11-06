/*
 * udec_cx_inl.h
 *
 *  Created on: Sep 10, 2026
 *      Author: abertazza
 */

#ifndef INC_UDEC_CX_INL_H_
#define INC_UDEC_CX_INL_H_

#include "udec_cx.h"
#include "run_mode.h"

inline void door_unlock(void) {
    udec_cx.out.elt = true;
}

inline void door_lock(void) {
    udec_cx.out.elt = false;
}

inline bool bot_unlock_zone(void) {
    return udec_cx.in_good && udec_cx.in.bot_uzone;
}

inline bool top_unlock_zone(void) {
    return udec_cx.in_good && udec_cx.in.top_uzone;
}

#endif /* INC_UDEC_CX_INL_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
