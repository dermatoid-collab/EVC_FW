/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "battery.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_bb.h"
#include "HMI.h"
#include "timer.h"
#include "pmode.h"
#include "application.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

IWDG_HandleTypeDef hiwdg;

QSPI_HandleTypeDef hqspi;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t timestamp;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM11_Init(void);
static void MX_UART5_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
static void MX_TIM6_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    __enable_irq();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0); // Forza la priorità massima al tick

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  MX_TIM1_Init();
  MX_TIM11_Init();
  MX_UART5_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_TIM6_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  HAL_TIM_Base_Start(&htim6);

  hmi_init_bb();

  /* Filtri CAN: servono i PDO dal VFD e la mailbox del caricabatterie */
  /* Per poter usare entrambe le fifo bisogna usare due filter bank distinti */
  /* Bank 0/1: risposte da caricabatteria, 00C01xx */
  CAN_FilterTypeDef filt_cfg = {
      .FilterBank = 0,
      .FilterIdHigh = 0x0000,
      .FilterIdLow = 0x0000,
      .FilterMaskIdHigh = 0x0000,
      .FilterMaskIdLow = 0x0000,
      .FilterFIFOAssignment = CAN_FILTER_FIFO0,
      .FilterMode = CAN_FILTERMODE_IDMASK,
      .FilterScale = CAN_FILTERSCALE_32BIT,
      .FilterActivation = CAN_FILTER_ENABLE,
      .SlaveStartFilterBank = 13,
  };
  HAL_CAN_ConfigFilter(&hcan1, &filt_cfg);
  filt_cfg.FilterBank = 1,
  filt_cfg.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  HAL_CAN_ConfigFilter(&hcan1, &filt_cfg);

  /* Bank 2/3: PDO e beat da VFD: 181, 281, 381, 701 */
  filt_cfg.FilterScale = CAN_FILTERSCALE_16BIT;
  filt_cfg.FilterBank = 2;
  filt_cfg.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  HAL_CAN_ConfigFilter(&hcan1, &filt_cfg);
  filt_cfg.FilterBank = 3;
  filt_cfg.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  HAL_CAN_ConfigFilter(&hcan1, &filt_cfg);

  /* le prime 13 bank sono per CAN1, le altre per CAN2*/
  /* bank 14 per CAN2 UDEC C/CX e L2D
   * NOTA: quando si configura CAN2 bisogna ri-specificare
   * lo slave filter bank */
  filt_cfg.SlaveStartFilterBank = 13;

  filt_cfg.FilterScale = CAN_FILTERSCALE_32BIT;
  filt_cfg.FilterBank = 14;
  filt_cfg.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  HAL_CAN_ConfigFilter(&hcan2, &filt_cfg);

  filt_cfg.FilterScale = CAN_FILTERSCALE_32BIT;
  filt_cfg.FilterBank = 15;
  filt_cfg.FilterFIFOAssignment = CAN_FILTER_FIFO1;
  HAL_CAN_ConfigFilter(&hcan2, &filt_cfg);

  tmr_config(TMR_ID_VFD_OFFLINE, TMR_TYPE_IEC_TON, 200);
  tmr_config(TMR_ID_VFD_MASTER_HB, TMR_TYPE_IEC_TON, 50);

  tmr_config(TMR_ID_UDEC_C_OFFLINE, TMR_TYPE_IEC_TON, 1500);
  tmr_config(TMR_ID_UDEC_CX_OFFLINE, TMR_TYPE_IEC_TON, 500);
  tmr_config(TMR_ID_NPB_SCAN, TMR_TYPE_D1, 200);
  tmr_config(TMR_ID_PMODE_TO_CHARGER, TMR_TYPE_IEC_TON, TIMEOUT_PMODE_TO_CHARGER);
  tmr_config(TMR_ID_PMODE_TO_DRIVE, TMR_TYPE_IEC_TON, TIMEOUT_PMODE_TO_DRIVE);
  tmr_config(TMR_ID_PMODE_WAIT_TO_CHARGER, TMR_TYPE_IEC_TON, TIMEOUT_PMODE_WAIT_TO_CHARGER);
  tmr_config(TMR_ID_MACH_PARK_TIME, TMR_TYPE_IEC_TON, TIMEOUT_MACH_PARK_TIME);
  tmr_config(TMR_ID_MACH_DOOR_TIME, TMR_TYPE_IEC_TON, TIMEOUT_MACH_DOOR_TIME);

  tmr_config(TMR_ID_HMI_RESET_TIMEOUT, TMR_TYPE_IEC_TON, TIMEOUT_HMI_RESET);
  tmr_config(TMR_ID_COP_RESET_TIMEOUT, TMR_TYPE_IEC_TON, TIMEOUT_COP_RESET);
  tmr_config(TMR_ID_OCC_TIMEOUT, TMR_TYPE_IEC_TOF, TIMEOUT_OCC);
  tmr_config(TMR_ID_LOCK_DELAY_OP, TMR_TYPE_IEC_TON, TIMEOUT_LOCK_DELAY_OP);
  tmr_config(TMR_ID_LOCK_DELAY_CL, TMR_TYPE_IEC_TON, TIMEOUT_LOCK_DELAY_CL);
  tmr_config(TMR_ID_VFD_RESET_DELAY, TMR_TYPE_IEC_TON, TIMEOUT_VFD_ON_RESTART);
  tmr_config(TMR_ID_NMT_PERIOD, TMR_TYPE_D1, TIMEOUT_NMT_PERIOD);
  tmr_config(TMR_ID_BUTTONS_LED, TMR_TYPE_IEC_TON, 500);
  tmr_config(TMR_ID_UNLOCK_TIMEOUT, TMR_TYPE_IEC_TON, 5000);
  tmr_config(TMR_ID_LOCK_TIMEOUT, TMR_TYPE_IEC_TON, 5000);

  /* tempo intervento rele */
  tmr_config(TMR_ID_OTR_REL, TMR_TYPE_IEC_TON, TIMEOUT_RELAY_SWITCH);
  tmr_config(TMR_ID_BRK_REL, TMR_TYPE_IEC_TON, TIMEOUT_RELAY_SWITCH);
  tmr_config(TMR_ID_FBK_BOT_REL, TMR_TYPE_IEC_TON, TIMEOUT_MIRROR_SWITCH);
  tmr_config(TMR_ID_FBK_TOP_REL, TMR_TYPE_IEC_TON, TIMEOUT_MIRROR_SWITCH);
  tmr_config(TMR_ID_VFD_OUTPUT_SWITCH, TMR_TYPE_IEC_TON, TIMEOUT_RELAY_SWITCH);
  tmr_config(TMR_ID_VFD_OUTPUT_DELAY, TMR_TYPE_IEC_TON, TIMEOUT_VFD_OUTPUT_DELAY);
  tmr_config(TMR_ID_NPB_CURRENT_REG, TMR_TYPE_D1, TIMEOUT_NPB_CURRENT_REG);
  /* Il timeout arresto motore è pari al 150% del tempo massimo
   * comandato. Attenzione che il VFD ha unità di 100 ms mentre qui si
   * lavora con 1 ms. Viene calcolato dinamicamente quando viene
   * calcolato lo stop, questo valore non è significativo. */
  tmr_config(TMR_ID_STOP_TIMEOUT, TMR_TYPE_IEC_TON, 200);

  tmr_config(TMR_ID_L2D_0_OFFLINE, TMR_TYPE_IEC_TON, 2000);
  tmr_config(TMR_ID_L2D_8_OFFLINE, TMR_TYPE_IEC_TON, 2000);

  /* tempo sincronizzazione messaggi can */
  tmr_config(TMR_ID_ERR_SYNC, TMR_TYPE_IEC_TON, 1000);
  tmr_config(TMR_ID_DOORUNLOCK_DELAY, TMR_TYPE_IEC_TON, 1500);
  tmr_config(TMR_ID_TRIP_DISPLAY, TMR_TYPE_D1, TIMEOUT_TRIP_DISPLAY);
  tmr_config(TMR_ID_BRAKE_RELEASE, TMR_TYPE_IEC_TON, TIMEOUT_BRAKE_RELEASE);
  tmr_config(TMR_ID_DOOR_APPROACH, TMR_TYPE_IEC_TON, TIMEOUT_DOOR_APPROACH);

  tmr_config(TMR_ID_L2D_0_OFFLINE, TMR_TYPE_IEC_TON, 2000);
  tmr_config(TMR_ID_L2D_8_OFFLINE, TMR_TYPE_IEC_TON, 2000);

  HAL_CAN_Start(&hcan1);
  HAL_CAN_Start(&hcan2);

  /* se leggo a polling il CAN non devo usare questo altrimenti mi svuota i buffer */
//  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//    __enable_irq();
  while (1)
  {
      HAL_IWDG_Refresh(&hiwdg);
      main_loop();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 270;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 20;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = ENABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = ENABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 8;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 20;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 10;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 89;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 0;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */
  HAL_TIM_MspPostInit(&htim11);

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_9B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_EVEN;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, USB_5VEN_Pin|OUT1_1_Pin|OUT2_1_Pin|OUT3_1_Pin
                          |OUT4_1_Pin|RS485_DE_Pin|NPB_EN_Pin|OTR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_BAT_Pin|CAN_HS2_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_CHG_Pin|LED_DIS_Pin|LED_DEBUG_Pin|BRK_EN_Pin
                          |CAN_HS1_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, BT_EN_Pin|VFD_PWR_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCL_Pin|SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : USB_5VEN_Pin OUT1_1_Pin OUT2_1_Pin OUT3_1_Pin
                           OUT4_1_Pin RS485_DE_Pin NPB_EN_Pin OTR_EN_Pin */
  GPIO_InitStruct.Pin = USB_5VEN_Pin|OUT1_1_Pin|OUT2_1_Pin|OUT3_1_Pin
                          |OUT4_1_Pin|RS485_DE_Pin|NPB_EN_Pin|OTR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : EXP_00_Pin EXP_01_Pin EXP_02_Pin EXP_03_Pin */
  GPIO_InitStruct.Pin = EXP_00_Pin|EXP_01_Pin|EXP_02_Pin|EXP_03_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : BT_STATE_Pin */
  GPIO_InitStruct.Pin = BT_STATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BT_STATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_BAT_Pin CAN_HS2_EN_Pin */
  GPIO_InitStruct.Pin = LED_BAT_Pin|CAN_HS2_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_CHG_Pin LED_DIS_Pin LED_DEBUG_Pin BRK_EN_Pin
                           CAN_HS1_EN_Pin */
  GPIO_InitStruct.Pin = LED_CHG_Pin|LED_DIS_Pin|LED_DEBUG_Pin|BRK_EN_Pin
                          |CAN_HS1_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : IN1_1_Pin IN1_2_Pin IN1_3_Pin IN1_4_Pin */
  GPIO_InitStruct.Pin = IN1_1_Pin|IN1_2_Pin|IN1_3_Pin|IN1_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : NPB_DET_Pin */
  GPIO_InitStruct.Pin = NPB_DET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(NPB_DET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTR_FB_Pin */
  GPIO_InitStruct.Pin = OTR_FB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(OTR_FB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BYP_S14_Pin S1_Pin S2_Pin S3_Pin
                           S4_Pin S8_Pin */
  GPIO_InitStruct.Pin = BYP_S14_Pin|S1_Pin|S2_Pin|S3_Pin
                          |S4_Pin|S8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : BT_EN_Pin VFD_PWR_EN_Pin */
  GPIO_InitStruct.Pin = BT_EN_Pin|VFD_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : BRK_FB_Pin */
  GPIO_InitStruct.Pin = BRK_FB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BRK_FB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SCL_Pin SDA_Pin */
  GPIO_InitStruct.Pin = SCL_Pin|SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
