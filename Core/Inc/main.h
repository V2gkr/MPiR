/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define M1_start_Pin GPIO_PIN_0
#define M1_start_GPIO_Port GPIOC
#define M1_start_EXTI_IRQn EXTI0_IRQn
#define M1_stop_Pin GPIO_PIN_1
#define M1_stop_GPIO_Port GPIOC
#define M1_stop_EXTI_IRQn EXTI1_IRQn
#define M2_start_Pin GPIO_PIN_2
#define M2_start_GPIO_Port GPIOC
#define M2_start_EXTI_IRQn EXTI2_IRQn
#define M2_stop_Pin GPIO_PIN_3
#define M2_stop_GPIO_Port GPIOC
#define M2_stop_EXTI_IRQn EXTI3_IRQn
#define LED_START_Pin GPIO_PIN_0
#define LED_START_GPIO_Port GPIOA
#define LED_STOP_Pin GPIO_PIN_1
#define LED_STOP_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define START_BUTTON_Pin GPIO_PIN_4
#define START_BUTTON_GPIO_Port GPIOA
#define START_BUTTON_EXTI_IRQn EXTI4_IRQn
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define STOP_BUTTON_Pin GPIO_PIN_6
#define STOP_BUTTON_GPIO_Port GPIOA
#define STOP_BUTTON_EXTI_IRQn EXTI9_5_IRQn
#define M2_step_Pin GPIO_PIN_10
#define M2_step_GPIO_Port GPIOB
#define M2_direction_Pin GPIO_PIN_8
#define M2_direction_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define M1_step_Pin GPIO_PIN_4
#define M1_step_GPIO_Port GPIOB
#define M1_direction_Pin GPIO_PIN_5
#define M1_direction_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
