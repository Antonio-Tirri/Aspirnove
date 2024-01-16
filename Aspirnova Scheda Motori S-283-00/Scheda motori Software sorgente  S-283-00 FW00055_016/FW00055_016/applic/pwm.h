/**	\file   pwm.h
 *
 * 	\brief  Gestione PWM
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 04/05/2015
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _pwm_h_
#define _pwm_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif

		
/* Init */
int PWM_Init(void);

/* Set pwm channel
		\param channel: TIM_CHANNEL_1 .. TIM_CHANNEL_4
		\param value: value 0 .. 65535
		\return 0: OK
				-1: ERR
*/
int PWM_Set(uint8_t channel, uint16_t value);
		
/* Set pwm channel
		\param channel: TIM_CHANNEL_1 .. TIM_CHANNEL_4
		\param percent_value: percent value 0 .. 100%
		\return 0: OK
				-1: ERR
*/
int PWM_SetPercent(uint8_t channel, uint16_t percent_value);



/** PID su PWM H2O.
		\param Errore temperatura, in °C * 10
		\return Percentuale PWM applicata
 * */
uint8_t PID_H2O_Poll(int16_t error);


/** PID su PWM fan
		\param Errore CO2, in ppm
		\return Percentuale PWM da applicare
 */
uint8_t PID_FAN_Poll(uint8_t fan, uint16_t cmd_base, int16_t error);

// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

