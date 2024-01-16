/**	\file   uart1.h
 *
 * 	\brief  Gestione UART 1
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
#ifndef _uart1_h_
#define _uart1_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif
		
		
#define UART1_RX_BUFFER_SIZE	128
#define UART1_TX_BUFFER_SIZE	128


		
/* Init */
int Uart1_Init(void);

		
/** Gestione Interrupt */
void Uart1_IRQHandler (void);
		
/** Invia pacchetto su UART
\param *block: puntatore a buffer dati
\param len:	numero di byte da inviare
\return Numero di byte inviati
*/
int Uart1_Send(void *block, uint8_t len);
		
/** Riceziona pacchetto su UART
	\param *block: puntatore a buffer dati
	\param len:	numero di byte da ricevere
	\return Numero di byte ricevuti		
*/
int Uart1_Recv(void *block, int len);
	
/** Funzione da richiamare ogni ms, per identificare la completa ricezione di un pacchetto dall'uart */
void Uart1_RTOManagement(void);


// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

