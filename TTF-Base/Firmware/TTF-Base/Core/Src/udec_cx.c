/*
 * udec_cx.c
 *
 *  Created on: Sep 9, 2026
 *      Author: abertazza
 */

#include "timer.h"
#include "can_inl.h"
#include "udec_cx_inl.h"

struct udec_cx_info udec_cx;

/* Inlines */
void door_unlock(void);
void door_lock(void);
bool bot_unlock_zone(void);
bool top_unlock_zone(void);

/** Frame di beat ricevuto da UDEC.CX, riavvia il timeout */
void udec_cx_process_beat(void)
{
    udec_cx.online = true;
    tmr_reset(TMR_ID_UDEC_CX_OFFLINE);
}

/* FRAME
 * AA AA AA AA BB BB BB BB
 *    inputs	 outputs */
/** frame ricevuto da CX: stato di ingressi e uscite secondo CX */
void udec_cx_process_dout(const struct can_frame *frame)
{
    udec_cx.in.w = can_extract_uint32(frame->data);
    union udec_cx_outputs recv;
    recv.w = can_extract_uint32(frame->data+4);
    udec_cx.in_good = true;
    if (udec_cx.online
            && (recv.w != udec_cx.out.w)
            && can_tx_push_word_ext(UDEC_CAN_ID(UDEC_TYPE_CX, 0, UDEC_ID_DOUT),
                    udec_cx.out.w, CAN125))
    {
        udec_cx.old_out = udec_cx.out;
    }
}

/** Imposta comando uscite, se modificate */
void udec_cx_send_outputs(void)
{
    if (udec_cx.online
            && (udec_cx.old_out.w != udec_cx.out.w)
            && can_tx_push_word_ext(UDEC_CAN_ID(UDEC_TYPE_CX, 0, UDEC_ID_DOUT),
                    udec_cx.out.w, CAN125))
    {
        udec_cx.old_out.w = udec_cx.out.w;
    }
}

/** Iterazione di controllo UDEC.CX */
void udec_cx_scan_cycle(void)
{
    /* Verifica che il beat della CX non sia in ritardo */
    if (tmr_update(TMR_ID_UDEC_CX_OFFLINE, true))
    {
        udec_cx.online = false;
        udec_cx.in_good = false;
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
