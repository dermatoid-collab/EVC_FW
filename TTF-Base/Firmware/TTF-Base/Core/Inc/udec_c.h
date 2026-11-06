#ifndef UDEC_C_H
#define UDEC_C_H
#include "can.h"

/* Status word, usata anche come beat; nel frame ci sono anche altre info */
union udec_c_status_word {
    struct {
        bool start : 1;               /* Sempre a uno! */
        bool v24_total : 1;
        bool v24_elt_prot : 1;
        bool v24_gen_prot : 1;
        bool can_ewg : 1;
        bool can_epv : 1;
        bool can_bof : 1;
        bool can_bit_error : 1;
        bool can_stuff : 1;
        bool can_form : 1;
        bool can_ack : 1;
        bool can_rx_fifo_overrun : 1;
        bool can_rx_buffer_overrun : 1;
        bool can_tx_buffer_overrun : 1;
    };
    uint32_t w;
};

/* Control word (usata praticamente mai…)*/
union udec_c_control_word {
    struct {
        bool reset_24v_errors : 1;
        bool reset_can_errors : 1;
        bool reset_elt_errors : 1;
    };
    uint32_t w;
};

union udec_c_inputs {
    struct
    {
        /* Ingressi digitali */
        bool ftc1:1;
        bool ftc2:1;
        bool ftc3:1;
        bool sa_chi:1;
        bool sb_pc0:1;
        bool sb_pc1:1;
        bool sb_pc2:1;
        bool sb_pc3:1;
        bool sb_pc4:1;
        bool sb_pc5:1;
        bool sb_pc6:1;
        bool sb_pc7:1;
        bool sb_pc8:1;
        bool sb_pc9:1;
        bool sb_pc10:1;
        bool sb_pc11:1;
        bool sb_pc12:1;
        bool sb_pc13:1;
        bool sq_tca:1;
        bool sq_fta12:1;
        /* Monitor linee di sicurezza */
        bool s31_par:1;
        bool s32_pec:1;
        bool s33_pet:1;
        bool s34_col:1;
        bool s35_tc:1;
        bool s36_ft:1;
        bool s51_apc1:1;
        bool s52_apc2:1;
        bool s53_apc3:1;
    };
    uint32_t w;
};

union udec_c_outputs {
    struct {
        bool hl_pc0:1;
        bool hl_pc1:1;
        bool hl_pc2:1;
        bool hl_pc3:1;
        bool hl_pc4:1;
        bool hl_pc5:1;          /* Nonché EV-CAB-L */
        bool hl_pc6:1;
        bool hl_pc7:1;
        bool hl_pc8:1;
        bool hl_pc9:1;
        bool hl_pc10:1;
        bool hl_pc11:1;
        bool hl_pc12:1;         /* Nonché +24V-FTCx */
        bool hl_pc13:1;         /* Nonché ELT-CMD */
        bool elt_24v:1;
        bool op1_ap:1;
        bool op1_ch:1;
        bool op2_ap:1;
        bool op2_ch:1;
        bool op3_ap:1;
        bool op3_ch:1;
        bool hl_ovl:1;
        bool hl_emg:1;          /* Nonché +24V-OP */
        bool hl_occ:1;
        bool hl_buz_ft:1;
        bool hl_buz:1;          /* Buzzer su scheda */
    };
    uint32_t w;
};

/** Interfaccia a UDEC.C */
struct udec_c_info {
    uint32_t can_stats;
    union udec_c_status_word status_word;
    union udec_c_control_word control_word;
    union udec_c_inputs in;
    union udec_c_outputs out;
    union udec_c_outputs old_out;
    bool online;
    bool in_good;
    bool send_outputs;
};
extern struct udec_c_info udec_c;

void udec_c_process_status_word(const struct can_frame *frame);
void udec_c_process_inputs(const struct can_frame *frame);
void udec_c_process_can_stats(const struct can_frame *frame);
void udec_c_scan_cycle(void);
void udec_c_send_outputs(void);

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
