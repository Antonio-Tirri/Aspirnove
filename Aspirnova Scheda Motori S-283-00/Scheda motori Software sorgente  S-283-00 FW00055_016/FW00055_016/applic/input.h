/**	\file   input.h
 * 
 * 	\brief  Input driver
 * 
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 13/02/2017
 * 	-   Versione iniziale
 * 
 *  \section dsc Description
 *  -
 * 
 *  -	
 * 
 */
#ifndef _input_h_
#define _input_h_

#include <stdint.h>



#define DIN_HV1			((uint16_t)0x01)
#define DIN_HV2			((uint16_t)0x02)
#define DIN_TERMOST		((uint16_t)0x04)
#define DIN_FILTRO		((uint16_t)0x08)


// Valore di default per filterTick (input.c)
#define	FILTER_TICK				5

// Valore di default per filterLen (input.c)
#define FILTER_LEN				10

// Numero di ingressi da leggere
#define FILTERED_INPUT			4



/** Inizializza modulo ingressi.
	\return 0 se operazione conclusa correttamente
*/
uint8_t Input_Init(void);

/** Restituisce stato ingressi IN_1..IN_8 
	\return Stato ingressi: Bit0 = 1 se IN_1 ON, Bit0 = 0 se IN_1 OFF .. Bit7 = 1 se IN_8 ON, Bit7 = 0 se IN_8 OFF 
*/
uint8_t Input_GetValue(void);

/** Restituisce stato ingressi IN_1..IN_8 filtrati secondo le tempistiche definite.
	\return Stato ingressi: Bit0 = 1 se IN_1 ON, Bit0 = 0 se IN_1 OFF .. Bit7 = 1 se IN_8 ON, Bit7 = 0 se IN_8 OFF 
*/
uint8_t Input_GetFilteredValue(void);

/** Gestione filtro su ingressi. Da richiamare ogni ms. */
void Input_Tick(void);

/** Richiamato su interrupt EXTI
 * \param fan: ventola interessata
 * */
void Input_EXTIEvent(uint8_t fan);

#endif 

