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



#define WINTACHO_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)

// Videate collegate
extern tWindow winMenu;



static uint8_t refresh = TRUE;

static uint16_t contRun = 0;

static uint8_t sendIndex = 0;

static uint16_t tachoFan1 = 0, tachoFan2 = 0;



/** Funzione di inizializzazione finestra */
static void load(void)
{
	refresh = TRUE;
	contRun = WINTACHO_MODBUS_REFRESH_PERIOD;
	sendIndex = 0;

	tachoFan1 = 0;
	tachoFan2 = 0;
} 

/** Funzione di disegno finestra */
static void draw(void)
{

	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();

		Windows_setfont(&SMALL_FONT);
		sprintf(c_string, tabMsg[_TACHO_FAN_]);
		Windows_Write(20, 10, c_string, NORMAL_MODE);

		sprintf(c_string, "%s:", tabMsg[_FAN_1_]);
		Windows_Write(0, 25, c_string, NORMAL_MODE);

		sprintf(c_string, "%s:", tabMsg[_FAN_2_]);
		Windows_Write(0, 43, c_string, NORMAL_MODE);

		sprintf(c_string, "%s", tabMsg[_ESC_]);
		Windows_Write(100, 64, c_string, INVERSE_MODE);
	}

	// Aggiorna valori tacho
	if (Dati.hardwareSlave != HARDWARE_S0242)
		sprintf(c_string, "%u Hz   ", tachoFan1);
	else
		sprintf(c_string, "-");
	Windows_Write(80, 25, c_string, NORMAL_MODE);


	if (Dati.hardwareSlave != HARDWARE_S0242)
		sprintf(c_string, "%u Hz   ", tachoFan2);
	else
		sprintf(c_string, "-");

	Windows_Write(80, 43, c_string, NORMAL_MODE);

}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{
	static uint16_t contRun = 0;

	if (++contRun >= WINTACHO_MODBUS_REFRESH_PERIOD)
	{
		contRun = 0;
		// Ogni X leggo valori tacho
		ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_TACHO_FAN_1, 2);
	}

	// Aggiorna variabili locali
	tachoFan1 = Dati.registers[REGISTER_TACHO_FAN_1];
	tachoFan2 = Dati.registers[REGISTER_TACHO_FAN_2];

	switch (cmd)
	{
		// Pressione tasto M
		case (TASTO_M | !TASTO_M_PREC):
				Windows_Load(&winMenu);
		break;

		default:
		break;
	}

}


/** Finestra */
const tWindow winTacho = { load, draw, run, 0 };








