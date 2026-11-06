#ifndef NPB_H
#define NPB_H
#include <stdint.h>
#include <stdbool.h>
/* Caricabatteria Meanwell serie NPB */
enum {
    NPB_CAN_REQ_ID = 0x00C0103,
    NPB_CAN_RSP_ID = 0x00C0003
};
/* Bit del registro FAULT_STATUS */
enum {
    NPB_FLT_NO      = 0x00,
    NPB_FLT_OTP     = 0x02,
    NPB_FLT_OPV     = 0x04,
    NPB_FLT_OLP     = 0x08,
    NPB_FLT_SHORT   = 0x10,
    NPB_FLT_AC_FAIL = 0x20,
    NPB_FLT_OP_OFF  = 0x40,
    NPB_FLT_HI_TEMP = 0x80,
};

/* Bit del registro CHG_STATUS */
enum {
    NPB_CHG_STATUS_FULL        = 0x0001,
    NPB_CHG_STATUS_CCM         = 0x0002,
    NPB_CHG_STATUS_CVM         = 0x0004,
    NPB_CHG_STATUS_FVM         = 0x0008,
    NPB_CHG_STATUS_WAKEUP_STOP = 0x0040,
    NPB_CHG_STATUS_NTCER       = 0x0400,
    NPB_CHG_STATUS_BTNC        = 0x0800,
    NPB_CHG_STATUS_CCTOF       = 0x2000,
    NPB_CHG_STATUS_CVTOF       = 0x4000,
    NPB_CHG_STATUS_FVTOF       = 0x8000,
};

struct npb_cmd {
    uint16_t set_voltage;
    uint16_t set_current;
    bool poll_pending;
    uint8_t poll_index;
};

struct npb_info {
    bool online;
    uint16_t out_voltage;
    uint16_t out_current;
    uint16_t set_voltage;
    uint16_t set_current;
    uint8_t fault;
    uint16_t chg_status;
    bool poll_pending;
    uint8_t poll_index;
    struct npb_cmd cmd;
    struct npb_cmd cmd_old;
};
extern struct npb_info npb;


void npb_process_can_rx(const struct can_frame *frame);
void npb_scan_cycle(void);
void npb_set_psumode(void);
void npb_disable_eeprom(void);

#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
