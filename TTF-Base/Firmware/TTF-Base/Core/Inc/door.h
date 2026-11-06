#ifndef DOOR_H
#define DOOR_H
#include <stdbool.h>
/** Stati di funzionamento del controllo porte */
enum door_status {
    DOOR_STOPPED,
    DOOR_LOCKED,
    DOOR_CLOSING,
    DOOR_LOCKING,
    DOOR_UNLOCKING,
    DOOR_OPENING,
};

/** Comandi al controllo porte */
enum door_cmd {
    DOOR_CMD_NONE,
    DOOR_CMD_CLOSE,
    DOOR_CMD_OPEN,
    DOOR_CMD_RESET
};

extern bool open_door_request;
void door_loop(enum door_cmd cmd);
void door_reset();

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
