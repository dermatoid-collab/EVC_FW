#ifndef VFD_H
#define VFD_H

#include "timer.h"
#include "can.h"
#include "pmode_inl.h"

/* Driver brushless Italsea 7BL */

/** ID CAN VFD (per configurazione nodo 1) */
enum {
    VFD_CAN_ID_TPDO1 = 0x181,
    VFD_CAN_ID_RPDO1 = 0x201,
    VFD_CAN_ID_TPDO2 = 0x281,
    VFD_CAN_ID_TPDO3 = 0x381,
    VFD_CAN_ID_BEAT  = 0x701,
    VFD_CAN_ID_TSDO  = 0x601,
    VFD_CAN_ID_RSDO  = 0x581,
};
/** Command words (RPDO1, byte 7) */
enum {
    VFD_COMMAND_FAULT_RESET = 0x01,
    VFD_COMMAND_DISABLE = 0x02,
    VFD_COMMAND_OPEN_BRAKE = 0x04,
    VFD_COMMAND_RESET_POS = 0x40,
    VFD_COMMAND_REBOOT = 0x80,
};

/** Motor status (TPDO1, byte 0) */
enum vfd_run_dir {
    VFD_MOTOR_RUN = 0x01,
    VFD_MOTOR_FWD = 0x02,
    VFD_MOTOR_REV = 0x04,
};

enum {
    /** Attesa prima della riapertura normale relè di uscita */
    TIMEOUT_VFD_OUTPUT_DELAY = 10000,
};

/** Stato macchina commutazione relè di uscita */
enum vfd_output_state {
    VFD_OUTPUT_OPEN,
    VFD_OUTPUT_CLOSING,
    VFD_OUTPUT_CLOSED,
    VFD_OUTPUT_OPENING
};

/** Richiesta di movimento al relè di uscita */
enum vfd_output_req {
    VFD_OUTPUT_OPEN_REQ,
    VFD_OUTPUT_CLOSE_REQ,
    VFD_OUTPUT_EMERGENCY_REQ
};

/** Fault VFD (allarmi), (RPDO2, byte 7) */
enum vfd_fault {
    VFD_FAULT_NO = 0,
    VFD_FAULT_FWBW_SWITCH_ON = 1,
    VFD_FAULT_HALL_FAIL = 2,
    VFD_FAULT_POT_FAIL = 3,
    VFD_FAULT_REF_OUT_NEUTRAL = 4,
    VFD_FAULT_BOARD_OVERTEMP = 5,
    VFD_FAULT_POWER_STAGE = 6,
    VFD_FAULT_OVERCURRENT = 7,
    VFD_FAULT_POWER_FUSE_RELAY = 8,
    VFD_FAULT_UNDERVOLTAGE = 9,
    VFD_FAULT_OVERVOLTAGE = 10,
    VFD_FAULT_OVERLOAD = 11,
    VFD_FAULT_DISABLE_ON = 12,
    VFD_FAULT_KEY_OFF = 13,
    VFD_FAULT_EEPROM = 14,
    VFD_FAULT_OVERSPEED = 15,
    VFD_FAULT_COMM_TIMEOUT = 16,
    VFD_FAULT_MOTOR_OVERTEMP = 17,
    VFD_FAULT_PHASE_LACK = 18,
};
/** Numero di beat da ricevere dopo il reset per considerarlo concluso */
enum {
    VFD_RESET_BEATS = 10
};
/* Direzioni per il movimento */
enum vfd_run_req {
    VFD_RUN_FWD = 0,
    VFD_RUN_REV = 1,
    VFD_RUN_STOP = 2
};

/** RPM per velocità lineare.
 * Con motore da 10 cm/s: Velocità in RPM motore per le varie velocità lineari, dagli RPM
 * motore si divide per 31 per avere i cm/s; il riduttore comunque è 1:27
 *
 * NUOVO RIDUTTORE 1:25.3, per avere cm/s si divide per 45 */
enum {
    /** Velocità speciale: il minimo azionabile per poter aprire il
     * freno stando sostanzialmente fermi */
    RPM_BRAKE_RELEASE = 11,
    ACC_BRAKE_RELEASE = 1,
    RPM_SPEED_2 = 91,
    RPM_SPEED_3 = 136,
    RPM_SPEED_5 = 227,
    RPM_SPEED_8 = 364,
    RPM_SPEED_10 = 455,
    RPM_SPEED_15 = 683
};

enum {
    /** Tempo minimo del timeout di arresto */
    VFD_STOP_TIME_MIN = 500,
    /** Tempo supplementare per comunicazione dal VFD */
    VFD_STOP_TIME_ADD = 200,
};

/** Stato generale del VFD */
struct vfd_request {
    uint16_t speed;
    enum vfd_run_req direction;
    uint8_t time_acc;
    uint8_t time_dec;
    uint8_t cmds;
};
struct vfd_info {
    bool online;
    uint8_t motor_status;
    int8_t motor_voltage;
    int8_t motor_current;
    uint8_t input_voltage;
    uint8_t overload;
    uint8_t heatsink_temperature;
    int16_t motor_speed;
    int16_t motor_position;
    enum vfd_fault fault;
    /* Stato del contattore di uscita, funzionalmente fa parte del
     * VFD */
    enum vfd_output_state output_state;
    enum vfd_output_req output_req;
    bool output_coil;           /* Comando effettivo del relè */
    /* Informazioni sullo stato richiesto; l'invio viene posticipato
     * alla prima occasione in cui è possibile l'invio */
    unsigned restarting;            /* Contatore di beat dopo il riavvio */
    bool request_pending;       /* Si applica solo al movimento */
    struct vfd_request current_req;
    struct vfd_request last_req;
};
extern struct vfd_info vfd;

void vfd_process_can_tpdo1(const struct can_frame *frame);
void vfd_process_can_tpdo2(const struct can_frame *frame);
void vfd_process_can_tpdo3(const struct can_frame *frame);
void vfd_process_can_beat(const struct can_frame *frame);
void vfd_scan_cycle(void);
void vfd_move_request(uint16_t speed,
        enum vfd_run_req direction, uint8_t time_acc, uint8_t time_dec);
void vfd_cmd_request(uint8_t cmd);
void vfd_reset(void);
void vfd_reboot(void);
bool vfd_param_setup(void);
bool vfd_is_ready(void);
#endif
/* Local Variables: */
/* tab-width: 4 */
/* End: */
