/**	\file   tastiera.h
 *
 *	\brief  Modulo lettura tastiera
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
#ifndef _TASTIERA_H_
#define _TASTIERA_H_


#include "stdint.h"

// Tasti
#define TASTO_ON			(1L << 0)
#define TASTO_UP			(1L << 1)
#define TASTO_DOWN			(1L << 2)
#define TASTO_M				(1L << 3)
#define TASTO_BP			(1L << 4)

// Tasti istante precedente
#define TASTO_ON_PREC		((unsigned long)TASTO_ON		<< 16)
#define TASTO_UP_PREC		((unsigned long)TASTO_UP		<< 16)
#define TASTO_DOWN_PREC		((unsigned long)TASTO_DOWN		<< 16)
#define TASTO_M_PREC		((unsigned long)TASTO_M			<< 16)
#define TASTO_BP_PREC		((unsigned long)TASTO_BP		<< 16)



// Pulsanti menù
#define DKEY_ENTER	 		(TASTO_M | !TASTO_M_PREC)
#define DKEY_ESC			(TASTO_ON | !TASTO_ON_PREC)
#define DKEY_SX				0
#define DKEY_DX				0
#define DKEY_DEL			0
#define DKEY_DOT			0
#define SKEY_DS_INC			(TASTO_UP | !TASTO_UP_PREC)
#define SKEY_DS_DEC			(TASTO_DOWN | !TASTO_DOWN_PREC)
#define SKEY_DS_INC_PRESS	(TASTO_UP | TASTO_UP_PREC)
#define SKEY_DS_DEC_PRESS	(TASTO_DOWN | TASTO_DOWN_PREC)



/** Lettura tastiera con memorizzazione istante precedente, per poter gestire, a livello applicativo, pressioni e rilasci. */
void Tastiera_Read(uint32_t *k);

/** Forza pressione tasto tastiera */
void Tastiera_ForceEnterCmd(void);

#endif 
