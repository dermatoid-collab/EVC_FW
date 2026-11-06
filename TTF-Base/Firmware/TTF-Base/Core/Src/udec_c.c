#include "timer.h"
#include "can_inl.h"
#include "udec_c_inl.h"

struct udec_c_info udec_c;

/* Inlines */
bool magnet_ftc(void);
bool door_is_closed(void);
void door_set_close(void);
void door_set_open(void);
void door_set_neutral(void);
bool is_P0_on(void);
bool is_P1_on(void);

void overload_lamp(bool s);
void reopen_lamp(bool s);
void enable_tlc(void);
void disable_tlc(void);
void enable_p71_out(void);
void disable_p71_out(void);
bool upward_ok(void);
bool downward_ok(void);
bool is_car_ok(void);
bool overload_in(void);
void set_occ(bool s);
bool door_is_locked(void);
bool reopen_door_button(void);

/** Ricezione del beat */
void udec_c_process_beat(void)
{
    tmr_reset(TMR_ID_UDEC_C_OFFLINE);
    udec_c.online = true;
}

/** Status word di UDEC.C (non usata), contiene anche il feedback uscite */
void udec_c_process_status_word(const struct can_frame *frame)
{
    udec_c.status_word.w = can_extract_uint32(frame->data);
    /* Nei byte 4–8 ci sono i latch delle uscite. Lo sfruttiamo per confermare lo stato tracciato */
    union udec_c_outputs recv;
    recv.w = can_extract_uint32(frame->data+4);
    /* Corregge le uscite se necessario */
    if (udec_c.online &&
            (recv.w != udec_c.out.w) &&
            can_tx_push_word_ext(UDEC_CAN_ID(UDEC_TYPE_C, 0, UDEC_ID_DOUT),
                    udec_c.out.w, CAN125)) {
        udec_c.old_out.w = udec_c.out.w;
    }
    udec_c_process_beat();
}

/** Ingressi da UDEC.C */
void udec_c_process_inputs(const struct can_frame *frame)
{
    udec_c.in.w = can_extract_uint32(frame->data);
    udec_c_process_beat();
    udec_c.in_good = true;
}

/** Stato CAN da UDEC.C, in ESR (non usato) */
void udec_c_process_can_stats(const struct can_frame *frame)
{
    udec_c.can_stats = can_extract_uint32(frame->data);
}

/** Ciclo di scansione UDEC.C; controlla solo che non sia in timeout */
void udec_c_scan_cycle(void)
{
    if (tmr_update(TMR_ID_UDEC_C_OFFLINE, true)) {
        udec_c.online = false;
        udec_c.in_good = false;
    }
}

/* Aggiornamento uscite, sezione UDEC.C */
void udec_c_send_outputs(void)
{
    if (udec_c.online)
    {
        /* Control word: la invia se non è uguale a zero; una volta inviata viene azzerata */
        if (udec_c.control_word.w &&
                can_tx_push_word_ext(UDEC_CAN_ID(UDEC_TYPE_C, 0, UDEC_ID_CONTROL_WORD),
                        udec_c.control_word.w, CAN125)) {
            udec_c.control_word.w = 0;
        }
        /* Invia lo stato delle uscite aggiornato */
        if ((udec_c.old_out.w != udec_c.out.w) &&
                can_tx_push_word_ext(UDEC_CAN_ID(UDEC_TYPE_C, 0, UDEC_ID_DOUT),
                        udec_c.out.w, CAN125)) {
            udec_c.old_out.w = udec_c.out.w;
        }
    }
}
/* Local Variables: */
/* tab-width: 4 */
/* End: */
