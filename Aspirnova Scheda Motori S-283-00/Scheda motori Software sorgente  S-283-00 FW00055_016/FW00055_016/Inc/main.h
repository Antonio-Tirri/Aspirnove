/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include "stm32f2xx_hal.h"

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
#define DIP_1_Pin GPIO_PIN_0
#define DIP_1_GPIO_Port GPIOC
#define DIP_2_Pin GPIO_PIN_1
#define DIP_2_GPIO_Port GPIOC
#define DIP_3_Pin GPIO_PIN_2
#define DIP_3_GPIO_Port GPIOC
#define DIP_4_Pin GPIO_PIN_3
#define DIP_4_GPIO_Port GPIOC
#define AN0_CO2_Pin GPIO_PIN_0
#define AN0_CO2_GPIO_Port GPIOA
#define AN1_AUX1_Pin GPIO_PIN_1
#define AN1_AUX1_GPIO_Port GPIOA
#define AN2_AUX2_Pin GPIO_PIN_2
#define AN2_AUX2_GPIO_Port GPIOA
#define AN3_TEMP_EXT_Pin GPIO_PIN_3
#define AN3_TEMP_EXT_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_5
#define FLASH_SCK_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_6
#define FLASH_MISO_GPIO_Port GPIOA
#define FLASH_MOSI_Pin GPIO_PIN_7
#define FLASH_MOSI_GPIO_Port GPIOA
#define AN14_TEMP_INT_Pin GPIO_PIN_4
#define AN14_TEMP_INT_GPIO_Port GPIOC
#define RELE_AUX1_Pin GPIO_PIN_5
#define RELE_AUX1_GPIO_Port GPIOC
#define TACH_F1_Pin GPIO_PIN_0
#define TACH_F1_GPIO_Port GPIOB
#define TACH_F1_EXTI_IRQn EXTI0_IRQn
#define TACH_F2_Pin GPIO_PIN_1
#define TACH_F2_GPIO_Port GPIOB
#define TACH_F2_EXTI_IRQn EXTI1_IRQn
#define DIP_5_Pin GPIO_PIN_2
#define DIP_5_GPIO_Port GPIOB
#define ANOUT_H2O_Pin GPIO_PIN_10
#define ANOUT_H2O_GPIO_Port GPIOB
#define ANOUT_BATT_Pin GPIO_PIN_11
#define ANOUT_BATT_GPIO_Port GPIOB
#define RELE_FAN2_Pin GPIO_PIN_12
#define RELE_FAN2_GPIO_Port GPIOB
#define RELE_BATT_Pin GPIO_PIN_13
#define RELE_BATT_GPIO_Port GPIOB
#define DIP_7_Pin GPIO_PIN_14
#define DIP_7_GPIO_Port GPIOB
#define RELE_FAN1_Pin GPIO_PIN_15
#define RELE_FAN1_GPIO_Port GPIOB
#define RELE_BYP_Pin GPIO_PIN_6
#define RELE_BYP_GPIO_Port GPIOC
#define DIN_TERMOST_Pin GPIO_PIN_7
#define DIN_TERMOST_GPIO_Port GPIOC
#define DIN_FILTRO_Pin GPIO_PIN_8
#define DIN_FILTRO_GPIO_Port GPIOC
#define DIN_HV2_Pin GPIO_PIN_9
#define DIN_HV2_GPIO_Port GPIOC
#define DIN_HV1_Pin GPIO_PIN_8
#define DIN_HV1_GPIO_Port GPIOA
#define ANOUT_FAN1_Pin GPIO_PIN_15
#define ANOUT_FAN1_GPIO_Port GPIOA
#define LED_ERR_Pin GPIO_PIN_10
#define LED_ERR_GPIO_Port GPIOC
#define LED_ON_Pin GPIO_PIN_11
#define LED_ON_GPIO_Port GPIOC
#define UART5_TX_Pin GPIO_PIN_12
#define UART5_TX_GPIO_Port GPIOC
#define UART5_RX_Pin GPIO_PIN_2
#define UART5_RX_GPIO_Port GPIOD
#define ANOUT_FAN2_Pin GPIO_PIN_3
#define ANOUT_FAN2_GPIO_Port GPIOB
#define DIP_6_Pin GPIO_PIN_4
#define DIP_6_GPIO_Port GPIOB
#define RS485_DIR_Pin GPIO_PIN_5
#define RS485_DIR_GPIO_Port GPIOB
#define RS485_TX_Pin GPIO_PIN_6
#define RS485_TX_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_7
#define RS485_RX_GPIO_Port GPIOB
#define LED_PC1_Pin GPIO_PIN_8
#define LED_PC1_GPIO_Port GPIOB
#define LED_PC2_Pin GPIO_PIN_9
#define LED_PC2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
