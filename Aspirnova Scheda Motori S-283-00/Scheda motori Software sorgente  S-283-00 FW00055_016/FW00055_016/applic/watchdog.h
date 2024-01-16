/**	\file   pwm.h
 *
 * 	\brief  Gestione Watchdog interno
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 26/09/2018
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */


#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__



/** Inizializza modulo watchdog. */
void Watchdog_Init(void);

/** Gestione controllore watchdog. Da richiamare nel main. */
void Watchdog_Poll(void);

/** Refresh watchdog. Da richiamare ogni ms. */
void Watchdog_OnTimer(void);

/** Forza reset per watchdog */
void Watchdog_ForcedReset(void);




#endif

