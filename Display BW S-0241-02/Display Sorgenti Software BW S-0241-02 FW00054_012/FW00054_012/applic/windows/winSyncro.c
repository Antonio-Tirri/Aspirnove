#include "windows.h"
#include "main.h"
#include "application.h"
#include "tastiera.h"
#include "dati.h"
#include "board.h"
#include "modbusMaster.h"

static uint16_t contRun = 0;

static uint8_t sendRequest = 0;



#define WINSYNCRO_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)



// Videate collegate
extern tWindow winMenu;



/** Funzione di inizializzazione finestra */
static void load(void)
{
	Windows_Clear();
	contRun = WINSYNCRO_MODBUS_REFRESH_PERIOD;
	sendRequest = 0;
}

/** Funzione di disegno finestra */
static void draw(void)
{
}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{
	if (++contRun >= WINSYNCRO_MODBUS_REFRESH_PERIOD)
	{
		contRun = 0;

		switch (sendRequest)
		{
			case (0):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_SLAVE_VERSION, 1);
			break;

			case (1):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_PROG_1_START, 10);
			break;

			case (2):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_PROG_6_START, 10);
			break;

			case (3):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_PROG_11_START, 8);
			break;

			case (4):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_STERILIZER_PROG_1_START, 10);
			break;

			case (5):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_STERILIZER_PROG_6_START, 10);
			break;

			case (6):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_STERILIZER_PROG_11_START, 8);
			break;

			case (7):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_PROG_1_DETAILS, 10);
			break;

			case (8):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_PROG_11_DETAILS, 4);
			break;

			case (9):
				ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_PARTIAL_HOUR_METERS_H, 4);
			break;

			default:
				// Dati.currentMenu e Dati.level sono già stati impostati nella videata principale
				Windows_Load(&winMenu);
			break;
		}

		++sendRequest;

	}
	
		
}


/** Finestra */
const tWindow winSyncro = { load, draw, run, 0 };








