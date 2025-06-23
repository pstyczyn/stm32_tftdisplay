/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CC1 16000
#define A_Pin GPIO_PIN_0
#define A_GPIO_Port GPIOA
#define B_Pin GPIO_PIN_1
#define B_GPIO_Port GPIOA
#define E_Pin GPIO_PIN_4
#define E_GPIO_Port GPIOA
#define F_Pin GPIO_PIN_5
#define F_GPIO_Port GPIOA
#define G_Pin GPIO_PIN_6
#define G_GPIO_Port GPIOA
#define RESET_Pin GPIO_PIN_4
#define RESET_GPIO_Port GPIOC
#define Zielony_1_Pin GPIO_PIN_5
#define Zielony_1_GPIO_Port GPIOC
#define Ad2_Pin GPIO_PIN_0
#define Ad2_GPIO_Port GPIOB
#define Bd2_Pin GPIO_PIN_1
#define Bd2_GPIO_Port GPIOB
#define Cd2_Pin GPIO_PIN_2
#define Cd2_GPIO_Port GPIOB
#define Zolty_1_Pin GPIO_PIN_6
#define Zolty_1_GPIO_Port GPIOC
#define Czerwony_1_Pin GPIO_PIN_8
#define Czerwony_1_GPIO_Port GPIOC
#define Czerwony_2_Pin GPIO_PIN_9
#define Czerwony_2_GPIO_Port GPIOC
#define DC_Pin GPIO_PIN_10
#define DC_GPIO_Port GPIOA
#define D_Pin GPIO_PIN_11
#define D_GPIO_Port GPIOA
#define C_Pin GPIO_PIN_12
#define C_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_15
#define CS_GPIO_Port GPIOA
#define Gd2_Pin GPIO_PIN_11
#define Gd2_GPIO_Port GPIOC
#define Dd2_Pin GPIO_PIN_3
#define Dd2_GPIO_Port GPIOB
#define Ed2_Pin GPIO_PIN_4
#define Ed2_GPIO_Port GPIOB
#define Fd2_Pin GPIO_PIN_5
#define Fd2_GPIO_Port GPIOB
#define Zolty_2_Pin GPIO_PIN_8
#define Zolty_2_GPIO_Port GPIOB
#define Zielony_2_Pin GPIO_PIN_9
#define Zielony_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
