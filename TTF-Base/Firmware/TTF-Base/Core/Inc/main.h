/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_ll_dma.h"
//#include "stm32f4xx_ll_i2c.h" // fino al fix hardware
//#include "stm32f4xx_ll_iwdg.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_gpio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

//extern DAC_HandleTypeDef hdac;

//extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_i2c1_rx;

//extern IWDG_HandleTypeDef hiwdg;

extern QSPI_HandleTypeDef hqspi;

//extern RTC_HandleTypeDef hrtc;

//extern SD_HandleTypeDef hsd;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim11;
//extern TIM_HandleTypeDef htim6;

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart2;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USB_5VEN_Pin GPIO_PIN_2
#define USB_5VEN_GPIO_Port GPIOE
#define OUT1_1_Pin GPIO_PIN_3
#define OUT1_1_GPIO_Port GPIOE
#define OUT2_1_Pin GPIO_PIN_4
#define OUT2_1_GPIO_Port GPIOE
#define OUT3_1_Pin GPIO_PIN_5
#define OUT3_1_GPIO_Port GPIOE
#define OUT4_1_Pin GPIO_PIN_6
#define OUT4_1_GPIO_Port GPIOE
#define EXP_00_Pin GPIO_PIN_0
#define EXP_00_GPIO_Port GPIOC
#define EXP_01_Pin GPIO_PIN_1
#define EXP_01_GPIO_Port GPIOC
#define EXP_02_Pin GPIO_PIN_2
#define EXP_02_GPIO_Port GPIOC
#define EXP_03_Pin GPIO_PIN_3
#define EXP_03_GPIO_Port GPIOC
#define SIN_A_Pin GPIO_PIN_0
#define SIN_A_GPIO_Port GPIOA
#define COS_A_Pin GPIO_PIN_1
#define COS_A_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_2
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_3
#define BT_RX_GPIO_Port GPIOA
#define REF1V25_Pin GPIO_PIN_5
#define REF1V25_GPIO_Port GPIOA
#define BT_STATE_Pin GPIO_PIN_6
#define BT_STATE_GPIO_Port GPIOA
#define LED_BAT_Pin GPIO_PIN_7
#define LED_BAT_GPIO_Port GPIOA
#define LED_CHG_Pin GPIO_PIN_4
#define LED_CHG_GPIO_Port GPIOC
#define LED_DIS_Pin GPIO_PIN_5
#define LED_DIS_GPIO_Port GPIOC
#define SPIF_SCLK_Pin GPIO_PIN_2
#define SPIF_SCLK_GPIO_Port GPIOB
#define RS485_RO_Pin GPIO_PIN_7
#define RS485_RO_GPIO_Port GPIOE
#define RS485_DI_Pin GPIO_PIN_8
#define RS485_DI_GPIO_Port GPIOE
#define RS485_DE_Pin GPIO_PIN_10
#define RS485_DE_GPIO_Port GPIOE
#define IN1_1_Pin GPIO_PIN_12
#define IN1_1_GPIO_Port GPIOE
#define IN1_2_Pin GPIO_PIN_13
#define IN1_2_GPIO_Port GPIOE
#define IN1_3_Pin GPIO_PIN_14
#define IN1_3_GPIO_Port GPIOE
#define IN1_4_Pin GPIO_PIN_15
#define IN1_4_GPIO_Port GPIOE
#define CAN_RX_LS_Pin GPIO_PIN_12
#define CAN_RX_LS_GPIO_Port GPIOB
#define CAN_TX_HS_Pin GPIO_PIN_13
#define CAN_TX_HS_GPIO_Port GPIOB
#define NPB_DET_Pin GPIO_PIN_14
#define NPB_DET_GPIO_Port GPIOB
#define OTR_FB_Pin GPIO_PIN_15
#define OTR_FB_GPIO_Port GPIOB
#define BYP_S14_Pin GPIO_PIN_8
#define BYP_S14_GPIO_Port GPIOD
#define BT_EN_Pin GPIO_PIN_9
#define BT_EN_GPIO_Port GPIOD
#define VFD_PWR_EN_Pin GPIO_PIN_10
#define VFD_PWR_EN_GPIO_Port GPIOD
#define SPIF_SIO0_Pin GPIO_PIN_11
#define SPIF_SIO0_GPIO_Port GPIOD
#define SPIF_SIO1_Pin GPIO_PIN_12
#define SPIF_SIO1_GPIO_Port GPIOD
#define BRK_FB_Pin GPIO_PIN_6
#define BRK_FB_GPIO_Port GPIOC
#define LED_DEBUG_Pin GPIO_PIN_7
#define LED_DEBUG_GPIO_Port GPIOC
#define BRK_EN_Pin GPIO_PIN_9
#define BRK_EN_GPIO_Port GPIOC
#define CAN_HS2_EN_Pin GPIO_PIN_15
#define CAN_HS2_EN_GPIO_Port GPIOA
#define CAN_HS1_EN_Pin GPIO_PIN_10
#define CAN_HS1_EN_GPIO_Port GPIOC
#define CAN_RX_HS_Pin GPIO_PIN_0
#define CAN_RX_HS_GPIO_Port GPIOD
#define CAN_TX_HSD1_Pin GPIO_PIN_1
#define CAN_TX_HSD1_GPIO_Port GPIOD
#define S1_Pin GPIO_PIN_3
#define S1_GPIO_Port GPIOD
#define S2_Pin GPIO_PIN_4
#define S2_GPIO_Port GPIOD
#define S3_Pin GPIO_PIN_5
#define S3_GPIO_Port GPIOD
#define S4_Pin GPIO_PIN_6
#define S4_GPIO_Port GPIOD
#define S8_Pin GPIO_PIN_7
#define S8_GPIO_Port GPIOD
#define SPIF_CS_Pin GPIO_PIN_6
#define SPIF_CS_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_7
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_8
#define SDA_GPIO_Port GPIOB
#define NPB_EN_Pin GPIO_PIN_0
#define NPB_EN_GPIO_Port GPIOE
#define OTR_EN_Pin GPIO_PIN_1
#define OTR_EN_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
