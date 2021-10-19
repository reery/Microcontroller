/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAXUSERNAMELENGTH 20
#define MAXPASSWORDLENGTH 20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
uint8_t buffer[100];
uint8_t smallBuffer[1];
uint8_t username[MAXUSERNAMELENGTH + 1];
uint8_t correctUsername[MAXUSERNAMELENGTH + 1] = "Slavko";
uint8_t correctPassword[MAXPASSWORDLENGTH + 1] = "empathy";
uint8_t password[MAXPASSWORDLENGTH + 1];
int i = 0;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
void printSerial(char *);
void inputUsername(uint8_t*, int);
void inputPassword(uint8_t*, int);
int checkAccess(uint8_t *, uint8_t *, uint8_t *, uint8_t *);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//uint8_t buffer[100];
//uint8_t smallBuffer[1];
//uint8_t username[21];
//uint8_t correctUsername[21] = "Slavko";
//uint8_t correctPassword[21] = "empathy";
//uint8_t password[20];
//int i = 0;
UART_HandleTypeDef * huartAddress = &huart2;

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  printSerial("\r\nUsername: ");
	  inputUsername(username, 20);

	  printSerial("\r\nPassword: ");
	  inputPassword(password, 20);

	  if(checkAccess(username, password, correctUsername, correctPassword) == 0) {
		  printSerial("\r\nSTM32$ ");
	  }

	  printSerial("\r\nYour username is: ");
	  HAL_UART_Transmit(&huart2, username, strlen((char*)username), HAL_MAX_DELAY);

	  printSerial("\r\nYour password is: ");
	  HAL_UART_Transmit(&huart2, password, strlen((char*)password), HAL_MAX_DELAY);


	  //HAL_Delay(1000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void printSerial(char * stringToPrint) {
	  strcpy((char*)buffer, stringToPrint);
	  HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), 100);
}

void inputUsername(uint8_t * username, int usernameMaxLength) {
	  for(i = 0; i <= usernameMaxLength; i++) {
		  HAL_UART_Receive(&huart2, smallBuffer, sizeof(smallBuffer), HAL_MAX_DELAY);
		  HAL_UART_Transmit(&huart2, smallBuffer, sizeof(smallBuffer), HAL_MAX_DELAY);

		  if(i == 0 && smallBuffer[0] == '\r') {
			  username[0] = '\0';
			  smallBuffer[0] = '0';
			  printSerial("\r\nUsername: ");
			  i = -1;
			  continue;
		  }

		  if(i > 0 && smallBuffer[0] == '\r') {
			  username[i] = '\0';
			  smallBuffer[0] = '0';
			  return;
		  }

		  if(i < usernameMaxLength ) { // add check if it is alphanum and no special characters to write. Then also check what happens when you use special chars
			  username[i] = smallBuffer[0];
		  } else if(i == usernameMaxLength) {
			  printSerial("\r\nUsername too long. Max 20 characters.\r\n");
			  i = -1;
			  username[0] = '\0';
			  printSerial("\r\nUsername: ");
			  smallBuffer[0] = '0';
			  continue;
		  }
	  }
}

void inputPassword(uint8_t * password, int passwordMaxLength) {
	  for(i = 0; i <= passwordMaxLength; i++) {
		  HAL_UART_Receive(&huart2, smallBuffer, sizeof(smallBuffer), HAL_MAX_DELAY);

		  if(i == 0 && smallBuffer[0] == '\r') {
			  password[0] = '\0';
			  smallBuffer[0] = '0';
			  printSerial("\r\nPassword: ");
			  i = -1;
			  continue;
		  }

		  if(i > 0 && smallBuffer[0] == '\r') {
			  password[i] = '\0';
			  smallBuffer[0] = '0';
			  return;
		  }

		  strcpy((char*)buffer, "*");
		  HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), 100);

		  if(i < passwordMaxLength) {
			  password[i] = smallBuffer[0];
		  } else if(i == passwordMaxLength){
			  printSerial("\r\nPassword too long. Max 20 characters.\r\n");
			  i = -1;
			  password[0] = '\0';
			  printSerial("\r\nPassword: ");
			  smallBuffer[0] = '0';
			  continue;
		  }
	  }
}

int checkAccess(uint8_t * username, uint8_t * password, uint8_t * correctUsername, uint8_t * correctPassword) {
	  if(strcmp((const char*)username, (const char*)correctUsername) == 0 && strcmp((const char*)password, (const char*)correctPassword) == 0) {
		  printSerial("\r\nHello ");
		  printSerial((char*)username);
		  printSerial(".");
		  return 0;
	  } else {
		  printSerial("\r\nWrong username or password. Please try again.");
		  return 1;
	  }
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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
