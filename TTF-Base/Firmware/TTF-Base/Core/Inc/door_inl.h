#ifndef DOOR_INL_H
#define DOOR_INL_H
#include "door.h"
extern bool open_door_request;
inline void set_open_door_request(void) {
    open_door_request = true;
}
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
