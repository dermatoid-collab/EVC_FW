/*
 * application.h
 *
 *  Created on: Jul 10, 2026
 *      Author: abertazza
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

#include "main.h"
#include "can.h"
#include "timer_inl.h"

#define N_INPUTS 9

union udec_cx_inputs_t {
	struct
	{
		bool s3:1;
		bool s3a:1;
		bool s3b:1;
		bool s3c:1;
		bool s4:1;
		bool fbk_byp_top:1;
		bool fbk_byp_bot:1;
		bool top_uzone:1;
		bool bot_uzone:1;
	};
	uint32_t w;
};

union udec_cx_outputs_t {
	struct
	{
		bool en_byp_top:1;
		bool en_byp_bot:1;
		bool elt:1;
	};
	uint32_t w;
};


void main_loop(void);


#endif /* INC_APPLICATION_H_ */
