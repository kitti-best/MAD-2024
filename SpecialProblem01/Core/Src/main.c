/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
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

/* USER CODE BEGIN PV */
uint8_t buffer[100000];
uint8_t idx = 0;
uint8_t start_of_text = 0;
uint8_t is_ready[] = " is ready";

uint8_t start_fist = 0;
uint8_t my_turn = 0;

uint8_t count_r_data = 0;

bool have_name = 0;
uint8_t name_idx = 0;
char c;
char username[256];
char prompt[260] = "Name: ";
char exit_message[] = "Chat terminated...";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
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

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

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
  MX_USART3_UART_Init();

  uint8_t man_from[] = "Man from U.A.R.T.";
  uint8_t press_q[] = "Quit PRESS q\r\n";

  uint8_t name[] = "\tName: ";

  HAL_UART_Transmit(&huart3, (uint8_t*) man_from, strlen((char*) man_from), 100);
  if(start_fist){
	  HAL_UART_Transmit(&huart3, "1\r\n", 3, 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*) press_q, strlen((char*) press_q), 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*) name, strlen((char*) name), 100);
  } else {
	  HAL_UART_Transmit(&huart3, "2\r\n", 3, 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*) press_q, strlen((char*) press_q), 100);
	  HAL_UART_Transmit(&huart3, "\t", 1, 100);

  }

  HAL_UART_Receive_IT(&huart2, &c, 1);  HAL_UART_Receive_IT(&huart3, &c,1);
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* Transmit banner message */
	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	if(c == 'q' || c == 'Q'){
//		HAL_UART_Transmit(&huart3, "\r\n", 2, 100);
//		HAL_UART_Transmit(&huart2, "q", 1, 100);
//		HAL_UART_Transmit(&huart3, exit_message, strlen(exit_message), 100);
//		exit(0);
//	}
	if (huart->Instance == USART3 && my_turn){
		if(!have_name && c != '\r' && c != '\b'){
			username[name_idx++] = c;
		}
		if(c == '\r'){
			if(buffer[idx - 1] == 'q' && (idx - start_of_text) == 1){
        		HAL_UART_Transmit(&huart3, "\r\n", 2, 100);
        		HAL_UART_Transmit(&huart2, "q\r", 2, 100);
        		HAL_UART_Transmit(&huart3, exit_message, strlen(exit_message), 100);
        		exit(0);
			}
			buffer[idx] = '\0';
			HAL_UART_Transmit(&huart2, (uint8_t*)username, strlen(username), 100);
			if(!have_name){
				have_name = 1;

				memset(prompt, '\0', sizeof(prompt));
				strcat(prompt, username);
				strcat(prompt, " => ");

			    HAL_UART_Transmit(&huart2, (uint8_t*)is_ready, strlen(is_ready), 100);
			} else {
				HAL_UART_Transmit(&huart2, ": ", 2, 100);
				HAL_UART_Transmit(&huart2, buffer + start_of_text, idx - start_of_text, 100);
			}

			start_of_text = idx + 1;
			idx++;
			HAL_UART_Transmit(&huart2, "\r", 1, 100);
			HAL_UART_Transmit(&huart3, "\r\n\t", 3, 100);
			my_turn = 0;
		} else if (c == '\b') {
			if( idx > start_of_text){
				if(!have_name){
					username[name_idx] = '\0';
					name_idx--;
				}

				buffer[idx] = '\0';
				idx--;
				HAL_UART_Transmit(&huart3, "\b ", 2, 100);
				if( idx == start_of_text){ HAL_UART_Transmit(&huart3, "\b", 1, 100); }
			}
		} else {
			 buffer[idx++] = c;
		}
		if(idx > start_of_text) HAL_UART_Transmit(&huart3, (uint8_t*) &c, 1, 100);
	}

    if (huart->Instance == USART2 && !my_turn){
        if(c == '\r'){
            if((buffer[idx - 1] == 'q') && ((idx - 1) == start_of_text)){
                HAL_UART_Transmit(&huart3, "\r\n", 2, 100);
                HAL_UART_Transmit(&huart3, exit_message, strlen(exit_message), 100);
                exit(0);
            }
            buffer[idx] = '\0';
//          HAL_UART_Transmit(&huart3, buffer + start_of_text, strlen((char *)(buffer + start_of_text)), 100);
            HAL_UART_Transmit(&huart3, buffer + start_of_text, idx - start_of_text, 100);
//          HAL_UART_Transmit(&huart3, "HI", 2, 100);
            start_of_text = idx + 1;
            idx++;
            my_turn = 1;
            HAL_UART_Transmit(&huart3, "\r\n\t", 3, 100);
            HAL_UART_Transmit(&huart3, prompt, strlen(prompt), 100);
        } else {
            buffer[idx++] = c;
        }
    }

	HAL_UART_Receive_IT(&huart2, &c, 1);
	HAL_UART_Receive_IT(&huart3, &c, 1);
}

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

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
