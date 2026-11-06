/*
 * l2d.h
 *
 *  Created on: May 27, 2026
 *      Author: abertazza
 */

#ifndef INC_L2D_H_
#define INC_L2D_H_
/* Sulla macchina c'è solo una porta ma a doppia anta; quindi bisogna
 * indirizzare l'operatore 0 e l'operatore 8 */
enum {
    N_OPERATORI_MAX = 9
};

extern bool l2d_online[N_OPERATORI_MAX];

typedef enum
{
    L2D_NoReq,
    L2D_OpReq,
    L2D_ClReq,
    L2D_LrnReq,
    L2D_RebootReq
} L2D_CmdReq_t;

typedef enum
{
    L2D_MsgFreq = 0x2000,
    L2D_AccRamp,
    L2D_DecRamp,
    L2D_ObstCurrOp,
    L2D_ObstCurrCl,
    L2D_OpenSpeed,
    L2D_CloseSpeed,
    L2D_dtForceCl,
    L2D_DoorSide,
    L2D_DoorLightSet,
    L2D_dsDecOp = 0x200B,
    L2D_dsDecCl,
    L2D_PWMClose,
    L2D_DoorType,
    L2D_CmdReq = 0x20FE,
    L2D_FwVer
} L2D_ParamAddr_t;

typedef union
{
    struct
    {
        L2D_ParamAddr_t address;
        L2D_CmdReq_t command;
    };
    uint32_t w;
} L2D_CmdPayload_t;

typedef union
{
    struct
    {
        L2D_ParamAddr_t address;
        uint16_t command;
    };
    uint32_t w;
} L2D_Payload_t;

typedef enum
{
    L2D_SelfLearn = 0x00,
    L2D_Init = 0x01,
    L2D_StopCl,
    L2D_StopOp,
    L2D_StopMid,
    L2D_MovOp,
    L2D_MovCl,
    L2D_Obstacle,
    L2D_StopErr,
} L2D_StatusCode_t;

typedef enum
{
    L2D_Temp = 1,
    L2D_Low24,
    L2D_Encod,
    L2D_DoorLightErr
} L2D_ErrorCode_t;

typedef struct
{
    L2D_StatusCode_t status;
    L2D_ErrorCode_t errCode;
    unsigned voltage;
    unsigned nOpen;
    unsigned temp;
} L2D_StatusWord_t;

void l2d_process_status_word(const struct can_frame *frame);
void l2d_process_read_resp(const struct can_frame *frame);
void l2d_process_write_resp(const struct can_frame *frame);
void l2d_scan(void);
void l2d_set_neutral(void);
void l2d_set_open(void);
void l2d_set_close(void);
bool l2d_param_setup(void);


#endif /* INC_L2D_H_ */
/* Local Variables: */
/* tab-width: 4 */
/* End: */
