#include "modbus.h"
#include "main.h"
#include "board.h"
#include "dati.h"
#include "uart1.h"
#include "maintimer.h"
#include "application.h"
#include "utility.h"
#include "usb.h"
#include "rtc.h"
#include <string.h>

// Dimensione minima pacchetto modbus
#define MIN_PACKET_LEN			5



// Attesa prima di rispondere al master RS485, una volta ricevuta una richiesta
#define MODBUS_WAIT_FOR_RESPONSE	1 //(30 / TIME_MAIN_CYCLE)



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
static uint8_t broadcast = 0;




/** Puntatori a tabelle valori */
/***********************************************************************************************/


static tDataTable tableInputRegisters = { NULL, 0 };
void Modbus_LinkInputRegistersTable(void *data, size_t size)
{
	tableInputRegisters.data = data;
	tableInputRegisters.size = size;
}
/***********************************************************************************************/





void Modbus_Init(void)
{
	// Link a Input Register
	Modbus_LinkInputRegistersTable(&Dati.registers[REGISTER_1], sizeof(Dati.registers) ); // Tutti i registri
}



int Modbus_Send(void *buffer, uint16_t size)
{

	// Nessuna risposta per i pacchetti broadcast
	if (broadcast == TRUE)
		return 0;

	// Abilita trasmissione 485
	RS485_TXEN_On();

	// Invio pacchetto
	if (Uart1_Send(buffer, size) == 0)
		return -1;

	//Led_TxFlash(50);


	return 0;
}

int Modbus_CanReadRegister(uint16_t address)
{
	if (address < REGISTERS_NUM)
		return 0;
	else
		return 1;
}



int Modbus_CanWriteRegister(uint16_t address, uint16_t value)
{
	if (address >= REGISTERS_NUM)
			return 1;


	return 0;
}


int Modbus_SaveRegisterRequired(uint16_t address)
{
	if (address >= REGISTERS_NUM)
			return 1;

/*
	switch (address)
	{
		case (REGISTER_RELAY):
			//Parameters.regTimeout = Dati.registers[address];
			return 0;


		default:
			return 1;
	}
*/
	return 0;
}




/** Metodo interno. Risponde al master  con i valori dei registri richiesti. */
static int read16bitRegisters(uint8_t *pdu, uint16_t size, tDataTable *dataTable)
{
	uint8_t i;
	uint16_t *dataPtr;
	uint16_t crc;
	uint16_t quantity, address;


	// Verifica tabella
	if ((dataTable == NULL) || (dataTable->data == NULL) || (dataTable->size == 0))
	{
		return EXCEPTION_ILLEGAL_DATA_VALUE;
	}
	else
	{
		dataPtr = dataTable->data;
	}

	// Decodifica indirizzo
	address = pdu[2];
	address <<= 8;
	address += pdu[3];


	// Decodifica bytes
	quantity = pdu[4];
	quantity <<= 8;
	quantity += pdu[5];


	// Verifica indirizzo e quantità richiesta
	if ((address + quantity) > (dataTable->size))
	{
		return EXCEPTION_ILLEGAL_DATA_ADDRESS;
	}

	// Crea buffer risposta
	iobuffer[0] = Dati.idBoard;
	iobuffer[1] = pdu[1];
	iobuffer[2] = quantity * 2;

	// Carica bytes risposta
	i = 3;

	while (quantity > 0)
	{
		if (Modbus_CanReadRegister(address) == 0)
		{
			iobuffer[i++] = (uint8_t) (dataPtr[address] >> 8);
			iobuffer[i++] = (uint8_t) (dataPtr[address] & 0xFF);
			address++;
			quantity--;
		}
		else
		{
			// Termina lettura per eccezione
			quantity = 0;
		}
	}

	// Calcola CRC sul pacchetto da inviare
	crc = Crc16(iobuffer, i);
	iobuffer[i++] = (uint8_t) (crc >> 8);
	iobuffer[i++] = (uint8_t) (crc & 0xFF);

	// Invia risposta
	if (Modbus_Send(iobuffer, i) != 0)
	{
		return EXCEPTION_SERVER_DEVICE_FAILURE;
	}



	return EXCEPTION_NONE;
}




/** Metodo interno. Scrive valore registro all'interno della struttura.*/
static int writeSingleRegister(uint8_t *pdu, uint16_t size, tDataTable *dataTable)
{
	uint16_t *dataPtr;
	uint16_t address, data;
	uint8_t reg;
	uint32_t tmpUInt32;

	// Verifica tabella
	if ((dataTable == NULL) || (dataTable->data == NULL)
			|| (dataTable->size == 0)) {
		return EXCEPTION_ILLEGAL_DATA_VALUE;
	}
	else
	{
		dataPtr = dataTable->data;
	}

	// Decodifica indirizzo
	address = pdu[2];
	address <<= 8;
	address += pdu[3];
	if (address >= dataTable->size / 2)
	{
		return EXCEPTION_ILLEGAL_DATA_ADDRESS;
	}

	// Verifica indirizzo e quantità richiesta
	if ((address + 1) > dataTable->size / 2)
	{
		return EXCEPTION_ILLEGAL_DATA_ADDRESS;
	}

	// Imposta valore
	data = pdu[4];
	data <<= 8;
	data += pdu[5];
	// Se abilitato alla scrittura, imposta valore
	if (Modbus_CanWriteRegister(address, data) == 0)
	{
		dataPtr[address] = data;


		switch (address)
		{
			case (REGISTER_DATETIME_DD_MM):
				Dati.dateTimeDDMM = data;
			break;

			case (REGISTER_DATETIME_YY_H):
				Dati.dateTimeYYH = data;
			break;

			case (REGISTER_DATETIME_M_S):

				Dati.dateTimeMS = data;

				RTC_SetDateTime( (Dati.dateTimeDDMM >> 4) & 0xff, Dati.dateTimeDDMM & 0x0f,
						(Dati.dateTimeYYH >> 8) & 0xff, Dati.dateTimeYYH & 0xff,
						(Dati.dateTimeMS >> 8) & 0xff, 0);
			break;


			case (REGISTER_PROG_1_DETAILS):
			case (REGISTER_PROG_2_DETAILS):
			case (REGISTER_PROG_3_DETAILS):
			case (REGISTER_PROG_4_DETAILS):
			case (REGISTER_PROG_5_DETAILS):
			case (REGISTER_PROG_6_DETAILS):
			case (REGISTER_PROG_7_DETAILS):
			case (REGISTER_PROG_8_DETAILS):
			case (REGISTER_PROG_9_DETAILS):
			case (REGISTER_PROG_10_DETAILS):
			case (REGISTER_PROG_11_DETAILS):
			case (REGISTER_PROG_12_DETAILS):
			case (REGISTER_PROG_13_DETAILS):
			case (REGISTER_PROG_14_DETAILS):

				reg = (address - REGISTER_PROG_1_DETAILS);
				Parameters.registerProgDetails[reg] = Dati.registers[address];

				// Salverà la struttura (Dati_Save()) alla scrittura del registro REGISTER_PROG_x_STOP
			break;


			case (REGISTER_PROG_1_STOP):
			case (REGISTER_PROG_2_STOP):
			case (REGISTER_PROG_3_STOP):
			case (REGISTER_PROG_4_STOP):
			case (REGISTER_PROG_5_STOP):
			case (REGISTER_PROG_6_STOP):
			case (REGISTER_PROG_7_STOP):
			case (REGISTER_PROG_8_STOP):
			case (REGISTER_PROG_9_STOP):
			case (REGISTER_PROG_10_STOP):
			case (REGISTER_PROG_11_STOP):
			case (REGISTER_PROG_12_STOP):
			case (REGISTER_PROG_13_STOP):
			case (REGISTER_PROG_14_STOP):

				reg = (address - REGISTER_PROG_1_STOP) / 2;
				Parameters.registerProgStart[reg] = Dati.registers[address - 1];
				Parameters.registerProgStop[reg] = Dati.registers[address];

				Dati_Save();
			break;




			case (REGISTER_STERILIZER_PROG_1_STOP):
			case (REGISTER_STERILIZER_PROG_2_STOP):
			case (REGISTER_STERILIZER_PROG_3_STOP):
			case (REGISTER_STERILIZER_PROG_4_STOP):
			case (REGISTER_STERILIZER_PROG_5_STOP):
			case (REGISTER_STERILIZER_PROG_6_STOP):
			case (REGISTER_STERILIZER_PROG_7_STOP):
			case (REGISTER_STERILIZER_PROG_8_STOP):
			case (REGISTER_STERILIZER_PROG_9_STOP):
			case (REGISTER_STERILIZER_PROG_10_STOP):
			case (REGISTER_STERILIZER_PROG_11_STOP):
			case (REGISTER_STERILIZER_PROG_12_STOP):
			case (REGISTER_STERILIZER_PROG_13_STOP):
			case (REGISTER_STERILIZER_PROG_14_STOP):

				reg = (address - REGISTER_STERILIZER_PROG_1_STOP) / 2;
				Parameters.registerSterilizerProgStart[reg] = Dati.registers[address - 1];
				Parameters.registerSterilizerProgStop[reg] = Dati.registers[address];

				Dati_Save();
			break;


			case (REGISTER_PARTIAL_HOUR_METERS_L):

				// Quando ricevo REGISTER_PARTIAL_HOUR_METERS_L devo già aver ricevuto REGISTER_PARTIAL_HOUR_METERS_H

				tmpUInt32 = (Dati.registers[REGISTER_PARTIAL_HOUR_METERS_H] << 16) + Dati.registers[REGISTER_PARTIAL_HOUR_METERS_L];
				if (tmpUInt32 != Parameters.partialHourMeters)
				{
					Parameters.partialHourMeters = tmpUInt32;
					Dati_Save();
				}
			break;

			default:
			break;
		}


	}
	else
		return EXCEPTION_NAK;

	// Eco del comando ricevuto
	if (Modbus_Send(pdu, size) != 0)
	{
		return EXCEPTION_SERVER_DEVICE_FAILURE;
	}

	return EXCEPTION_NONE;
}



char str[32];

int Modbus_OnPacketReceive(void *block, int len)
{
	if (len > MODBUS_BUFFER_SIZE)
		return 1;

	//Led_RxFlash(20);
	LED_PC1_Toggle();


	memcpy(Dati.modbusBuffer, (uint8_t *)block, len);
	Dati.modbusBufferLen = len;

/*
	sprintf(str, " ");
	for (int w=0; w<len; w++)
	{
		sprintf(str, "%s%u ", str, Dati.modbusBuffer[w]);

	}

	sprintf(str, "%s\r\n", str);
	USB_Send((uint8_t *)str, strlen(str));
*/
	return 0;
}


/** Metodo interno. Gestisce le risposte al master, rispettando i tempi di ritardo necessari */
int Modbus_ResponseManagement(void)
{

	uint16_t crc;
	uint8_t *src;
	uint8_t len;
	int res;

	src = (uint8_t *)Dati.modbusValidRequest;
	len = Dati.modbusValidRequestLen;

	// Nessuna richiesta pendente?
	if (Dati.modbusWaitForResponse == 0)
		return -1;

	// Attesa prima di rispondere rispettata?
	if (--Dati.modbusWaitForResponse > 0)
		return -1;


	// Verifica validità lunghezza
	if (len == 0)
		return -1;


	// Decodifica comando ricevuto
	switch (src[1])
	{

		case FUNCTION_READ_INPUT_REGISTERS:
			res = read16bitRegisters(src, len, &tableInputRegisters);
		break;

		case FUNCTION_WRITE_SINGLE_REGISTER:
			res = writeSingleRegister(src, len, &tableInputRegisters);
		break;

		default:
			// Codice non supportato
			res = EXCEPTION_ILLEGAL_FUNCTION;
		break;
	}

	switch (res)
	{
		// In caso di errori, invia pacchetto con codice eccezione
		case (EXCEPTION_ILLEGAL_FUNCTION):
		case (EXCEPTION_ILLEGAL_DATA_ADDRESS):
		case (EXCEPTION_ILLEGAL_DATA_VALUE):
		case (EXCEPTION_SERVER_DEVICE_FAILURE):
		case (EXCEPTION_NAK):
			iobuffer[0] = Dati.idBoard;
			iobuffer[1] = src[1] | 0x80;
			iobuffer[2] = res;

			crc = Crc16(iobuffer, 3);
			iobuffer[3] = (uint8_t) (crc >> 8);
			iobuffer[4] = (uint8_t) (crc & 0xFF);
			Modbus_Send(iobuffer, 6);
		break;

		case (EXCEPTION_NONE):
		default:
		break;
	}

	return res;

	return 0;
}



/* gestione pacchetto ModBus Cablato*/
int Modbus_Poll(void)
{
	uint8_t *src = (uint8_t *)Dati.modbusBuffer;
	uint8_t len;
	uint16_t packCrc;


	// Gestione eventuali richieste pendenti
	Modbus_ResponseManagement();


	// Pacchetto presente?
	if (Dati.modbusBufferLen == 0)
		return -1;

	len = Dati.modbusBufferLen;
	Dati.modbusBufferLen = 0;

	// Lunghezza valida?
	if (len < MIN_PACKET_LEN)
		return -1;

	// Se il pacchetto non è indirizzato a questa scheda e non si tratta di un pacchetto broadcast esco
	if (src[0] == Dati.idBoard)
		broadcast = FALSE;
	else if (src[0] == 0)
		broadcast = TRUE;
	else
		return -1;

	// Verifica validità crc
	packCrc = (src[len-2] << 8) + src[len-1];
	if (packCrc != Crc16((uint8_t *)Dati.modbusBuffer, len - 2))
		return -1;

	// Memorizza richiesta valida
	memcpy(Dati.modbusValidRequest, (uint8_t *)src, len);
	Dati.modbusValidRequestLen = len;
	Dati.modbusWaitForResponse = MODBUS_WAIT_FOR_RESPONSE;

	return 0;
}


