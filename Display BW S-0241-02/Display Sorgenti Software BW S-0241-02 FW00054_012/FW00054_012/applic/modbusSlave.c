#include "modbusSlave.h"
#include "main.h"
#include "board.h"
#include "dati.h"
#include "uart1.h"

#include "application.h"
#include "utility.h"
#include <string.h>




void ModbusSlave_Init(void)
{
	// Utilizzare Uart3
}



int ModbusSlave_Send(void *buffer, uint16_t size)
{
	// Abilita trasmissione 485
	//RS485S_TXEN_On();



	return 0;
}

int ModbusSlave_OnPacketReceive(void)
{
	return 0;
}




/* Gestione pacchetto ModBus Slave */
int ModbusSlave_Poll(void)
{

	return 0;
}


int ModbusSlave_RequestRegisters(uint8_t slave, uint8_t first_reg, uint8_t num_reg)
{

	return 0;
}

int ModbusSlave_WriteRegisters(uint8_t slave, uint16_t reg, uint16_t value)
{
	
	return 0;
}

