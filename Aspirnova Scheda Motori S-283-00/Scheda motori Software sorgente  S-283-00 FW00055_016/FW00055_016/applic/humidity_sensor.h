/**	\file   humidity_sensor.h
 *
 * 	\brief  Sensore umidità.
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com
 *
 *
 *  \version    v1.0 - 11/08/2020
 * 	-   Versione iniziale
 *
 * 	\section dsc Descrizione
 *  Il modulo contiene le funzioni di base per la gestione di un sensore di umidità 0-10V
 *
 */
#ifndef _humidity_sensor_h_
#define _humidity_sensor_h_

#include <stdio.h>



// Tipi sensore
enum
{
	HUMIDITY_SENSOR_TYPE_EE10_ELEKTRONIK = 0,

	HUMIDITY_SENSOR_TYPE_NUM
};





int16_t HumiditySensor_Convert(uint8_t type, float value);


#endif

