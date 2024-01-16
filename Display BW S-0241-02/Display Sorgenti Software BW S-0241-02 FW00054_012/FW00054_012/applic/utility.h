/**	\file   utility.h
 *
 * 	\brief  Funzioni utility
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 26/04/2018
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _utility_h_
#define _utility_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif


/** Calcola CRC16 sul buffer di lunghezza size.*/
uint16_t Crc16(uint8_t* buffer, uint16_t size);


		
// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

