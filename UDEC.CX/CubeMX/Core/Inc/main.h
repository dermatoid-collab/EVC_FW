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
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "application.h"
#include "timer.h"
#include "timer_inl.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define SPIF_SIO2_Pin GPIO_PIN_2
#define SPIF_SIO2_GPIO_Port GPIOE
#define LOAD_CLK_Pin GPIO_PIN_3
#define LOAD_CLK_GPIO_Port GPIOE
#define LOAD_DOUT_Pin GPIO_PIN_4
#define LOAD_DOUT_GPIO_Port GPIOE
#define LOAD_NPDWN_Pin GPIO_PIN_5
#define LOAD_NPDWN_GPIO_Port GPIOE
#define BYP_TOP_FB_Pin GPIO_PIN_6
#define BYP_TOP_FB_GPIO_Port GPIOE
#define SAFE_S3C_Pin GPIO_PIN_13
#define SAFE_S3C_GPIO_Port GPIOC
#define SAFE_S4_Pin GPIO_PIN_14
#define SAFE_S4_GPIO_Port GPIOC
#define BYP_BOT_FB_Pin GPIO_PIN_15
#define BYP_BOT_FB_GPIO_Port GPIOC
#define OSCI1_Pin GPIO_PIN_0
#define OSCI1_GPIO_Port GPIOH
#define OSCO1_Pin GPIO_PIN_1
#define OSCO1_GPIO_Port GPIOH
#define MOT_PTC_Pin GPIO_PIN_0
#define MOT_PTC_GPIO_Port GPIOC
#define SPI_MOSI_Pin GPIO_PIN_1
#define SPI_MOSI_GPIO_Port GPIOC
#define SPI_MISO_Pin GPIO_PIN_2
#define SPI_MISO_GPIO_Port GPIOC
#define SAFE_S3A_Pin GPIO_PIN_3
#define SAFE_S3A_GPIO_Port GPIOC
#define ONEWIRE_IN0_Pin GPIO_PIN_0
#define ONEWIRE_IN0_GPIO_Port GPIOA
#define SPIF_SIO3_Pin GPIO_PIN_1
#define SPIF_SIO3_GPIO_Port GPIOA
#define ONEWIRE_IN1_Pin GPIO_PIN_2
#define ONEWIRE_IN1_GPIO_Port GPIOA
#define NEOPIX_TX_Pin GPIO_PIN_3
#define NEOPIX_TX_GPIO_Port GPIOA
#define AUDIO_OUT_Pin GPIO_PIN_4
#define AUDIO_OUT_GPIO_Port GPIOA
#define VLIGHT_TRIM_Pin GPIO_PIN_5
#define VLIGHT_TRIM_GPIO_Port GPIOA
#define ISNS_U_Pin GPIO_PIN_6
#define ISNS_U_GPIO_Port GPIOA
#define ISNS_V_Pin GPIO_PIN_7
#define ISNS_V_GPIO_Port GPIOA
#define BYP_BOT_EN_Pin GPIO_PIN_4
#define BYP_BOT_EN_GPIO_Port GPIOC
#define BYP_TOP_EN_Pin GPIO_PIN_5
#define BYP_TOP_EN_GPIO_Port GPIOC
#define LPWM3_OUT_Pin GPIO_PIN_0
#define LPWM3_OUT_GPIO_Port GPIOB
#define LPWM4_OUT_Pin GPIO_PIN_1
#define LPWM4_OUT_GPIO_Port GPIOB
#define SPIF_CLK_Pin GPIO_PIN_2
#define SPIF_CLK_GPIO_Port GPIOB
#define VFD_I1_Pin GPIO_PIN_7
#define VFD_I1_GPIO_Port GPIOE
#define VFD_I2_Pin GPIO_PIN_8
#define VFD_I2_GPIO_Port GPIOE
#define VFD_I3_Pin GPIO_PIN_9
#define VFD_I3_GPIO_Port GPIOE
#define VFD_I4_Pin GPIO_PIN_10
#define VFD_I4_GPIO_Port GPIOE
#define VFD_I5_Pin GPIO_PIN_11
#define VFD_I5_GPIO_Port GPIOE
#define VFD_I6_Pin GPIO_PIN_12
#define VFD_I6_GPIO_Port GPIOE
#define VFD_I7_Pin GPIO_PIN_13
#define VFD_I7_GPIO_Port GPIOE
#define VFD_STO_Pin GPIO_PIN_14
#define VFD_STO_GPIO_Port GPIOE
#define VFD_RUN_Pin GPIO_PIN_15
#define VFD_RUN_GPIO_Port GPIOE
#define SPI_SCK_Pin GPIO_PIN_10
#define SPI_SCK_GPIO_Port GPIOB
#define VFD_Y2_Pin GPIO_PIN_12
#define VFD_Y2_GPIO_Port GPIOB
#define BRK_EN_Pin GPIO_PIN_13
#define BRK_EN_GPIO_Port GPIOB
#define BRK_EN_FB_Pin GPIO_PIN_14
#define BRK_EN_FB_GPIO_Port GPIOB
#define BRK_FB2_Pin GPIO_PIN_15
#define BRK_FB2_GPIO_Port GPIOB
#define BRK_FB1_Pin GPIO_PIN_8
#define BRK_FB1_GPIO_Port GPIOD
#define OTR_FB_Pin GPIO_PIN_9
#define OTR_FB_GPIO_Port GPIOD
#define OTR_EN_Pin GPIO_PIN_10
#define OTR_EN_GPIO_Port GPIOD
#define SPIF_SIO0_Pin GPIO_PIN_11
#define SPIF_SIO0_GPIO_Port GPIOD
#define SPIF_SIO1_Pin GPIO_PIN_12
#define SPIF_SIO1_GPIO_Port GPIOD
#define LPWM5_OUT_Pin GPIO_PIN_13
#define LPWM5_OUT_GPIO_Port GPIOD
#define LPWM6_OUT_Pin GPIO_PIN_14
#define LPWM6_OUT_GPIO_Port GPIOD
#define VLIGHT_EN_Pin GPIO_PIN_15
#define VLIGHT_EN_GPIO_Port GPIOD
#define MODBUS_TX_Pin GPIO_PIN_6
#define MODBUS_TX_GPIO_Port GPIOC
#define MODBUS_RX_Pin GPIO_PIN_7
#define MODBUS_RX_GPIO_Port GPIOC
#define SDIO_D0_Pin GPIO_PIN_8
#define SDIO_D0_GPIO_Port GPIOC
#define SDIO_D1_Pin GPIO_PIN_9
#define SDIO_D1_GPIO_Port GPIOC
#define DEBUG_Pin GPIO_PIN_8
#define DEBUG_GPIO_Port GPIOA
#define UART_TX_Pin GPIO_PIN_9
#define UART_TX_GPIO_Port GPIOA
#define CAN_RX_Pin GPIO_PIN_11
#define CAN_RX_GPIO_Port GPIOA
#define CAN_TX_Pin GPIO_PIN_12
#define CAN_TX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define ONEWIRE_LED_Pin GPIO_PIN_15
#define ONEWIRE_LED_GPIO_Port GPIOA
#define SDIO_D2_Pin GPIO_PIN_10
#define SDIO_D2_GPIO_Port GPIOC
#define SDIO_D3_Pin GPIO_PIN_11
#define SDIO_D3_GPIO_Port GPIOC
#define SDIO_CK_Pin GPIO_PIN_12
#define SDIO_CK_GPIO_Port GPIOC
#define MODBUS_DE_Pin GPIO_PIN_0
#define MODBUS_DE_GPIO_Port GPIOD
#define SDIO_CD_Pin GPIO_PIN_1
#define SDIO_CD_GPIO_Port GPIOD
#define SDIO_CMD_Pin GPIO_PIN_2
#define SDIO_CMD_GPIO_Port GPIOD
#define SDIO_PWR_Pin GPIO_PIN_3
#define SDIO_PWR_GPIO_Port GPIOD
#define LIGHT1_EN_Pin GPIO_PIN_4
#define LIGHT1_EN_GPIO_Port GPIOD
#define LIGHT2_EN_Pin GPIO_PIN_5
#define LIGHT2_EN_GPIO_Port GPIOD
#define SAFE_S3B_Pin GPIO_PIN_6
#define SAFE_S3B_GPIO_Port GPIOD
#define SAFE_S3_Pin GPIO_PIN_7
#define SAFE_S3_GPIO_Port GPIOD
#define ONEWIRE_OUT_Pin GPIO_PIN_3
#define ONEWIRE_OUT_GPIO_Port GPIOB
#define LPWM1_OUT_Pin GPIO_PIN_4
#define LPWM1_OUT_GPIO_Port GPIOB
#define LPWM2_OUT_Pin GPIO_PIN_5
#define LPWM2_OUT_GPIO_Port GPIOB
#define SPIF_NCS_Pin GPIO_PIN_6
#define SPIF_NCS_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOB
#define SPI_NSS_Pin GPIO_PIN_9
#define SPI_NSS_GPIO_Port GPIOB
#define SPIF_NCS2_Pin GPIO_PIN_0
#define SPIF_NCS2_GPIO_Port GPIOE
#define AUDIO_EN_Pin GPIO_PIN_1
#define AUDIO_EN_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
extern TIM_HandleTypeDef htim4;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
