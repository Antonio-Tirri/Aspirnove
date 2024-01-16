/**	\file   temperature_sensor.h
 *
 * 	\brief  Sensori temperatura.
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com
 *
 *
 *  \version    v1.0 - 04/05/2015
 * 	-   Versione iniziale
 *
 * 	\section dsc Descrizione
 *  Il modulo contiene le funzioni di base per la gestione di un sensore di temperatura TCR6180 (Ni1000).
 *
 */
#ifndef _ni1000_tcr6180_h_
#define _ni1000_tcr6180_h_

#include <stdio.h>



// Tipi sensore
enum
{
	TEMPERATURE_SENSOR_TYPE_NTC_10K_3435 = 0,

	TEMPERATURE_SENSOR_TYPE_NUM
};





int16_t TemperatureSensor_Convert(uint8_t type, int16_t value);


#endif

