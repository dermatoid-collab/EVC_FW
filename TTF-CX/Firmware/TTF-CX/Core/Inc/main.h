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
#define S3A_Pin GPIO_PIN_14
#define S3A_GPIO_Port GPIOC
#define BYP_BOT_EN_Pin GPIO_PIN_15
#define BYP_BOT_EN_GPIO_Port GPIOC
#define S4_Pin GPIO_PIN_0
#define S4_GPIO_Port GPIOC
#define BYP_BOT_FB_Pin GPIO_PIN_1
#define BYP_BOT_FB_GPIO_Port GPIOC
#define S3_Pin GPIO_PIN_2
#define S3_GPIO_Port GPIOC
#define VLIGHT_EN_Pin GPIO_PIN_3
#define VLIGHT_EN_GPIO_Port GPIOC
#define SPIF_NCS2_Pin GPIO_PIN_0
#define SPIF_NCS2_GPIO_Port GPIOA
#define LOAD_DOUT_Pin GPIO_PIN_1
#define LOAD_DOUT_GPIO_Port GPIOA
#define BYP_TOP_FB_Pin GPIO_PIN_2
#define BYP_TOP_FB_GPIO_Port GPIOA
#define NEOPIX_TX_Pin GPIO_PIN_3
#define NEOPIX_TX_GPIO_Port GPIOA
#define LOAD_CLK_Pin GPIO_PIN_4
#define LOAD_CLK_GPIO_Port GPIOA
#define VLIGHT_TRIM_Pin GPIO_PIN_5
#define VLIGHT_TRIM_GPIO_Port GPIOA
#define LPWM1_OUT_Pin GPIO_PIN_6
#define LPWM1_OUT_GPIO_Port GPIOA
#define LPWM2_OUT_Pin GPIO_PIN_7
#define LPWM2_OUT_GPIO_Port GPIOA
#define LOAD_NPWDN_Pin GPIO_PIN_4
#define LOAD_NPWDN_GPIO_Port GPIOC
#define TOP_UZONE_Pin GPIO_PIN_5
#define TOP_UZONE_GPIO_Port GPIOC
#define LPWM3_OUT_Pin GPIO_PIN_0
#define LPWM3_OUT_GPIO_Port GPIOB
#define LPWM4_OUT_Pin GPIO_PIN_1
#define LPWM4_OUT_GPIO_Port GPIOB
#define SPIF_SCLK_Pin GPIO_PIN_2
#define SPIF_SCLK_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_10
#define SCL_GPIO_Port GPIOB
#define BOT_UZONE_Pin GPIO_PIN_12
#define BOT_UZONE_GPIO_Port GPIOB
#define BYP_TOP_EN_Pin GPIO_PIN_13
#define BYP_TOP_EN_GPIO_Port GPIOB
#define S3C_Pin GPIO_PIN_14
#define S3C_GPIO_Port GPIOB
#define DEBUG_Pin GPIO_PIN_13
#define DEBUG_GPIO_Port GPIOC
#define SPIF_SIO0_Pin GPIO_PIN_9
#define SPIF_SIO0_GPIO_Port GPIOC
#define UART_TX_Pin GPIO_PIN_9
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin GPIO_PIN_10
#define UART_RX_GPIO_Port GPIOA
#define CAN_RX_Pin GPIO_PIN_11
#define CAN_RX_GPIO_Port GPIOA
#define CAN_TX_Pin GPIO_PIN_12
#define CAN_TX_GPIO_Port GPIOA
#define SPI_NSS_Pin GPIO_PIN_15
#define SPI_NSS_GPIO_Port GPIOA
#define SPIF_SIO1_Pin GPIO_PIN_10
#define SPIF_SIO1_GPIO_Port GPIOC
#define SDA_Pin GPIO_PIN_12
#define SDA_GPIO_Port GPIOC
#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_MISO_Pin GPIO_PIN_4
#define SPI_MISO_GPIO_Port GPIOB
#define SPI_MOSI_Pin GPIO_PIN_5
#define SPI_MOSI_GPIO_Port GPIOB
#define SPIF_NCS_Pin GPIO_PIN_6
#define SPIF_NCS_GPIO_Port GPIOB
#define LPWM5_OUT_Pin GPIO_PIN_7
#define LPWM5_OUT_GPIO_Port GPIOB
#define EL_PWM_OUT_Pin GPIO_PIN_8
#define EL_PWM_OUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern TIM_HandleTypeDef htim4;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* Local Variables: */
/* tab-width: 4 */
/* End: */
