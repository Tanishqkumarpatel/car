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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */
void Forward(void);
void Back(void);
void Right(void);
void Left(void);
void Stop(void);
void Roll(void);
float Get_Distance(void);
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim4);
  HAL_GPIO_WritePin(A0_STBY_GPIO_Port, A0_STBY_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  float distance = Get_Distance();

	  if (distance >=0 && distance <= 15.0) {
		  Stop();
		  HAL_Delay(200);

		  Back();
		  HAL_Delay(400);

		  Right();
		  HAL_Delay(500);

		  Stop();
		  HAL_Delay(200);
	  } else if (distance > 15.0 && distance <= 20.0) {
		  Roll();
	  } else {
		  Forward();
	  }
//	  Small Delay to prevent overlapping of sound waves.
	  HAL_Delay(50);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 83;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, A5_TRIG_Pin|A4_BIN2_LM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A0_STBY_Pin|A1_AIN1_RM_Pin|A2_AIN2_RM_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : A5_TRIG_Pin A4_BIN2_LM_Pin */
  GPIO_InitStruct.Pin = A5_TRIG_Pin|A4_BIN2_LM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : A0_STBY_Pin A1_AIN1_RM_Pin A2_AIN2_RM_Pin LD2_Pin */
  GPIO_InitStruct.Pin = A0_STBY_Pin|A1_AIN1_RM_Pin|A2_AIN2_RM_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : A3_BIN1_LM_Pin */
  GPIO_InitStruct.Pin = A3_BIN1_LM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(A3_BIN1_LM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : D10_ECHO_Pin */
  GPIO_InitStruct.Pin = D10_ECHO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(D10_ECHO_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Forward(void) {
//	Right Motor Forward
	HAL_GPIO_WritePin(A1_AIN1_RM_GPIO_Port, A1_AIN1_RM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A2_AIN2_RM_GPIO_Port, A2_AIN2_RM_Pin, GPIO_PIN_RESET);

//	Left Motor Forward
	HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A4_BIN2_LM_GPIO_Port, A4_BIN2_LM_Pin, GPIO_PIN_RESET);
}

void Back(void) {
//	Right Motor Reverse
	HAL_GPIO_WritePin(A1_AIN1_RM_GPIO_Port, A1_AIN1_RM_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A2_AIN2_RM_GPIO_Port, A2_AIN2_RM_Pin, GPIO_PIN_SET);

//	Left Motor Reverse
	HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A4_BIN2_LM_GPIO_Port, A4_BIN2_LM_Pin, GPIO_PIN_SET);
}

void Left(void) {
//	Right Motor Forward
	HAL_GPIO_WritePin(A1_AIN1_RM_GPIO_Port, A1_AIN1_RM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A2_AIN2_RM_GPIO_Port, A2_AIN2_RM_Pin, GPIO_PIN_RESET);

//	Left Motor Reverse
	HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A4_BIN2_LM_GPIO_Port, A4_BIN2_LM_Pin, GPIO_PIN_SET);

}

void Right(void) {
//	Right Motor Reverse
	HAL_GPIO_WritePin(A1_AIN1_RM_GPIO_Port, A1_AIN1_RM_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A2_AIN2_RM_GPIO_Port, A2_AIN2_RM_Pin, GPIO_PIN_SET);

//	Left Motor Forward
	HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A4_BIN2_LM_GPIO_Port, A4_BIN2_LM_Pin, GPIO_PIN_RESET);
}

void Stop(void) {
//	Right Motor High
	HAL_GPIO_WritePin(A1_AIN1_RM_GPIO_Port, A1_AIN1_RM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A2_AIN2_RM_GPIO_Port, A2_AIN2_RM_Pin, GPIO_PIN_SET);

//	Left Motor High
	HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A4_BIN2_LM_GPIO_Port, A4_BIN2_LM_Pin, GPIO_PIN_SET);
}

void Roll(void) {
//	Right Motor Low
	HAL_GPIO_WritePin(A1_AIN1_RM_GPIO_Port, A1_AIN1_RM_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A2_AIN2_RM_GPIO_Port, A2_AIN2_RM_Pin, GPIO_PIN_RESET);

//	Left Motor Low
	HAL_GPIO_WritePin(A3_BIN1_LM_GPIO_Port, A3_BIN1_LM_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A4_BIN2_LM_GPIO_Port, A4_BIN2_LM_Pin, GPIO_PIN_RESET);
}

float Get_Distance(void) {
	uint32_t local_time = 0;

//	Starting the sensor
	HAL_GPIO_WritePin(A5_TRIG_GPIO_Port, A5_TRIG_Pin, GPIO_PIN_SET);

//	Waiting for timer to hit 10 micro-seconds.
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	while (__HAL_TIM_GET_COUNTER(&htim4) < 10);

	HAL_GPIO_WritePin(A5_TRIG_GPIO_Port, A5_TRIG_Pin, GPIO_PIN_RESET);

//	Starting timer.
	__HAL_TIM_SET_COUNTER(&htim4, 0);

//	Checking if the Echo is high if not then graceful exit.
	while (HAL_GPIO_ReadPin(D10_ECHO_GPIO_Port, D10_ECHO_Pin) == GPIO_PIN_RESET)
	{
//		ECHO is low for 30ms. Sound waves did not fire.
		if (__HAL_TIM_GET_COUNTER(&htim4) > 30000) return -1.0;
	}

//	Reseting the timer to 0 as soon as the Sound waves are fire (ECHO is High).
	__HAL_TIM_SET_COUNTER(&htim4, 0);

//	Letting the timer runs till sound waves bounces back and is received
	while (HAL_GPIO_ReadPin(D10_ECHO_GPIO_Port, D10_ECHO_Pin) == GPIO_PIN_SET)
	{
		// Safety timeout (Did not receive any sound waves): Object is too far away (out of range)
		if (__HAL_TIM_GET_COUNTER(&htim4) > 30000) return -1.0;
	}

//	Gets the time it took sound waves to bounce back to receiver
	local_time = __HAL_TIM_GET_COUNTER(&htim4);

	return (local_time * 0.0343) / 2.0;
}
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
