#ifndef PMODE_INL_H
#define PMODE_INL_H
#include "pmode.h"
#include "stm32f4xx_ll_gpio.h"

inline void pmode_request(enum pmode_requested new_mode) {
    pmode.req_mode = new_mode;
}

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
