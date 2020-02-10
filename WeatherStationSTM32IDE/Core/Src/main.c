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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "BME280.h"
#include "ESP8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GreenLedLow  HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, 0)
#define GreenLedHigh HAL_GPIO_WritePin(GreenLed_GPIO_Port, GreenLed_Pin, 1)

#define YellowLedLow  HAL_GPIO_WritePin(YellowLed_GPIO_Port, YellowLed_Pin, 0)
#define YellowLedHigh HAL_GPIO_WritePin(YellowLed_GPIO_Port, YellowLed_Pin, 1)

#define RedLedLow  HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, 0)
#define RedLedHigh HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, 1)

#define Start   0
#define Working 1
#define Sleep   2
#define Error   3
#define Warning 4
#define OperationGood 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int currentStationStatus = -1;
bool ledsEnable = true;
uint32_t lastPressed = 0;

char buff[128];
float currentBatteryVoltage;
int counter = 0;

bool request, connect, disconnect, echo, restart, test;

BME280_WeatherData *currentWeather = NULL;

volatile char recvComBuff[64];

extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef DateToUpdate;
extern RTC_AlarmTypeDef sAlarm;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void PC_Send(char *str);
float getBatteryVoltage();
void BME280_Start();
void ESP8266_Start();
void BME280_GetWeather();
void SendRequest();
void DisconnectFromAP();
void ShowTimeRTC();
void StationStatus(int newStationStatus);
void StopMode();
void SleepMode();
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
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  StationStatus(Start);

  int BME280_InitStatus = BME280_Init();

  if(BME280_InitStatus == BME280_INIT_FAIL)
  {
  	StationStatus(Error);
  	NVIC_SystemReset();
  }

  ESP8266_SetConfig(&huart2, GPIOB, GPIO_PIN_10);
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  	StationStatus(Working);

  	ESP8266_ON();

	  request = false;
	  connect = false;
	  disconnect = false;

	  currentWeather = BME280_GetWeatherData();
	  currentBatteryVoltage = getBatteryVoltage();

	  for(int i = 0; i < 3; ++i)
	  {
	  	StationStatus(Working);
	  	HAL_Delay(2000);

	  	//connect = ESP8266_ConnectTo("Snapy", "31055243167vlad");
	  	connect = ESP8266_ConnectTo("MERCUSYS_7EBA", "3105vlad3010vlada");

	  	if(connect)
	  		  break;

	  	StationStatus(Warning);
	  	restart = ESP8266_Restart();
	  }

	  if(!connect)
	  {
	  	StationStatus(Error);
	  	NVIC_SystemReset();
	  }

	  sprintf(buff, "GET /weatherStation/main.php?type=addNewRecord&t=%2.2f&h=%3.2f&p=%4.2f&v=%2.2f", currentWeather->temperature, currentWeather->humidity, currentWeather->pressure, currentBatteryVoltage);
	  request = ESP8266_SendRequest("TCP", "192.168.1.102", 80, buff);

	  if(!request)
	  {
	  	StationStatus(Error);
	  	NVIC_SystemReset();
	  }

	  disconnect = ESP8266_DisconnectFromWifi();

		if(!disconnect)
		{
			StationStatus(Error);
			NVIC_SystemReset();
		}

	  ESP8266_OFF();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  sTime.Hours = 0;
	  sTime.Minutes = 0;
	  sTime.Seconds = 0;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	  {
	  	Error_Handler();
	  }

	  sAlarm.AlarmTime.Hours = 0;
	  sAlarm.AlarmTime.Minutes = 0;
	  sAlarm.AlarmTime.Seconds = 15;
	  sAlarm.Alarm = RTC_ALARM_A;
	  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	  {
	  	Error_Handler();
	  }

	  StationStatus(Sleep);
	  //HAL_Delay(3000);
	  //StopMode();
	  SleepMode();
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
float getBatteryVoltage()
{
	const float r1 = 47.7;
	const float r2 = 91;

	//const float ADC_ReferenceVoltage = 3.3;
	const float ADC_ReferenceVoltage = 5.0;
	const float ADC_Resolution = 4095;

	MX_ADC1_Init();

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	uint32_t ADC_Value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	HAL_ADC_DeInit(&hadc1);

	float ADC_Voltage = (ADC_Value / ADC_Resolution) * ADC_ReferenceVoltage;

	float realVoltage = ADC_Voltage * (r1 + r2) / r2;

	return realVoltage;
}

void PC_Send(char *str)
{
	//HAL_UART_Transmit(&huart1,(uint8_t*)str,strlen(str),1000);
	//CDC_Transmit_FS(str, strlen(str));
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GetTick() - lastPressed > 300 && GPIO_Pin == LedsControlButton_Pin)
	{
		lastPressed = HAL_GetTick();

		if(ledsEnable == true)
		{
			GreenLedLow;
			YellowLedLow;
			RedLedLow;

			ledsEnable = false;

			HAL_GPIO_DeInit(GPIOB, YellowLed_Pin | GreenLed_Pin);
			HAL_GPIO_DeInit(RedLed_GPIO_Port, RedLed_Pin);
		}
		else
		{
			ledsEnable = true;

			GPIO_InitTypeDef GPIO_InitStruct = {0};

			HAL_GPIO_WritePin(RedLed_GPIO_Port, RedLed_Pin, GPIO_PIN_RESET);
			GPIO_InitStruct.Pin = RedLed_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_Init(RedLed_GPIO_Port, &GPIO_InitStruct);

		  HAL_GPIO_WritePin(GPIOB, YellowLed_Pin | GreenLed_Pin, GPIO_PIN_RESET);
			GPIO_InitStruct.Pin = YellowLed_Pin | GreenLed_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		}

		StationStatus(-1);
	}
}

void StationStatus(int newStationStatus)
{
	if(newStationStatus != -1)
		currentStationStatus = newStationStatus;

	if(ledsEnable)
	{
		GreenLedLow;
		YellowLedLow;
		RedLedLow;

		switch(currentStationStatus)
		{
			case Start:
				for(int i = 0; i < 5; ++i)
				{
					GreenLedLow;
					YellowLedLow;
					RedLedLow;

					HAL_Delay(100);

					GreenLedHigh;
					YellowLedHigh;
					RedLedHigh;

					HAL_Delay(100);
				}
				break;

			case Working:
				YellowLedHigh;
				break;

			case Sleep:
				GreenLedHigh;
				break;

			case Error:
				for(int i = 0; i < 10; ++i)
				{
					RedLedHigh;
					HAL_Delay(100);
					RedLedLow;
					HAL_Delay(100);
				}
				break;

			case Warning:
				for(int i = 0; i < 5; ++i)
				{
					YellowLedHigh;
					HAL_Delay(100);
					YellowLedLow;
					HAL_Delay(100);
				}
				break;

			case OperationGood:
				GreenLedHigh;
				HAL_Delay(300);
				GreenLedLow;
				break;
		}
	}
}

void StopMode()
{
	HAL_SuspendTick();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	HAL_ResumeTick();
}

void SleepMode()
{
    //Suspend Tick increment to prevent wakeup by Systick interrupt. Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)
    HAL_SuspendTick();

    //Before we can access to every register of the PWR peripheral we must enable it
    __HAL_RCC_PWR_CLK_ENABLE();

    //Request to enter SLEEP mode
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

    //Resume Tick interrupt if disabled prior to sleep mode entry
    HAL_ResumeTick();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	while(1)
	{
		StationStatus(Error);
	}
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
