/**	\file   uart3.h
 *
 * 	\brief  Gestione UART 3
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 19/05/2015
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _uart3_h_
#define _uart3_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif
		
		
#define UART3_RX_BUFFER_SIZE	128
#define UART3_TX_BUFFER_SIZE	128


		
/* Init */
int Uart3_Init(void);

		
/** Gestione Interrupt */
void Uart3_IRQHandler (void);
		
/** Invia pacchetto su UART
\param *block: puntatore a buffer dati
\param len:	numero di byte da inviare
\return Numero di byte inviati
*/
int Uart3_Send(void *block, uint8_t len);
		
/** Riceziona pacchetto su UART
	\param *block: puntatore a buffer dati
	\param len:	numero di byte da ricevere
	\return Numero di byte ricevuti		
*/
int Uart3_Recv(void *block, int len);
	

// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

