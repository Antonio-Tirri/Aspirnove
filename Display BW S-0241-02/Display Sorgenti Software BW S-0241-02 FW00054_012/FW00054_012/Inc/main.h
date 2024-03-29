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
#define LCD_D0_Pin GPIO_PIN_0
#define LCD_D0_GPIO_Port GPIOC
#define LCD_D1_Pin GPIO_PIN_1
#define LCD_D1_GPIO_Port GPIOC
#define LCD_D2_Pin GPIO_PIN_2
#define LCD_D2_GPIO_Port GPIOC
#define LCD_D3_Pin GPIO_PIN_3
#define LCD_D3_GPIO_Port GPIOC
#define SW5_Pin GPIO_PIN_0
#define SW5_GPIO_Port GPIOA
#define DIP_5_Pin GPIO_PIN_1
#define DIP_5_GPIO_Port GPIOA
#define DIP_6_Pin GPIO_PIN_2
#define DIP_6_GPIO_Port GPIOA
#define DIP_1_Pin GPIO_PIN_3
#define DIP_1_GPIO_Port GPIOA
#define DIP_2_Pin GPIO_PIN_4
#define DIP_2_GPIO_Port GPIOA
#define DIP_3_Pin GPIO_PIN_5
#define DIP_3_GPIO_Port GPIOA
#define DIP_4_Pin GPIO_PIN_6
#define DIP_4_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_7
#define SW1_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOC
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_0
#define SW2_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOB
#define SW3_Pin GPIO_PIN_2
#define SW3_GPIO_Port GPIOB
#define SW4_Pin GPIO_PIN_10
#define SW4_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOB
#define LCD_CLR_Pin GPIO_PIN_12
#define LCD_CLR_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_14
#define LCD_RW_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_15
#define LCD_EN_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOC
#define LED_V_Pin GPIO_PIN_8
#define LED_V_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_9
#define LED_R_GPIO_Port GPIOC
#define LCD_PWM_Pin GPIO_PIN_8
#define LCD_PWM_GPIO_Port GPIOA
#define RS485S_TX_Pin GPIO_PIN_10
#define RS485S_TX_GPIO_Port GPIOC
#define RS485S_RX_Pin GPIO_PIN_11
#define RS485S_RX_GPIO_Port GPIOC
#define RS485S_DIR_Pin GPIO_PIN_12
#define RS485S_DIR_GPIO_Port GPIOC
#define DIP_4B3_Pin GPIO_PIN_3
#define DIP_4B3_GPIO_Port GPIOB
#define RS485M_DIR_Pin GPIO_PIN_5
#define RS485M_DIR_GPIO_Port GPIOB
#define RS485M_TX_Pin GPIO_PIN_6
#define RS485M_TX_GPIO_Port GPIOB
#define RS485M_RX_Pin GPIO_PIN_7
#define RS485M_RX_GPIO_Port GPIOB
#define HW_SEL0_Pin GPIO_PIN_8
#define HW_SEL0_GPIO_Port GPIOB
#define HW_SEL1_Pin GPIO_PIN_9
#define HW_SEL1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
