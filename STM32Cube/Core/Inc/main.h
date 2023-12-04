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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern int switches_byte;
extern int test_value;
extern int tPass;
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
#define SW7_Pin GPIO_PIN_1
#define SW7_GPIO_Port GPIOA
#define SW4_Pin GPIO_PIN_2
#define SW4_GPIO_Port GPIOA
#define DCDC_EN_Pin GPIO_PIN_3
#define DCDC_EN_GPIO_Port GPIOA
#define SW8_Pin GPIO_PIN_5
#define SW8_GPIO_Port GPIOA
#define SW6_Pin GPIO_PIN_6
#define SW6_GPIO_Port GPIOA
#define SW5_Pin GPIO_PIN_7
#define SW5_GPIO_Port GPIOA
#define JOY_CCW_Pin GPIO_PIN_0
#define JOY_CCW_GPIO_Port GPIOB
#define JOY_GND_Pin GPIO_PIN_1
#define JOY_GND_GPIO_Port GPIOB
#define JOY_CW_Pin GPIO_PIN_10
#define JOY_CW_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_12
#define OLED_CS_GPIO_Port GPIOB
#define OLES_SSPI_MOSI_Pin GPIO_PIN_13
#define OLES_SSPI_MOSI_GPIO_Port GPIOB
#define OLES_SSPI_SCK_Pin GPIO_PIN_15
#define OLES_SSPI_SCK_GPIO_Port GPIOB
#define ADC_EN_Pin GPIO_PIN_8
#define ADC_EN_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_9
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_15
#define SW2_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_6
#define SW3_GPIO_Port GPIOB
#define VBUS_EN_Pin GPIO_PIN_7
#define VBUS_EN_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_8
#define OLED_DC_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_9
#define OLED_RES_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
