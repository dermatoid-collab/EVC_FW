/*
 * application.c
 *
 *  Created on: Jul 10, 2026
 *      Author: abertazza
 */

#include "application.h"


extern CAN_HandleTypeDef hcan1;
extern UART_HandleTypeDef huart6;
extern QSPI_HandleTypeDef hqspi;

uint8_t FSM_test_hw;
bool rcv_cplt, tx_cplt;
uint8_t recv_data[8];

bool byp_top, byp_bot, cmd_byp_bot, cmd_byp_top;
uint8_t DInDebounceCnt[N_INPUTS] = {0};

union udec_cx_inputs_t udec_cx_inputs, udec_cx_inputs_old;
union udec_cx_outputs_t udec_cx_outputs, udec_cx_outputs_old;

bool debug_pin;
bool udec_ttf_online;


void inputs_scan(void)
{
	enum {
		 DInDebounce = 20
	};
    uint8_t DInRaw[N_INPUTS];

    DInRaw[0] = HAL_GPIO_ReadPin(SAFE_S3_GPIO_Port, SAFE_S3_Pin);
    DInRaw[1] = HAL_GPIO_ReadPin(SAFE_S3A_GPIO_Port, SAFE_S3A_Pin);
    DInRaw[2] = HAL_GPIO_ReadPin(SAFE_S3B_GPIO_Port, SAFE_S3B_Pin);
    DInRaw[3] = HAL_GPIO_ReadPin(SAFE_S3C_GPIO_Port, SAFE_S3C_Pin);
    DInRaw[4] = HAL_GPIO_ReadPin(SAFE_S4_GPIO_Port, SAFE_S4_Pin);
    DInRaw[5] = HAL_GPIO_ReadPin(BYP_TOP_FB_GPIO_Port, BYP_TOP_FB_Pin);
    DInRaw[6] = HAL_GPIO_ReadPin(BYP_BOT_FB_GPIO_Port, BYP_BOT_FB_Pin);
    DInRaw[7] = HAL_GPIO_ReadPin(VFD_I6_GPIO_Port, VFD_I6_Pin);
    DInRaw[8] = HAL_GPIO_ReadPin(VFD_I7_GPIO_Port, VFD_I7_Pin);

    /* Filtro per 5 cicli */
    for (int i = 0; i < N_INPUTS; i++)
    {
        // Estraggo il bit i-esimo dallo stato precedentemente validato
        uint8_t InputOld = ((udec_cx_inputs.w) >> i) & 0x01;

        if (DInRaw[i] != InputOld)
        {
        	if (DInDebounceCnt[i] < 0xFF)
        	{
        		DInDebounceCnt[i]++;
        	}

            if (DInDebounceCnt[i] > DInDebounce)
            {
                uint8_t AndMask = ~(0x1 << i);
                uint8_t OrMask = DInRaw[i] << i;

                udec_cx_inputs.w &= AndMask; // Azzero il bit i-esimo vecchio
                udec_cx_inputs.w |= OrMask;  // Scrivo il nuovo valore filtrato

                DInDebounceCnt[i] = 0;
            }
        }
        else
        {
            DInDebounceCnt[i] = 0; // Segnale stabile, resetto il contatore
        }
    }
}

static void led_blink()
{
	static unsigned timestamp;

	/* DEBUG */
	if (HAL_GetTick() - timestamp >= 250)
	{
		HAL_GPIO_TogglePin(DEBUG_GPIO_Port, DEBUG_Pin);
		timestamp = HAL_GetTick();
	}
}

void process_outputs(const struct can_frame *frame)
{
	udec_cx_outputs.w = can_extract_uint32(frame->data);
}

/* Dispatch frame CAN, indirizzi standard */
static void process_can_rx_std(const struct can_frame *frame)
{
	if ((frame->id == UDEC_TYPE_M)
			&& (frame->data[1] == UDEC_TYPE_M
	    			&& frame->data[0] != NMT_STOPPED))
	{
		/* can alive cx */
	    udec_ttf_online = true;
	    tmr_reset(TMR_ID_NMT_TIMEOUT);
	}

}

static void process_can_rx_ext(const struct can_frame *frame)
{
    /* Extended ID (battery charger e UDEC) */
    switch (frame->id) {
    case UDEC_CAN_ID(UDEC_TYPE_CX, 0, UDEC_ID_DOUT):
    	process_outputs(frame);
    	break;
    default:
        break;
    }
}

static void process_can_rx(void)
{
	struct can_frame frame;

	while(can_rx_buffer_pop(&frame))
        if (frame.IDE == CAN_ID_STD) {
            process_can_rx_std(&frame);
        } else {
            process_can_rx_ext(&frame);
        }
}

/* inviati ad ogni cambio o scadere del timer */
void can_send_io(void)
{
	uint8_t payload[8];

	if ((udec_cx_inputs.w != udec_cx_inputs_old.w)
			|| (udec_cx_outputs.w != udec_cx_outputs_old.w)
			|| tmr_update(TMR_ID_UDEC_CX_UPDATE, true))
	{
		memset(payload, 0, sizeof(payload));
		memcpy(&payload[0], &udec_cx_inputs, sizeof(udec_cx_inputs));
		memcpy(&payload[4], &udec_cx_outputs, sizeof(udec_cx_outputs));

		udec_cx_inputs_old.w = udec_cx_inputs.w;
		udec_cx_outputs_old.w = udec_cx_outputs.w;

		can_tx_push_fullframe_ext(UDEC_CAN_ID(UDEC_TYPE_CX, 0, UDEC_ID_STATUS_WORD), payload);
		tmr_reset(TMR_ID_UDEC_CX_UPDATE);
	}
}

void outputs_scan(void)
{
	/* valori percentuali, da calcolare Hz effettivi */
	enum {
		ELT_100 = 0xFFFF,
		ELT_30 = 0x4CC2,
		ELT_0 = 0
	};

	/* gestione bypass */
	if (udec_ttf_online)
	{
		HAL_GPIO_WritePin(BYP_BOT_EN_GPIO_Port, BYP_BOT_EN_Pin, udec_cx_outputs.en_byp_bot);
		HAL_GPIO_WritePin(BYP_TOP_EN_GPIO_Port, BYP_TOP_EN_Pin, udec_cx_outputs.en_byp_top);
	}
	else
	{
		HAL_GPIO_WritePin(BYP_BOT_EN_GPIO_Port, BYP_BOT_EN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BYP_TOP_EN_GPIO_Port, BYP_TOP_EN_Pin, GPIO_PIN_RESET);
	}

	/* gestione elettroserratura */
	if (udec_cx_outputs.elt && udec_ttf_online)
	{
		if (tmr_update(TMR_ID_ELT_PWM, true))
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, ELT_30);
		}
		else
		{
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, ELT_100);
		}
	}
	else
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, ELT_0);
}


void send_nmt(void)
{
	uint16_t nmt_msg = 0;

	if (tmr_update(TMR_ID_NMT_DELAY, true))
	{
		nmt_msg = (uint16_t)(NMT_OPERATIONAL << 8);
		nmt_msg |= UDEC_TYPE_CX;
		can_tx_push_halfword_std(ID_NMT, nmt_msg);
		tmr_reset(TMR_ID_NMT_DELAY);
	}
}

void nmt_scan(void)
{
	send_nmt();

	if (tmr_update(TMR_ID_NMT_TIMEOUT, true))
		udec_ttf_online = false;
}

void main_loop(void)
{
    can_pull_hw_fifo(&hcan1);
    process_can_rx();

    nmt_scan();
    inputs_scan();
    outputs_scan();

    can_send_io();
    can_push_hw_fifo(&hcan1);

    led_blink();
	tmr_end_scan();
}
