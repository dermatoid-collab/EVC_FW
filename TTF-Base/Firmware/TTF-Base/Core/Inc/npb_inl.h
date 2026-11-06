#ifndef NPB_INL_H
#define NPB_INL_H
#include "npb.h"
/* Predicati e accessori */
/* I bit di fault non sono solo nel registro di status */
inline bool npb_in_fault(void) {
    return (npb.fault != 0) && (npb.fault != NPB_FLT_OP_OFF);
}
inline uint16_t npb_chg_status(void) { return npb.chg_status; }
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
