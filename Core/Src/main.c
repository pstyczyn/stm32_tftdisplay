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
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "my_functions.h"
#include "LCD_functions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
unsigned int licznik=9;
bool flaga=TRUE;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
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

	adres_t ADRES;
	ADRES.ZERO=0x00;
	ADRES.SOFTWARERESET=0x01;
	ADRES.SLEEPOUT=0x11;
	ADRES.DISPLAYOFF=0x28;
	ADRES.DISPLAYON=0x29;
	ADRES.INTERFEJSON=0x3A;
	ADRES.BIT_12_ON_PIXEL=0x03;
	ADRES.BIT_16_ON_PIXEL=0x05;
	ADRES.BIT_18_ON_PIXEL=0x06;
	ADRES.CASET=0x2A;
	ADRES.RASET=0x2B;
	ADRES.MEMWRITE=0x2C;
	ADRES.X0_SET=0x00;
	ADRES.Y0_SET=0x00;
	ADRES.X_END_SET=ADRES.X0_SET+30;
	ADRES.Y_END_SET=ADRES.Y0_SET+60;
	ADRES.RM=0;
	ADRES.GM=63;
	ADRES.BM=31;
	ADRES.Mbyte=0x00;
	ADRES.Mbyte2=0x00;
	ADRES.Mbyte=((ADRES.Mbyte^ADRES.RM)<<3)^(ADRES.GM&0x38);
	ADRES.Mbyte2=((ADRES.Mbyte2^(ADRES.GM&0x07))<<5)^ADRES.BM;



	unsigned int i=0,j=0;
	long int napiecie=0;
	float fnapiecie=0.0;
	long int napiecie2=0;
	float fnapiecie2=0.0;
	float fnapiecie2_1=0.0;
	unsigned int seg1=0;
	unsigned int seg2=0;
	unsigned int tabADC[4]={0};
	float tabADC2[2]={0};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart2, &powitanie, sizeof(powitanie)-1, HAL_MAX_DELAY);
  HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
  allsegment(FALSE);
  LCD_init(&ADRES);
  LCD_printMENU(1, 1, 1, &ADRES);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_ADC_Start(&hadc2);
	  HAL_ADC_Start(&hadc1);
	  while(HAL_ADC_PollForConversion(&hadc2, 1)==HAL_OK)
	  {
		  //allsegment(FALSE);
		  napiecie2=HAL_ADC_GetValue(&hadc2);
		  fnapiecie2=(float)(napiecie2)*(8059.0/10000000.0);
		  fnapiecie2_1=fnapiecie2;
		  if(tryb==7)
		  {
			  fnapiecie2=fnapiecie2-(unsigned int)fnapiecie2;
			  num1=fnapiecie2*10;
			  tabADC[0]=fnapiecie2_1;
			  tabADC[1]=num1;
		  }
		  if(aDC2)
		  {
			  allsegment(FALSE);
//			  HAL_Delay(100);
			  segment1(fnapiecie2,TRUE);
			  fnapiecie2=fnapiecie2-(unsigned int)fnapiecie2;
			  num1=fnapiecie2*10;
			  segment2(num1,TRUE);
			  if(aDC1_wyslano)
			  {
				  LCD_komenda(&ADRES.DISPLAYOFF);
				  LCD_print_tryb_adc_numbers(1,1,1,&ADRES,fnapiecie2_1,num1,TRUE);
				  LCD_komenda(&ADRES.DISPLAYON);
			  }
		  }
		  if(lineON)
		  {
			  allsegment(FALSE);
			  tabADC2[0]=fnapiecie2;
			  linijka2(fnapiecie2);
		  }
		  HAL_ADC_Stop(&hadc2);
	  }
	  while(HAL_ADC_PollForConversion(&hadc1, 1)==HAL_OK)
	  {
		  //allsegment(FALSE);
		  napiecie=HAL_ADC_GetValue(&hadc1);
		  fnapiecie=(float)(napiecie)*(8059.0/10000000.0);
		  fnapiecie2_1=fnapiecie;
		  if(tryb==7)
		  {
			  fnapiecie=fnapiecie-(unsigned int)fnapiecie;
			  num1=fnapiecie*10;
			  tabADC[2]=fnapiecie2_1;
			  tabADC[3]=num1;
		  }
		  if(aDC1)
		  {
			  allsegment(FALSE);
			  segment1(fnapiecie,TRUE);
			  fnapiecie=fnapiecie-(unsigned int)fnapiecie;
			  num1=fnapiecie*10;
			  segment2(num1,TRUE);
			  if(aDC1_wyslano)
			  {
				  LCD_komenda(&ADRES.DISPLAYOFF);
				  LCD_print_tryb_adc_numbers(1,1,1,&ADRES,fnapiecie2_1,num1,FALSE);
				  LCD_komenda(&ADRES.DISPLAYON);
			  }
		  }
		  if(lineON)
		  {
			  tabADC2[1]=fnapiecie;
			  allsegment(FALSE);
			  linijka1(fnapiecie);
		  }
		  HAL_ADC_Stop(&hadc1);
	  }


	  if(odebrano)
	  {
		  if(rozmiar)
		  {
			  UART_decision(buforRx[0], &ADRES, tabADC);
		  }
	  }
	  else
	  {
		  if((tryb>=1 && tryb<=3) || tryb==4)
		  {
			  LCD_print_tryb_diody(1, 1, 1, &ADRES);
		  }
		  else if(tryb==7)
		  {
			  LCD_print_tryb_segment(1, 1, 1, &ADRES, tabADC);
		  }
		  else if(tryb==6)
		  {
			  LCD_print_tryb_tank(1, 1, 1, &ADRES, tabADC2);
		  }
	  }
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		if(buforRx[bufRxIndex]==10)
		{
			rozmiar=bufRxIndex;
			odebrano=TRUE;
			for(;bufRxIndex<17;bufRxIndex++)
			{
				buforRx[bufRxIndex]=0;
			}
			bufRxIndex=0;

		}
		else
		{
			bufRxIndex++;

			if(bufRxIndex>16)
			{
				bufRxIndex=0;
			}
		}
	}
	HAL_UART_Receive_IT(&huart2, &buforRx[bufRxIndex], 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		bufRxIndex++;
		UART_transmit(tryb);
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
  while(1)
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
