/**	\file   modbusSlave.h
 *
 * 	\brief  Gestione Modbus Slave
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
#ifndef _modbus_slave_h_
#define _modbus_slave_h_

#include <board.h>

// C++ Support
#ifdef __cplusplus
	extern "C" {
#endif



/** Inizializza modulo. */
void ModbusSlave_Init(void);

/** Gestione pacchetto modbus appena ricevuto. */
int ModbusSlave_OnPacketReceive(void);

/** Gestione pacchetto modbus cablato. Da richiamare nel main. */
int ModbusSlave_Poll(void);


int ModbusSlave_Send(void *buffer, uint16_t size);

int ModbusSlave_RequestRegisters(uint8_t slave, uint8_t first_reg, uint8_t num_reg);

int ModbusSlave_WriteRegisters(uint8_t slave, uint16_t reg, uint16_t value);




// C++ Support
#ifdef __cplusplus
	}
#endif

#endif 

