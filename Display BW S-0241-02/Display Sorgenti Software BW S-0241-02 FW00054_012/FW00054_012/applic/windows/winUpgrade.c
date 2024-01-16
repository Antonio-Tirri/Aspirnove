#include "windows.h"
#include "application.h"
#include "main.h"
#include "board.h"
#include "dati.h"
#include "tastiera.h"
#include "msg.h"
#include "board.h"
#include "watchdog.h"
#include "modbusMaster.h"
#include "maintimer.h"

enum
{
	WIN_UPGRADE_CURSOR_SI = 0,
	WIN_UPGRADE_CURSOR_NO,
	
	WIN_UPGRADE_CURSOR_NUM	
};



// Videate collegate
extern tWindow winMenu, winMain;


static char cursor = WIN_UPGRADE_CURSOR_NO;

static unsigned char refresh = TRUE;

static char cursorPrec;

static uint8_t click_pressed = FALSE;

static uint8_t contReturn = 0;

/** Funzione di inizializzazione finestra */
static void load(void)
{
	cursorPrec = 0xff;
	refresh = TRUE;
	click_pressed = FALSE;
	contReturn = 0;
} 

/** Funzione di disegno finestra */
static void draw(void)
{
	static uint16_t cont_draw = 0;
	
	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();
	}

	if (cursor != cursorPrec)
	{
		cursorPrec = cursor;

		Windows_setfont(&SMALL_FONT);
		sprintf(c_string, "%s", tabMsg[_CONFERMA_UPGRADE_]);
		Windows_Write(5, 15, c_string, NORMAL_MODE);

		Windows_setfont(&BIG_FONT);
		
		sprintf(c_string, "%s", tabMsg[_NO_]);
		Windows_Write(80, 55, c_string, (cursor == WIN_UPGRADE_CURSOR_NO)? INVERSE_MODE : NORMAL_MODE);
		
		sprintf(c_string, "%s", tabMsg[_SI_]);
		Windows_Write(20, 55, c_string, (cursor == WIN_UPGRADE_CURSOR_SI)? INVERSE_MODE : NORMAL_MODE);	
	}

	++cont_draw;
}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{

	switch (cmd)
	{
		case (TASTO_UP | !TASTO_UP_PREC):
			if (++cursor >= WIN_UPGRADE_CURSOR_NUM)
				cursor = WIN_UPGRADE_CURSOR_NUM - 1;
		break;

		case (TASTO_DOWN | !TASTO_DOWN_PREC):
			if (cursor)
			{
				--cursor;
			}
		break;

		// Pressione tasto ok
		case (TASTO_M | !TASTO_M_PREC):
			click_pressed = TRUE;
		break;

		
		// Rilascio tasto ok
		case ((!TASTO_M) | TASTO_M_PREC):
			// OK premuto in questa finestra?
			if (click_pressed)
			{		
				switch (cursor)
				{
					default:
					case (WIN_UPGRADE_CURSOR_NO):
						Windows_Load(&winMenu);	
					return;
					
					case (WIN_UPGRADE_CURSOR_SI):
						if (Dati.menuSlave)
						{
							ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_COMMAND, CMD_SLAVE_UPGRADE);
							contReturn = 5;// Attesa primo di tornare alla videata di lavoro (nella load viene reimpostato il baud rate. questa operazione può interrompere l'invio del pacchetto upgrade)
						}
						else
						{
							Board_RequestUpgrade();
							Watchdog_ForcedReset();
						}
					break;
				}										
			}
		break;
		
		default:
		break;
	}
	
	// Ritorno alla videata di lavoro dopo l'avvio del bootloader sulla scheda di controllo
	if (contReturn)
	{
		if (--contReturn == 0)
		{
			Windows_Load(&winMain);
		}
	}

}


/** Finestra */
const tWindow winUpgrade = { load, draw, run, 0 };








