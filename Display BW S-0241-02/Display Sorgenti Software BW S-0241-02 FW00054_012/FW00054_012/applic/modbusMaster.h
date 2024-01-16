/**	\file   modbus.h
 *
 * 	\brief  Gestione Modbus Master
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 12/11/2020
 * 	-   Versione iniziale
 *
 * 	\section req Requisiti per il corretto funzionamento
 * 	-   Nessuno
 *
 */
#ifndef _modbus_master_h_
#define _modbus_master_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif


// Tempo massima attesa risposta
#define MODBUS_TIMEOUT			(2000 / TIME_MAIN_CYCLE)


/** Inizializza modulo. */
void ModbusMaster_Init(void);

/** Gestione pacchetto modbus appena ricevuto. */
int ModbusMaster_OnPacketReceive(void *block, int len);


/** Gestione pacchetto modbus cablato. Da richiamare nel main. */
int ModbusMaster_Poll(void);


int ModbusMaster_Send(void *buffer, uint16_t size);


int ModbusMaster_RequestRegisters(uint8_t slave, uint8_t first_reg, uint8_t num_reg);

int ModbusMaster_WriteRegisters(uint8_t slave, uint16_t reg, uint16_t value);




// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

