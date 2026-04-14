/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define O6_Pin GPIO_PIN_2
#define O6_GPIO_Port GPIOE
#define O5_Pin GPIO_PIN_3
#define O5_GPIO_Port GPIOE
#define O4_Pin GPIO_PIN_4
#define O4_GPIO_Port GPIOE
#define O3_Pin GPIO_PIN_5
#define O3_GPIO_Port GPIOE
#define O2_Pin GPIO_PIN_6
#define O2_GPIO_Port GPIOE
#define O1_Pin GPIO_PIN_13
#define O1_GPIO_Port GPIOC
#define i1_home_x_Pin GPIO_PIN_0
#define i1_home_x_GPIO_Port GPIOC
#define i1_home_x_EXTI_IRQn EXTI0_IRQn
#define i2_home_y_Pin GPIO_PIN_2
#define i2_home_y_GPIO_Port GPIOC
#define i2_home_y_EXTI_IRQn EXTI2_IRQn
#define i3_home_z_Pin GPIO_PIN_3
#define i3_home_z_GPIO_Port GPIOC
#define i3_home_z_EXTI_IRQn EXTI3_IRQn
#define i4_vacum1_Pin GPIO_PIN_0
#define i4_vacum1_GPIO_Port GPIOA
#define i5_vacum2_Pin GPIO_PIN_3
#define i5_vacum2_GPIO_Port GPIOA
#define i6_vacum3_Pin GPIO_PIN_0
#define i6_vacum3_GPIO_Port GPIOB
#define i7_vacum4_Pin GPIO_PIN_1
#define i7_vacum4_GPIO_Port GPIOB
#define i8_vacum5_Pin GPIO_PIN_2
#define i8_vacum5_GPIO_Port GPIOB
#define i9_vacum6_Pin GPIO_PIN_7
#define i9_vacum6_GPIO_Port GPIOE
#define i10_start_Pin GPIO_PIN_8
#define i10_start_GPIO_Port GPIOE
#define i10_start_EXTI_IRQn EXTI9_5_IRQn
#define i11_stop_Pin GPIO_PIN_9
#define i11_stop_GPIO_Port GPIOE
#define i11_stop_EXTI_IRQn EXTI9_5_IRQn
#define output_z_sig_Pin GPIO_PIN_15
#define output_z_sig_GPIO_Port GPIOB
#define output_y_sig_Pin GPIO_PIN_11
#define output_y_sig_GPIO_Port GPIOD
#define O23_Pin GPIO_PIN_13
#define O23_GPIO_Port GPIOD
#define output_x_sig_Pin GPIO_PIN_14
#define output_x_sig_GPIO_Port GPIOD
#define O24_Pin GPIO_PIN_15
#define O24_GPIO_Port GPIOD
#define output_x_pull_Pin GPIO_PIN_6
#define output_x_pull_GPIO_Port GPIOC
#define O19_Pin GPIO_PIN_7
#define O19_GPIO_Port GPIOC
#define output_z_pull_Pin GPIO_PIN_8
#define output_z_pull_GPIO_Port GPIOC
#define O21_Pin GPIO_PIN_9
#define O21_GPIO_Port GPIOC
#define output_y_pull_Pin GPIO_PIN_8
#define output_y_pull_GPIO_Port GPIOA
#define O17_Pin GPIO_PIN_9
#define O17_GPIO_Port GPIOA
#define O20_Pin GPIO_PIN_10
#define O20_GPIO_Port GPIOA
#define O18_Pin GPIO_PIN_15
#define O18_GPIO_Port GPIOA
#define O22_Pin GPIO_PIN_12
#define O22_GPIO_Port GPIOC
#define O16_Pin GPIO_PIN_2
#define O16_GPIO_Port GPIOD
#define O15_Pin GPIO_PIN_7
#define O15_GPIO_Port GPIOD
#define O14_Pin GPIO_PIN_3
#define O14_GPIO_Port GPIOB
#define O13_Pin GPIO_PIN_4
#define O13_GPIO_Port GPIOB
#define O12_Pin GPIO_PIN_6
#define O12_GPIO_Port GPIOB
#define O11_Pin GPIO_PIN_7
#define O11_GPIO_Port GPIOB
#define O10_Pin GPIO_PIN_8
#define O10_GPIO_Port GPIOB
#define O9_Pin GPIO_PIN_9
#define O9_GPIO_Port GPIOB
#define O8_Pin GPIO_PIN_0
#define O8_GPIO_Port GPIOE
#define O7_Pin GPIO_PIN_1
#define O7_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
