/*
 * udec_cx.h
 *
 *  Created on: Sep 9, 2026
 *      Author: abertazza
 */
#ifndef INC_UDEC_CX_H_
#define INC_UDEC_CX_H_
#include "can.h"

union udec_cx_inputs {
	struct
	{
		bool s3:1;
		bool s3a:1; /* ritorno cabina */
		bool s3c:1; /* trap */
        bool s4:1;  /* tray */
        bool fbk_byp_bot:1;
        bool fbk_byp_top:1;
		bool top_uzone:1;
		bool bot_uzone:1;
	};
	uint32_t w;
};

union udec_cx_outputs {
	struct
	{
        bool en_byp_bot:1;
        bool en_byp_top:1;
		bool elt:1;
	};
	uint32_t w;
};

struct udec_cx_info {
    union udec_cx_inputs in;
    union udec_cx_outputs out, old_out;
    bool online : 1;
    bool in_good : 1;
};

extern struct udec_cx_info udec_cx;

void udec_cx_process_beat(void);
void udec_cx_process_dout(const struct can_frame *frame);
void udec_cx_send_outputs(void);
void udec_cx_scan_cycle(void);

#endif /* INC_UDEC_CX_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
