/**	\file   modbus.h
 *
 * 	\brief  Gestione Modbus
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 19/03/2019
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _modbus_h_
#define _modbus_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif



/** Inizializza modulo. */
void Modbus_Init(void);

/** Gestione pacchetto modbus appena ricevuto. */
int Modbus_OnPacketReceive(void *block, int len);

/** Gestione pacchetto modbus cablato. Da richiamare nel main. */
int Modbus_Poll(void);

/** Invio pacchetto */
int Modbus_Send(void *buffer, uint16_t size);



// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

