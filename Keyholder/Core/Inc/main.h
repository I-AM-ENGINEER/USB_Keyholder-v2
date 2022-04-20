/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern int switches_byte;
extern int test_value;
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
#define DISPLAY_ON2_Pin GPIO_PIN_1
#define DISPLAY_ON2_GPIO_Port GPIOA
#define DISPLAY_ON1_Pin GPIO_PIN_2
#define DISPLAY_ON1_GPIO_Port GPIOA
#define SW4_Pin GPIO_PIN_3
#define SW4_GPIO_Port GPIOA
#define SW4_EXTI_IRQn EXTI3_IRQn
#define SW5_Pin GPIO_PIN_4
#define SW5_GPIO_Port GPIOA
#define SW5_EXTI_IRQn EXTI4_IRQn
#define SW6_Pin GPIO_PIN_5
#define SW6_GPIO_Port GPIOA
#define SW6_EXTI_IRQn EXTI9_5_IRQn
#define SW7_Pin GPIO_PIN_6
#define SW7_GPIO_Port GPIOA
#define SW7_EXTI_IRQn EXTI9_5_IRQn
#define SW8_Pin GPIO_PIN_7
#define SW8_GPIO_Port GPIOA
#define SW8_EXTI_IRQn EXTI9_5_IRQn
#define ADC_VBAT_Pin GPIO_PIN_0
#define ADC_VBAT_GPIO_Port GPIOB
#define VBAT_EN_Pin GPIO_PIN_2
#define VBAT_EN_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_8
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI9_5_IRQn
#define SW2_Pin GPIO_PIN_9
#define SW2_GPIO_Port GPIOA
#define SW2_EXTI_IRQn EXTI9_5_IRQn
#define SW3_Pin GPIO_PIN_10
#define SW3_GPIO_Port GPIOA
#define SW3_EXTI_IRQn EXTI15_10_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
