/**	\file   maintimer.h
 * 
 * 	\brief  Main timer driver
 * 
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *  \version    v1.0 - 13/03/2019
 * 	-   Versione iniziale
 * 
 */
#ifndef _maintimer_h_
#define _maintimer_h_





// C++ Support
#ifdef __cplusplus
extern "C"
{
#endif
	
/** Inizializza il modulo MainTimer */
void MainTimer_Init(void);

/** Da richiamare ogni ms */
void MainTimer_OnTick(void);

/** Restituisce il valore corrente del timer, in ms
 * 	\return	Main timer value */
unsigned int MainTimer_GetValue(void);

/** Attesa dell'intervallo richiesto, in ms
 * 	\param value: Tempo, in ms */
void MainTimer_Wait(unsigned int value);

	
	
// C++ Support
#ifdef __cplusplus
}
#endif

#endif 
