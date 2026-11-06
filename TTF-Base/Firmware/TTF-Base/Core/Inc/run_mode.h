#ifndef RUN_MODE_H
#define RUN_MODE_H
#include <stdbool.h>

enum e_machine_state {
    MACHINE_STATE_INIT_SEQ,
	MACHINE_STATE_NORMAL,
    MACHINE_STATE_FALL_TEST,
    MACHINE_STATE_DOWN,
    MACHINE_STATE_OPEN_DOOR,
    MACHINE_STATE_CLOSE_DOOR,
    MACHINE_STATE_UP,
    MACHINE_STATE_STUCK_UP,
    MACHINE_STATE_WAIT_FOR_CALL_RELEASE,
    MACHINE_STATE_MAX
};

enum car_call {
	PREN_NONE,
	PREN_P0,
    PREN_P1
};

struct run_mode_info
{
    enum car_call call;
    enum e_machine_state state;
    bool bypass_manutenzione : 1;
    bool enable : 1;
    bool wait_for_enable : 1;
    bool busy : 1;
    bool overload : 1;
    bool reset_request : 1;
};
extern struct run_mode_info run_mode;
void run_mode_scan_cycle(void);
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
