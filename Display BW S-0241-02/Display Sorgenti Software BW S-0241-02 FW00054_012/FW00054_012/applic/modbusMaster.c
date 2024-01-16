#include "modbusMaster.h"
#include "main.h"
#include "board.h"
#include "dati.h"
#include "uart1.h"
#include "maintimer.h"
#include "application.h"
#include "utility.h"
#include <string.h>

// Dimensione minima pacchetto modbus
#define MIN_PACKET_LEN			5




// Codici funzione
#define FUNCTION_READ_INPUT_REGISTERS		0x04
#define FUNCTION_WRITE_SINGLE_REGISTER		0x06


/** Codici eccezione */
#define EXCEPTION_NONE						0x00
#define EXCEPTION_ILLEGAL_FUNCTION			0x01
#define EXCEPTION_ILLEGAL_DATA_ADDRESS		0x02
#define EXCEPTION_ILLEGAL_DATA_VALUE		0x03
#define EXCEPTION_SERVER_DEVICE_FAILURE		0x04
#define EXCEPTION_NAK						0x07

typedef struct _tDataTable
{
	void *data; /**< Puntatore tabella */
	size_t size; /**< Dimensione tabella in bytes */
} tDataTable;



static uint8_t iobuffer[128];

static uint8_t currentSlave = 0;
static uint16_t currentRegister = 0;



void ModbusMaster_Init(void)
{
}



int ModbusMaster_Send(void *buffer, uint16_t size)
{

	// Abilita trasmissione 485
	RS485_TXEN_On();

	// Invio pacchetto
	if (Uart1_Send(buffer, size) == 0)
		return -1;


	return 0;
}





int ModbusMaster_OnPacketReceive(void *block, int len)
{

	if (len > MODBUS_BUFFER_SIZE)
		return 1;

//	Led_RxFlash(20);

	memcpy(Dati.modbusBuffer, (uint8_t *)block, len);
	Dati.modbusBufferLen = len;

	return 0;
}




/* gestione pacchetto ModBus Cablato*/
int ModbusMaster_Poll(void)
{
	uint8_t *src = (uint8_t *)Dati.modbusBuffer;
	uint8_t len;
	uint16_t packCrc;
	uint16_t value;

	// Timeout ricezione
	if (Dati.modbusTimeout)
		--Dati.modbusTimeout;


	// Pacchetto presente?
	if (Dati.modbusBufferLen == 0)
		return -1;

	len = Dati.modbusBufferLen;
	Dati.modbusBufferLen = 0;

	// Lunghezza valida?
	if (len < MIN_PACKET_LEN)
		return -1;

	// Se non è la scheda interrogata a rispondere
	if (src[0] != currentSlave)
		return -1;

	// Verifica validità crc
	packCrc = (src[len-2] << 8) + src[len-1];
	if (packCrc != Crc16((uint8_t *)Dati.modbusBuffer, len - 2))
		return -1;

	// Pacchetto corretto
	switch (src[1])
	{
		case (FUNCTION_READ_INPUT_REGISTERS):

			for (uint8_t w=0; w<src[2]/2; w++)
			{
				value = (src[3 + (w*2)] << 8) + src[4 + (w*2)];

				if ((currentRegister + w) == REGISTER_STATUS)
				{
					// Stato filtro
					if (value & STATUS_FILTRO)
					{
						Dati.filterStatus |= (1 << (currentSlave-1));
					}
					else
					{
						Dati.filterStatus &= ~(1 << (currentSlave-1));
					}

					// Stato ventole
					if (value & STATUS_SPEED_FAN_GREATER_THAN_30)
					{
						Dati.fanStatus |= (1 << (currentSlave-1));
					}
					else
					{
						Dati.fanStatus &= ~(1 << (currentSlave-1));
					}

					// Stato antincendio
					if (value & STATUS_ANTIFIRE)
					{
						Dati.antifireStatus |= (1L << (currentSlave-1));
					}
					else
					{
						Dati.antifireStatus &= ~(1L << (currentSlave-1));
					}

					// Stato sensori tacho su ventole
					if (value & STATUS_TACHO)
					{
						Dati.tachoStatus |= (1L << (currentSlave-1));
					}
					else
					{
						Dati.tachoStatus &= ~(1L << (currentSlave-1));
					}

					// Stato allarme sanificazione
					if (value & STATUS_SANITIZE)
					{
						Dati.sanitizeStatus |= (1L << (currentSlave-1));
						Dati.sanitizeTimeout[currentSlave-1] = SANITIZE_TIMEOUT_SEC;
					}
					else
					{
						Dati.sanitizeStatus &= ~(1L << (currentSlave-1));
					}
				}


				// Slave corrispondente alla scheda attualmente visualizzata sul display?
				if (Parameters.currentBoard == currentSlave)
				{
					Dati.registers[currentRegister + w] = value;
					Dati.modbusTimeout = MODBUS_TIMEOUT;
				}

			}
		break;

		default:
		break;
	}


	return 0;
}


int ModbusMaster_RequestRegisters(uint8_t slave, uint8_t first_reg, uint8_t num_reg)
{
	uint16_t crc;

	currentSlave = slave;
	currentRegister = first_reg;

	iobuffer[0] = slave;
	iobuffer[1] = FUNCTION_READ_INPUT_REGISTERS;
	iobuffer[2] = (first_reg >> 8);
	iobuffer[3] = (first_reg & 0xff);
	iobuffer[4] = (num_reg >> 8);
	iobuffer[5] = (num_reg & 0xff);


	crc = Crc16(iobuffer, 6);
	iobuffer[6] = (uint8_t) (crc >> 8);
	iobuffer[7] = (uint8_t) (crc & 0xFF);

	ModbusMaster_Send(iobuffer, 8);

	return 0;

}

int ModbusMaster_WriteRegisters(uint8_t slave, uint16_t reg, uint16_t value)
{
	uint16_t crc;

	currentSlave = slave;
	currentRegister = reg;

	iobuffer[0] = slave;
	iobuffer[1] = FUNCTION_WRITE_SINGLE_REGISTER;
	iobuffer[2] = (reg >> 8);
	iobuffer[3] = (reg & 0xff);
	iobuffer[4] = (value >> 8);
	iobuffer[5] = (value & 0xff);


	crc = Crc16(iobuffer, 6);
	iobuffer[6] = (uint8_t) (crc >> 8);
	iobuffer[7] = (uint8_t) (crc & 0xFF);

	ModbusMaster_Send(iobuffer, 8);

	return 0;

}

