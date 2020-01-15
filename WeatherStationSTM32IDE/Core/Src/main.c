/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <usbd_cdc_if.h>

#include "BME280.h"
#include "ESP8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG_LEDS
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef DateToUpdate = {0};

char buff[128];
float currentBatteryVoltage;
int counter = 0;

bool request, connect, disconnect, echo, restart, test;

BME280_WeatherData *currentWeather = NULL;

volatile char recvComBuff[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PC_Send(char *str)
{
	//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
	CDC_Transmit_FS(str, strlen(str));
}

float getBatteryVoltage()
{
	const float r1 = 17.70;
	const float r2 = 16.05;
	const float ADC_ReferenceVoltage = 3.3;
	const float ADC_Resolution = 4095;

	uint32_t ADC_Value = 0;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	ADC_Value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	float ADC_InputVoltage = (ADC_Value / ADC_Resolution) * ADC_ReferenceVoltage;

	float realVoltage = ADC_InputVoltage / (r1 / r2);

	//float realVoltage = ADC_Value / 1085.0 / 0.48;

	return realVoltage;
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  PC_Send("****************************** Start ******************************\n");

  #ifdef DEBUG_LEDS
	  for(int i = 0; i < 5; ++i)
	  {
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

		  HAL_Delay(100);

		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

		  HAL_Delay(100);
	  }
  #endif

  BME280_Init();
  ESP8266_Init(&huart2, GPIOB, GPIO_PIN_10);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  PC_Send("\nBegin\n");

	  #ifdef DEBUG_LEDS
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	  #endif

	  request = false;
	  connect = false;
	  disconnect = false;

	  for(int i = 0; i < 3; ++i)
	  {
		  ESP8266_ON();
		  HAL_Delay(2000);

		  //connect = ESP8266_ConnectTo("Snapy", "31055243167vlad");
		  connect = ESP8266_ConnectTo("MERCUSYS_7EBA", "3105vlad3010vlada");
	      if(connect)
	      {
	    	  PC_Send("[ OK ] Connect to access point\n");
	    	  break;
	      }

	      PC_Send("[ WARNING ] Connect to access point failed! ESP Restart\n");
	      restart = ESP8266_Restart();

	      if(restart)
	      	  PC_Send("[ OK ] ESP Restart\n");
	      else
	          PC_Send("[ ERROR ] ESP Restart\n");
	  }

	  if(!connect)
	  {
		  PC_Send("[ ERROR ] Connect to access point\n");
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		  HAL_Delay(1000);
		  PC_Send("**************** System restart ****************\n");
	      NVIC_SystemReset();
	  }

	  currentWeather = BME280_GetWeatherData();
	  currentBatteryVoltage = getBatteryVoltage();

	  sprintf(buff, "GET /weatherStation/addWeather.php?t=%d&h=%d&p=%d&v=%2.2f", (int)currentWeather->temperature, (int)currentWeather->humidity, (int)currentWeather->pressure, currentBatteryVoltage);
	  request = ESP8266_SendRequest("TCP", "192.168.1.102", 80, buff);

	  if(!request)
	  {
		  PC_Send("[ ERROR ] request = false;\n");
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		  HAL_Delay(1000);
		  PC_Send("**************** System restart ****************\n");
	      NVIC_SystemReset();
	  }
	  else
	  {
		  PC_Send("[ OK ] Request\n");
	  }

	  disconnect = ESP8266_DisconnectFromWifi();

	  if(!disconnect)
	  {
		  PC_Send("[ ERROR ] disconnect = false;\n");
		  PC_Send("**************** System restart ****************\n");
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		  HAL_Delay(1000);
	      NVIC_SystemReset();
	  }
	  else
	  {
		  PC_Send("[ OK ] Disconnect\n");
	  }

	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	  ++counter;
	  sprintf(buff, "[%d-%d-%d %d:%d:%d] %d\n",DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year, sTime.Hours, sTime.Minutes, sTime.Seconds, counter);
	  PC_Send(buff);

	  ESP8266_OFF();
	  PC_Send("End\n");

	  #ifdef DEBUG_LEDS
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	  #endif

      HAL_Delay(10000);

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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
