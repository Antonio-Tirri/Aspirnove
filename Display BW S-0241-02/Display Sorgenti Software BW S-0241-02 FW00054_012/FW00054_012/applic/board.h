/**	\file   board.h
 * 
 * 	\brief  Board configuration
 * 
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *  \version    v1.0 - 21/02/2019
 * 	-   Versione iniziale
 *
 *
 *  \section req Requirements
 *
 */ 
#include "stm32f2xx_hal.h"
#include "stm32f2xx_hal_gpio.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef _BOARD_H_
#define _BOARD_H_

// Boolean
#define TRUE		1
#define FALSE		0

// Result
#define RESULT_OK		0
#define RESULT_ERR		1


// Revisioni hardware
#define HARDWARE_2019			0
#define HARDWARE_2020			1	// 202011:  6 dip anzichè 4, 2 pin per identificare l'hardware, UART per RS485 slave


// LED VERDE
#define LED_GREEN_On()			HAL_GPIO_WritePin	(LED_V_GPIO_Port, LED_V_Pin, GPIO_PIN_SET)
#define LED_GREEN_Off()			HAL_GPIO_WritePin	(LED_V_GPIO_Port, LED_V_Pin, GPIO_PIN_RESET)
#define LED_GREEN_Toggle()		HAL_GPIO_TogglePin	(LED_V_GPIO_Port, LED_V_Pin)

// LED ROSSO
#define LED_RED_On()			HAL_GPIO_WritePin	(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET)
#define LED_RED_Off()			HAL_GPIO_WritePin	(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET)
#define LED_RED_Toggle()		HAL_GPIO_TogglePin	(LED_R_GPIO_Port, LED_R_Pin)

// AUTO_ON
#define BUZZER_On()				HAL_GPIO_WritePin	(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET)
#define BUZZER_Off()			HAL_GPIO_WritePin	(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET)


// DIP_1
#define DIP_1_IsOn()			(HAL_GPIO_ReadPin	(DIP_1_GPIO_Port, DIP_1_Pin) == 0)

// DIP_2
#define DIP_2_IsOn()			(HAL_GPIO_ReadPin	(DIP_2_GPIO_Port, DIP_2_Pin) == 0)

// DIP_3
#define DIP_3_IsOn()			(HAL_GPIO_ReadPin	(DIP_3_GPIO_Port, DIP_3_Pin) == 0)

// DIP_4
#define DIP_4_IsOn()			(HAL_GPIO_ReadPin	(DIP_4_GPIO_Port, DIP_4_Pin) == 0)

// DIP_5
#define DIP_5_IsOn()			(HAL_GPIO_ReadPin	(DIP_5_GPIO_Port, DIP_5_Pin) == 0)

// DIP_6
#define DIP_6_IsOn()			(HAL_GPIO_ReadPin	(DIP_6_GPIO_Port, DIP_6_Pin) == 0)


#define HW_REV_1_Get()			(HAL_GPIO_ReadPin	(HW_SEL0_GPIO_Port, HW_SEL0_Pin))
#define HW_REV_2_Get()			(HAL_GPIO_ReadPin	(HW_SEL1_GPIO_Port, HW_SEL1_Pin))


// SW1
#define SW1_Pressed()			(HAL_GPIO_ReadPin	(SW1_GPIO_Port, SW1_Pin) == 0)
#define SW2_Pressed()			(HAL_GPIO_ReadPin	(SW2_GPIO_Port, SW2_Pin) == 0)
#define SW3_Pressed()			(HAL_GPIO_ReadPin	(SW3_GPIO_Port, SW3_Pin) == 0)
#define SW4_Pressed()			(HAL_GPIO_ReadPin	(SW4_GPIO_Port, SW4_Pin) == 0)
#define SW5_Pressed()			(HAL_GPIO_ReadPin	(SW5_GPIO_Port, SW5_Pin) == 0)

// AUTO_ON
#define RS485_TXEN_On()		HAL_GPIO_WritePin	(RS485M_DIR_GPIO_Port, RS485M_DIR_Pin, GPIO_PIN_SET)
#define RS485_TXEN_Off()		HAL_GPIO_WritePin	(RS485M_DIR_GPIO_Port, RS485M_DIR_Pin, GPIO_PIN_RESET)




// TIMER
#define TIMER1					(1 << 4)
#define TIMER3					(3 << 4)


#define PWM_BACKLIGHT			(TIMER1 + TIM_CHANNEL_1)
#define PWM_BUZZER				(TIMER3 + TIM_CHANNEL_4)





/** Inizializza modulo */
void Board_Init(void);

/** Restituisce lo stato dei dip switch */
uint8_t Board_GetDipSwitch(void);

/** Invia la richiesta di aggiornamento firmware applicazione al bootloader */
void Board_RequestUpgrade(void);


/** Verifica se la stringa in ingresso è un numero. Se si carica il valore numerico in value e restituisce TRUE. */
uint8_t isNumber(char *data, uint16_t len, int32_t *value);


/** Restituisce il tipo di hardware riconosciuto (in base ai pin PB8 - PB9): HARDWARE_2019 - HARDWARE_2020*/
uint8_t Board_GetHWRev(void);



void GPIO_SetInput(GPIO_TypeDef* port, unsigned int bit, unsigned char pull);

void GPIO_SetOutput(GPIO_TypeDef* port, unsigned int bit);

#endif


