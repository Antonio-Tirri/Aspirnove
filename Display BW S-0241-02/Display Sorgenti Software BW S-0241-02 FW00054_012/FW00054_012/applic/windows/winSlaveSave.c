#include "windows.h"
#include "main.h"
#include "application.h"
#include "board.h"
#include "dati.h"
#include "img.h"
#include "modbusMaster.h"
#include "tastiera.h"
#include "debug.h"
#include "msg.h"



#define WINSLAVESAVE_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)

// Videate collegate
extern tWindow winMain;



static uint8_t refresh = TRUE;

static uint16_t contRun = 0;

static uint8_t sendIndex = 0;

/** Funzione di inizializzazione finestra */
static void load(void)
{
	refresh = TRUE;
	contRun = WINSLAVESAVE_MODBUS_REFRESH_PERIOD;
	sendIndex = 0;
} 

/** Funzione di disegno finestra */
static void draw(void)
{

	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();

		Windows_setfont(&BIG_FONT);
		sprintf(c_string, tabMsg[_SALVATAGGIO_]);
		Windows_Write(15, 30, c_string, NORMAL_MODE);


	}

}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{

	if (++contRun >= WINSLAVESAVE_MODBUS_REFRESH_PERIOD)
	{
		contRun = 0;

		switch (sendIndex)
		{
			case (0):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_SEASON, Dati.season);
			break;

			case (1):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_THERMOSTAT, Dati.thermostat);
			break;

			case (2):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_AIR_QUALITY, Dati.airQuality);
				Dati.registers[REGISTER_AIR_QUALITY] = Dati.airQuality;
			break;

			case (3):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_CO2_SETPOINT, Dati.co2SetPoint);
			break;

			case (4):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_ROOM_SETPOINT, Dati.roomSetPoint);
			break;

			case (5):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_FAN_2_FAN_1, (Dati.fan2 << 8) + Dati.fan1);
			break;

			case (6):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_MAX_FAN_MIN_FAN, (Dati.maxFan << 8) + Dati.minFan);
			break;

			case (7):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_STERILIZER, Dati.sterilizer);
				Dati.registers[REGISTER_STERILIZER] = Dati.sterilizer;
			break;

			case (8):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_ANTIFIRE, Dati.antifire);
				Dati.registers[REGISTER_ANTIFIRE] = Dati.antifire;
			break;

			case (9):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_HUMIDITY_SENSOR, Dati.humiditySensor);
				Dati.registers[REGISTER_HUMIDITY_SENSOR] = Dati.humiditySensor;
			break;

			case (10):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_WORK_MODE, Dati.slaveWorkMode);
				Dati.registers[REGISTER_WORK_MODE] = Dati.slaveWorkMode;
			break;

			case (11):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_HUMIDITY_SETPOINT, Dati.humiditySetPoint);
				Dati.registers[REGISTER_HUMIDITY_SETPOINT] = Dati.humiditySetPoint;
			break;

			case (12):
				ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_COMMAND, CMD_SAVE_PARAMETERS);
			break;

			default:
				Dati.modbusTimeout = MODBUS_TIMEOUT;
				Windows_Load(&winMain);
			break;
		}

		++sendIndex;

	}
}


/** Finestra */
const tWindow winSlaveSave = { load, draw, run, 0 };








