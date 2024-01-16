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
#define HARDWARE_S0242			0
#define HARDWARE_S0283			1	// 202007:  7 dip anzichè 4, 2 pin per identificare l'hardware, TACH_F1 e TACH_F2 per misurare la velocità dei motori


#define FAN_1					0
#define FAN_2					1

// LED ERR
#define LED_ERR_On()			HAL_GPIO_WritePin	(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_RESET)
#define LED_ERR_Off()			HAL_GPIO_WritePin	(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_SET)
#define LED_ERR_Toggle()		HAL_GPIO_TogglePin	(LED_ERR_GPIO_Port, LED_ERR_Pin)
#define LED_ERR_Get()			HAL_GPIO_ReadPin	(LED_ERR_GPIO_Port, LED_ERR_Pin)

// LED ON
#define LED_ON_On()				HAL_GPIO_WritePin	(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET)
#define LED_ON_Off()			HAL_GPIO_WritePin	(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET)
#define LED_ON_Toggle()			HAL_GPIO_TogglePin	(LED_ON_GPIO_Port, LED_ON_Pin)

// LED PC1
#define LED_PC1_On()			HAL_GPIO_WritePin	(LED_PC1_GPIO_Port, LED_PC1_Pin, GPIO_PIN_RESET)
#define LED_PC1_Off()			HAL_GPIO_WritePin	(LED_PC1_GPIO_Port, LED_PC1_Pin, GPIO_PIN_SET)
#define LED_PC1_Toggle()		HAL_GPIO_TogglePin	(LED_PC1_GPIO_Port, LED_PC1_Pin)

// LED PC2
#define LED_PC2_On()			HAL_GPIO_WritePin	(LED_PC2_GPIO_Port, LED_PC2_Pin, GPIO_PIN_RESET)
#define LED_PC2_Off()			HAL_GPIO_WritePin	(LED_PC2_GPIO_Port, LED_PC2_Pin, GPIO_PIN_SET)
#define LED_PC2_Toggle()		HAL_GPIO_TogglePin	(LED_PC2_GPIO_Port, LED_PC2_Pin)



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

// DIP_7
#define DIP_7_IsOn()			(HAL_GPIO_ReadPin	(DIP_7_GPIO_Port, DIP_7_Pin) == 0)


// HW_REV (ex UART5)
#define HW_REV_1_Get()			(HAL_GPIO_ReadPin	(UART5_TX_GPIO_Port, UART5_TX_Pin))
#define HW_REV_2_Get()			(HAL_GPIO_ReadPin	(UART5_RX_GPIO_Port, UART5_RX_Pin))


// DIN
#define DIN_HV1_Get()			(HAL_GPIO_ReadPin	(DIN_HV1_GPIO_Port, DIN_HV1_Pin) == 0)
#define DIN_HV2_Get()			(HAL_GPIO_ReadPin	(DIN_HV2_GPIO_Port, DIN_HV2_Pin) == 0)
#define DIN_TERMOST_Get()		(HAL_GPIO_ReadPin	(DIN_TERMOST_GPIO_Port, DIN_TERMOST_Pin) == 0)
#define DIN_FILTRO_Get()		(HAL_GPIO_ReadPin	(DIN_FILTRO_GPIO_Port, DIN_FILTRO_Pin) == 0)

// RELE_FAN1
#define RELE_FAN1_On()			HAL_GPIO_WritePin	(RELE_FAN1_GPIO_Port, RELE_FAN1_Pin, GPIO_PIN_SET)
#define RELE_FAN1_Off()			HAL_GPIO_WritePin	(RELE_FAN1_GPIO_Port, RELE_FAN1_Pin, GPIO_PIN_RESET)
#define RELE_FAN1_Toggle()		HAL_GPIO_TogglePin	(RELE_FAN1_GPIO_Port, RELE_FAN1_Pin)
#define RELE_FAN1_Get()			HAL_GPIO_ReadPin	(RELE_FAN1_GPIO_Port, RELE_FAN1_Pin)

// RELE_FAN2
#define RELE_FAN2_On()			HAL_GPIO_WritePin	(RELE_FAN2_GPIO_Port, RELE_FAN2_Pin, GPIO_PIN_SET)
#define RELE_FAN2_Off()			HAL_GPIO_WritePin	(RELE_FAN2_GPIO_Port, RELE_FAN2_Pin, GPIO_PIN_RESET)
#define RELE_FAN2_Toggle()		HAL_GPIO_TogglePin	(RELE_FAN2_GPIO_Port, RELE_FAN2_Pin)
#define RELE_FAN2_Get()			HAL_GPIO_ReadPin	(RELE_FAN2_GPIO_Port, RELE_FAN2_Pin)

// RELE_BATT
#define RELE_BATT_On()			HAL_GPIO_WritePin	(RELE_BATT_GPIO_Port, RELE_BATT_Pin, GPIO_PIN_SET)
#define RELE_BATT_Off()			HAL_GPIO_WritePin	(RELE_BATT_GPIO_Port, RELE_BATT_Pin, GPIO_PIN_RESET)
#define RELE_BATT_Toggle()		HAL_GPIO_TogglePin	(RELE_BATT_GPIO_Port, RELE_BATT_Pin)
#define RELE_BATT_Get()			HAL_GPIO_ReadPin	(RELE_BATT_GPIO_Port, RELE_BATT_Pin)


// RELE_AUX1
#define RELE_AUX1_On()			HAL_GPIO_WritePin	(RELE_AUX1_GPIO_Port, RELE_AUX1_Pin, GPIO_PIN_SET)
#define RELE_AUX1_Off()			HAL_GPIO_WritePin	(RELE_AUX1_GPIO_Port, RELE_AUX1_Pin, GPIO_PIN_RESET)
#define RELE_AUX1_Toggle()		HAL_GPIO_TogglePin	(RELE_AUX1_GPIO_Port, RELE_AUX1_Pin)
#define RELE_AUX1_Get()			HAL_GPIO_ReadPin	(RELE_AUX1_GPIO_Port, RELE_AUX1_Pin)

// RELE_BYP
#define RELE_BYP_On()			HAL_GPIO_WritePin	(RELE_BYP_GPIO_Port, RELE_BYP_Pin, GPIO_PIN_SET)
#define RELE_BYP_Off()			HAL_GPIO_WritePin	(RELE_BYP_GPIO_Port, RELE_BYP_Pin, GPIO_PIN_RESET)
#define RELE_BYP_Toggle()		HAL_GPIO_TogglePin	(RELE_BYP_GPIO_Port, RELE_BYP_Pin)
#define RELE_BYP_Get()			HAL_GPIO_ReadPin	(RELE_BYP_GPIO_Port, RELE_BYP_Pin)


// RS485_TXEN
#define RS485_TXEN_On()			HAL_GPIO_WritePin	(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET)
#define RS485_TXEN_Off()		HAL_GPIO_WritePin	(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET)




// TIMER
#define TIMER2					(2 << 4)


#define PWM_FAN1				(TIMER2 + TIM_CHANNEL_1)
#define PWM_FAN2				(TIMER2 + TIM_CHANNEL_2)
#define PWM_H2O					(TIMER2 + TIM_CHANNEL_3)
#define PWM_BATT				(TIMER2 + TIM_CHANNEL_4)




/** Inizializza modulo */
void Board_Init(void);

/** Restituisce il tipo di hardware riconosciuto (in base ai pin PC12 - PD2): HARDWARE_2019 - HARDWARE_2020*/
uint8_t Board_GetHWRev(void);

/** Restituisce baud rate impostato (in base ai dip DIP_6 e DIP_7): BAUD_RATE_9600 - BAUD_RATE_19200 - BAUD_RATE_115200 */
uint8_t Board_GetBaudRate(void);

/** Restituisce Id scheda impostato su DIP switch (1..32) */
uint8_t Board_GetId(void);

/** Invia la richiesta di aggiornamento firmware applicazione al bootloader */
void Board_RequestUpgrade(void);


/** Verifica se la stringa in ingresso è un numero. Se si carica il valore numerico in value e restituisce TRUE. */
uint8_t isNumber(char *data, uint16_t len, int32_t *value);

/** Attivazione uscite RELE BATT - PWM BATT */
void BATT_Cmd(uint8_t cmd);

/** Attivazione uscite RELE FCOM, RELE FAN1, PWM FAN1 e RELE FCOM, RELE FAN2, PWM FAN2 */
void FANS_Cmd(uint8_t cmd1, uint8_t cmd2);


/** Restituisce il valore della sonda CO2, in ppm */
uint16_t AirQuality_Convert(uint16_t type, float volt);


/** Restituisce 0 se sono all'interno di un orario impostato per questo giorno */
uint8_t Schedule_IsSetTime(uint8_t day_week, uint8_t hour, uint8_t min, uint16_t *details);

/** Restituisce 0 se il calendario indica di attivare l'uscita sterilizzatore */
uint8_t Sterilizer_IsSetTime(uint8_t day_week, uint8_t hour, uint8_t min);



void GPIO_SetInput(GPIO_TypeDef* port, unsigned int bit, unsigned char pull);

void GPIO_SetOutput(GPIO_TypeDef* port, unsigned int bit);

#endif


