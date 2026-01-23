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
#define O13_Pin GPIO_PIN_2
#define O13_GPIO_Port GPIOE
#define O14_Pin GPIO_PIN_3
#define O14_GPIO_Port GPIOE
#define O15_Pin GPIO_PIN_4
#define O15_GPIO_Port GPIOE
#define O16_Pin GPIO_PIN_5
#define O16_GPIO_Port GPIOE
#define O17_Pin GPIO_PIN_6
#define O17_GPIO_Port GPIOE
#define O18_Pin GPIO_PIN_13
#define O18_GPIO_Port GPIOC
#define i1_home_x_Pin GPIO_PIN_0
#define i1_home_x_GPIO_Port GPIOC
#define i1_home_x_EXTI_IRQn EXTI0_IRQn
#define i2_home_y_Pin GPIO_PIN_1
#define i2_home_y_GPIO_Port GPIOC
#define i2_home_y_EXTI_IRQn EXTI1_IRQn
#define i3_home_z_Pin GPIO_PIN_2
#define i3_home_z_GPIO_Port GPIOC
#define i3_home_z_EXTI_IRQn EXTI2_IRQn
#define i4_estop_Pin GPIO_PIN_3
#define i4_estop_GPIO_Port GPIOC
#define i4_estop_EXTI_IRQn EXTI3_IRQn
#define i5_stop_Pin GPIO_PIN_4
#define i5_stop_GPIO_Port GPIOC
#define i5_stop_EXTI_IRQn EXTI4_IRQn
#define i9_pressure_Pin GPIO_PIN_7
#define i9_pressure_GPIO_Port GPIOE
#define i9_pressure_EXTI_IRQn EXTI9_5_IRQn
#define i12_vacum1_Pin GPIO_PIN_10
#define i12_vacum1_GPIO_Port GPIOE
#define i12_vacum1_EXTI_IRQn EXTI15_10_IRQn
#define i13_vacum2_Pin GPIO_PIN_11
#define i13_vacum2_GPIO_Port GPIOE
#define i13_vacum2_EXTI_IRQn EXTI15_10_IRQn
#define i14_Door_L_Pin GPIO_PIN_12
#define i14_Door_L_GPIO_Port GPIOE
#define i14_Door_L_EXTI_IRQn EXTI15_10_IRQn
#define i15_Door_R_Pin GPIO_PIN_13
#define i15_Door_R_GPIO_Port GPIOE
#define i15_Door_R_EXTI_IRQn EXTI15_10_IRQn
#define i16_start_Pin GPIO_PIN_14
#define i16_start_GPIO_Port GPIOE
#define i16_start_EXTI_IRQn EXTI15_10_IRQn
#define i17_reset_Pin GPIO_PIN_15
#define i17_reset_GPIO_Port GPIOE
#define i17_reset_EXTI_IRQn EXTI15_10_IRQn
#define output_x_pull_Pin GPIO_PIN_6
#define output_x_pull_GPIO_Port GPIOC
#define output_x_sig_Pin GPIO_PIN_7
#define output_x_sig_GPIO_Port GPIOC
#define output_z_pull_Pin GPIO_PIN_8
#define output_z_pull_GPIO_Port GPIOC
#define output_z_sig_Pin GPIO_PIN_9
#define output_z_sig_GPIO_Port GPIOC
#define output_y_pull_Pin GPIO_PIN_8
#define output_y_pull_GPIO_Port GPIOA
#define output_y_sig_Pin GPIO_PIN_9
#define output_y_sig_GPIO_Port GPIOA
#define O1_Pin GPIO_PIN_3
#define O1_GPIO_Port GPIOD
#define O2_Pin GPIO_PIN_4
#define O2_GPIO_Port GPIOD
#define O3_Pin GPIO_PIN_5
#define O3_GPIO_Port GPIOD
#define O4_Pin GPIO_PIN_6
#define O4_GPIO_Port GPIOD
#define O5_Pin GPIO_PIN_7
#define O5_GPIO_Port GPIOD
#define O6_Pin GPIO_PIN_3
#define O6_GPIO_Port GPIOB
#define O7_Pin GPIO_PIN_4
#define O7_GPIO_Port GPIOB
#define O8_Pin GPIO_PIN_5
#define O8_GPIO_Port GPIOB
#define O9_Pin GPIO_PIN_8
#define O9_GPIO_Port GPIOB
#define O10_Pin GPIO_PIN_9
#define O10_GPIO_Port GPIOB
#define O11_Pin GPIO_PIN_0
#define O11_GPIO_Port GPIOE
#define O12_Pin GPIO_PIN_1
#define O12_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
