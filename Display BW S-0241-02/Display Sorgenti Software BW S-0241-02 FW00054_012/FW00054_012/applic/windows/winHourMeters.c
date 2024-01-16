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



#define WINHOURMETERS_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)

// Videate collegate
extern tWindow winMenu;



static uint8_t refresh = TRUE;

static uint16_t contRun = 0;

static uint8_t click_pressed = FALSE;

static uint32_t totalHour = 0, partialHour = 0;

static uint16_t contOnPressed = 0;

static uint16_t contSaveDateTime = 0;


/** Funzione di inizializzazione finestra */
static void load(void)
{
	refresh = TRUE;
	contRun = WINHOURMETERS_MODBUS_REFRESH_PERIOD;

	click_pressed = FALSE;

	totalHour = 0;
	partialHour = 0;
} 

/** Funzione di disegno finestra */
static void draw(void)
{

	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();

		Windows_setfont(&SMALL_FONT);
		sprintf(c_string, tabMsg[_HOUR_METERS_]);
		Windows_Write(20, 10, c_string, NORMAL_MODE);

		sprintf(c_string, "%s", tabMsg[_TOTAL_HOURS_]);
		Windows_Write(0, 25, c_string, NORMAL_MODE);

		sprintf(c_string, "%s", tabMsg[_PARTIAL_HOURS_]);
		Windows_Write(0, 35, c_string, NORMAL_MODE);

		if (Dati.level == LEVEL_ADMIN)
		{
			sprintf(c_string, "%s", tabMsg[_PARTIAL_INFO_]);
			Windows_Write(0, 45, c_string, NORMAL_MODE);
		}

		sprintf(c_string, "%s", tabMsg[_ESC_]);
		Windows_Write(100, 64, c_string, INVERSE_MODE);
	}

	// Aggiorna valori tacho
	sprintf(c_string, "%lu ", totalHour);
	Windows_Write(80, 25, c_string, NORMAL_MODE);

	sprintf(c_string, "%lu ", partialHour);
	Windows_Write(80, 35, c_string, NORMAL_MODE);

}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{
	if (contSaveDateTime)
	{
		if ((contSaveDateTime % WINHOURMETERS_MODBUS_REFRESH_PERIOD) == 0)
		{
			switch (contSaveDateTime / WINHOURMETERS_MODBUS_REFRESH_PERIOD)
			{

				case (1):
					Dati.registers[REGISTER_PARTIAL_HOUR_METERS_H] = 0;
					ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_PARTIAL_HOUR_METERS_H, Dati.registers[REGISTER_PARTIAL_HOUR_METERS_H]);
				break;

				case (2):
					contSaveDateTime = 0;
					refresh = TRUE;
					Dati.registers[REGISTER_PARTIAL_HOUR_METERS_L] = 0;
					ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_PARTIAL_HOUR_METERS_L, Dati.registers[REGISTER_PARTIAL_HOUR_METERS_L]);
				break;

				default:
					contSaveDateTime = 0;
				break;
			}
		}

		if (contSaveDateTime != 0)
			++contSaveDateTime;
		return;
	}



	// Aggiorna variabili locali
	totalHour = (Dati.registers[REGISTER_TOTAL_HOUR_METERS_H] << 16) + Dati.registers[REGISTER_TOTAL_HOUR_METERS_L];
	partialHour = (Dati.registers[REGISTER_PARTIAL_HOUR_METERS_H] << 16) + Dati.registers[REGISTER_PARTIAL_HOUR_METERS_L];


	switch (cmd)
	{
		// Pressione tasto M
		case (TASTO_M | !TASTO_M_PREC):
			click_pressed = TRUE;
		break;


		// Rilascio tasto M
		case (TASTO_M_PREC):
			// M premuto in questa finestra?
			if (click_pressed)
			{
				Dati.currentMenu = MENU_SLAVE_MAIN;
				Windows_Load(&winMenu);
			}
		break;

		case (TASTO_ON | !TASTO_ON_PREC):
		case (TASTO_ON | TASTO_ON_PREC):
			if (Dati.level == LEVEL_ADMIN)
			{
				if (++contOnPressed >= (5000 / TIME_MAIN_CYCLE))
				{
					contOnPressed = 0;
					contSaveDateTime = 1;
				}
			}
		break;

		default:
			contOnPressed = 0;
		break;
	}

}


/** Finestra */
const tWindow winHourMeters = { load, draw, run, 0 };








