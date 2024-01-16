/**	\file   buzzer.h
 *
 * 	\brief  Gestione Buzzer
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 14/03/2019
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _buzzer_h_
#define _buzzer_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif

		
/** Attiva beep singolo */
uint8_t Buzzer_Beep(void);

/** Da richiamare ogni ms */
void Buzzer_OnTick();
		


// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

