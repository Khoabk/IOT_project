/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stdio.h"
#include "Scheduler.h"
#include "Error_handler.h"
#include "Task.h"
#include "i2c.h"
#include "lcd.h"
#include "Timer.h"
#include "dht11.h"
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
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */


//For UART operations
uint8_t temp = 0;
uint8_t command[30];
uint8_t index_command = 0;
uint8_t buffer_flag = 0;
//uint8_t req_check[] = {'#','r','e','q'};
uint8_t info[40];
uint8_t resp[20];
uint8_t req_state = 0;

uint8_t req_flag = 0;

uint8_t data[] ="Hello\r\n";

int s__n = 0;


void Request_parser(void)
{

	if(buffer_flag)
	{
		if(temp =='!')
			{
				index_command = (index_command == 0)? 28: index_command - 2;

				if(command[index_command] == '1' || command[index_command] == '0')
				{
					C_or_F = command[index_command] - '0';

					req_state = 0;
				}

				else req_state = 1;

				req_flag = 1;
			}

		buffer_flag = 0;
	}



}



void Send_info (void)
{
	int len;

	if(C_or_F)
	{
		len = sprintf((char*)info,"#info:%.1fC:%.1f%%!\r\n",temp_C,humidity);
	}
	else len = sprintf((char*)info,"#info:%.1fF:%.1f%%!\r\n",temp_F,humidity);

	HAL_UART_Transmit(&huart4,info,len,12);
}




void Send_resp (void)
{
	if (req_flag)
	{
		req_flag = 0;

		int len;

		if(req_state == 1)
		{
			len = sprintf((char*)resp, "#resp:ERROR!\r\n");
		}
		else len = sprintf((char*)resp,"#resp:OK!\r\n");

		HAL_UART_Transmit(&huart4,resp,len,12);

	}
}









//uint16_t period_time = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/*static void MX_TIM2_Init(void);
static void MX_TIM9_Init(void);
static void MX_UART4_Init(void);*/
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{

		SCH_Update();

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
	if(huart->Instance == UART4)
	{
		command[index_command++] = temp;

		index_command %= 30;

		HAL_UART_Receive_IT(&huart4,&temp, sizeof(temp));

		buffer_flag = 1;
	}
}


void I2C1_GPIO_CONFIG(void)
{
	   RCC->APB1ENR |= (1<<21); //Enable APB1 clock
	   RCC->AHB1ENR |= (1<<1); //Enable GPIOB clock
	   //Configure GPIOB on pin 7 (SDA) and pin 6 (SCL) for I2C operation
	   GPIOB->MODER |= (1<<15)|(1<<13);
	   GPIOB->MODER &= ~((1<<14)|(1<<12));
	   GPIOB->OTYPER |= (1<<7)|(1<<6);
	   GPIOB->OSPEEDR |= ((3<<14)|(3<<12));
	   GPIOB->PUPDR |= (1<<14)|(1<<12);
	   GPIOB->PUPDR &= ~(1<<15|1<<13);
	   GPIOB->AFR[0] |= (4<<28) | (4<<24);
}




void System_Initialization(void)
{
	 HAL_Init();
	 SystemClock_Config();
	 MX_GPIO_Init();
	 MX_UART4_Init();
	 I2C1_GPIO_CONFIG();
	 I2C_init();
	 lcd_init();
	 SCH_Init();
	 Data_line_pin_init();
	// MX_USART2_UART_Init();
//	 MX_IWDG_Init();
	// HAL_TIM_Base_Start_IT(&htim2);
	// MX_TIM2_Init();
}

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
  System_Initialization();

  //MX_TIM9_Init();

//  int offset = 5;


  SCH_Add_Task(Led_blinking1,300,1000);
  SCH_Add_Task(Send_info,1000,2000);
  SCH_Add_Task(Read_from_DHT11,0,2000);
  SCH_Add_Task(Display_Data_from_DHT11,500,2000);
  SCH_Add_Task(Send_resp,1500,2000);
  SCH_Add_Task(Request_parser,500,2000);
 // SCH_Add_Task(Request_parser,1000,4000);

 // SCH_Add_Task(Send_resp,1200,4000);

  //SCH_Add_Task(Display_Response,1,1500);
 // SCH_Add_Task(Display_Response,1000,1000);
 // SCH_Add_Task(Led_blinking2,1000,2000);
 // SCH_Add_Task(Display_number,2000,500);
//  SCH_Add_Task(Led_blinking3,1000,500);


  //Read_from_DHT11();

  //DHT11_Start();

  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start(&htim9);


  //Read_from_DHT11();



  HAL_UART_Receive_IT(&huart4,&temp, sizeof(temp));

  //Read_from_DHT11();

//  DHT11_Start();

//  uint8_t resp = Check_Response();


//  if(resp != -1) Led_blinking2();

 //HAL_TIM_Base_Start_IT(&htim9);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
/*  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM9_Init();
  MX_UART4_Init();*/
  /* USER CODE BEGIN 2 */
  //lcd_put_cursor(0,0);
 // lcd_clear();
  lcd_cursor_display_mode(0,1,0);
//  lcd_put_cursor(0,0);
 // lcd_write_string("Hello Be Thui");
 // HAL_Delay(1000);
//  lcd_clear();
 // Display_number();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */





  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 SCH_Dispatch_Task();

//	  if(s__n == 100000)
//	  {
	//  HAL_UART_Transmit(&huart4, data, sizeof(data), 1000);
//
//		  s__n = 0;
//	  }
//
//	  else s__n++;

//	  Read_from_DHT11();

	// HAL_Delay(1000);

//	Led_blinking1();


	//  Delay_mili(1000);

	 // HAL_IWDG_Refresh(&hiwdg);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 2-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 65535;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
