/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
#define MAEN1_Pin GPIO_PIN_2
#define MAEN1_GPIO_Port GPIOE
#define MAEN2_Pin GPIO_PIN_3
#define MAEN2_GPIO_Port GPIOE
#define MAF1_Pin GPIO_PIN_4
#define MAF1_GPIO_Port GPIOE
#define MAF2_Pin GPIO_PIN_5
#define MAF2_GPIO_Port GPIOE
#define ACMD1_Pin GPIO_PIN_4
#define ACMD1_GPIO_Port GPIOA
#define ACMD2_Pin GPIO_PIN_5
#define ACMD2_GPIO_Port GPIOA
#define SW_Pin GPIO_PIN_6
#define SW_GPIO_Port GPIOA
#define SW10_Pin GPIO_PIN_7
#define SW10_GPIO_Port GPIOA
#define DI01_Pin GPIO_PIN_4
#define DI01_GPIO_Port GPIOC
#define DI02_Pin GPIO_PIN_5
#define DI02_GPIO_Port GPIOC
#define DI04_Pin GPIO_PIN_0
#define DI04_GPIO_Port GPIOB
#define DI03_Pin GPIO_PIN_1
#define DI03_GPIO_Port GPIOB
#define DI06_Pin GPIO_PIN_7
#define DI06_GPIO_Port GPIOE
#define DI05_Pin GPIO_PIN_8
#define DI05_GPIO_Port GPIOE
#define DI08_Pin GPIO_PIN_9
#define DI08_GPIO_Port GPIOE
#define DI07_Pin GPIO_PIN_10
#define DI07_GPIO_Port GPIOE
#define DI10_Pin GPIO_PIN_11
#define DI10_GPIO_Port GPIOE
#define DI09_Pin GPIO_PIN_12
#define DI09_GPIO_Port GPIOE
#define DI12_Pin GPIO_PIN_13
#define DI12_GPIO_Port GPIOE
#define DI11_Pin GPIO_PIN_14
#define DI11_GPIO_Port GPIOE
#define DI14_Pin GPIO_PIN_15
#define DI14_GPIO_Port GPIOE
#define DI13_Pin GPIO_PIN_10
#define DI13_GPIO_Port GPIOB
#define DI15_Pin GPIO_PIN_12
#define DI15_GPIO_Port GPIOB
#define DO01_Pin GPIO_PIN_13
#define DO01_GPIO_Port GPIOB
#define DO02_Pin GPIO_PIN_14
#define DO02_GPIO_Port GPIOB
#define DO03_Pin GPIO_PIN_15
#define DO03_GPIO_Port GPIOB
#define DO04_Pin GPIO_PIN_8
#define DO04_GPIO_Port GPIOD
#define VALVE2_EN_Pin GPIO_PIN_9
#define VALVE2_EN_GPIO_Port GPIOD
#define VELVE1_STATUS_Pin GPIO_PIN_10
#define VELVE1_STATUS_GPIO_Port GPIOD
#define VELVE2_STATUS_Pin GPIO_PIN_11
#define VELVE2_STATUS_GPIO_Port GPIOD
#define VALVE1_EN_Pin GPIO_PIN_12
#define VALVE1_EN_GPIO_Port GPIOD
#define VALVE4_EN_Pin GPIO_PIN_13
#define VALVE4_EN_GPIO_Port GPIOD
#define VELVE3_STATUS_Pin GPIO_PIN_14
#define VELVE3_STATUS_GPIO_Port GPIOD
#define VELVE4_STATUS_Pin GPIO_PIN_15
#define VELVE4_STATUS_GPIO_Port GPIOD
#define VALVE3_EN_Pin GPIO_PIN_6
#define VALVE3_EN_GPIO_Port GPIOC
#define VALVE6_EN_Pin GPIO_PIN_7
#define VALVE6_EN_GPIO_Port GPIOC
#define VELVE5_STATUS_Pin GPIO_PIN_8
#define VELVE5_STATUS_GPIO_Port GPIOC
#define VELVE6_STATUS_Pin GPIO_PIN_9
#define VELVE6_STATUS_GPIO_Port GPIOC
#define VALVE5_EN_Pin GPIO_PIN_8
#define VALVE5_EN_GPIO_Port GPIOA
#define VALVE8_EN_Pin GPIO_PIN_11
#define VALVE8_EN_GPIO_Port GPIOA
#define VELVE7_STATUS_Pin GPIO_PIN_12
#define VELVE7_STATUS_GPIO_Port GPIOA
#define VELVE8_STATUS_Pin GPIO_PIN_10
#define VELVE8_STATUS_GPIO_Port GPIOC
#define VALVE7_EN_Pin GPIO_PIN_11
#define VALVE7_EN_GPIO_Port GPIOC
#define VALVE9_EN_Pin GPIO_PIN_12
#define VALVE9_EN_GPIO_Port GPIOC
#define VELVE9_STATUS_Pin GPIO_PIN_0
#define VELVE9_STATUS_GPIO_Port GPIOD
#define VALVE10_EN_Pin GPIO_PIN_1
#define VALVE10_EN_GPIO_Port GPIOD
#define VELVE10_STATUS_Pin GPIO_PIN_2
#define VELVE10_STATUS_GPIO_Port GPIOD
#define VALVE12_EN_Pin GPIO_PIN_3
#define VALVE12_EN_GPIO_Port GPIOD
#define VELVE11_STATUS_Pin GPIO_PIN_4
#define VELVE11_STATUS_GPIO_Port GPIOD
#define VELVE12_STATUS_Pin GPIO_PIN_5
#define VELVE12_STATUS_GPIO_Port GPIOD
#define VALVE11_EN_Pin GPIO_PIN_6
#define VALVE11_EN_GPIO_Port GPIOD
#define VELVE13_STATUS_Pin GPIO_PIN_7
#define VELVE13_STATUS_GPIO_Port GPIOD
#define VALVE14_EN_Pin GPIO_PIN_3
#define VALVE14_EN_GPIO_Port GPIOB
#define VALVE13_EN_Pin GPIO_PIN_4
#define VALVE13_EN_GPIO_Port GPIOB
#define VELVE14_STATUS_Pin GPIO_PIN_5
#define VELVE14_STATUS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
