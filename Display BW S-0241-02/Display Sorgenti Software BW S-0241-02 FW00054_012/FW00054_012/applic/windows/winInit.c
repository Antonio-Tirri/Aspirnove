#include "windows.h"
#include "main.h"
#include "application.h"
#include "tim.h"
#include "tastiera.h"
#include "dati.h"
#include "img.h"
#include "board.h"
#include "watchdog.h"


// Durata visualizzazione finestra
#define TIME_VISUALIZZAZIONE_FINESTRA		(3 * (1000 / TIME_MAIN_CYCLE))		// 3 secondi


// Contatore visualizzazione finestra
static int cont_run;

// Contatore accesso menù
static int cont_menu;

// Videate collegate
extern tWindow winMain, winMenu;



/** Funzione di inizializzazione finestra */
static void load(void)
{
	
	Windows_Clear();
	
	switch (Parameters.Logo)
	{
		default:
		case (NO_LOGO):
			// Se Fillbox
			if (Parameters.WorkMode == MODE_FILLBOX)
			{
				Windows_setfont(&BIG_FONT);
				sprintf(c_string, "FillBox");
				Windows_CenteredWrite(32, c_string, NORMAL_MODE);
			}
			cont_run = TIME_VISUALIZZAZIONE_FINESTRA / 2;
			cont_menu = TIME_VISUALIZZAZIONE_FINESTRA / 2;
			break;

		case (ASPIRNOVA_LOGO):
			Windows_PutImg(0, 0,  (PGSYMBOL) &Logo, NORMAL_MODE);
			cont_run = 0;
			cont_menu = 0;

			// Se Fillbox
			if (Parameters.WorkMode == MODE_FILLBOX)
			{
				Windows_setfont(&SMALL_FONT);
				sprintf(c_string, "FillBox");
				Windows_CenteredWrite(63, c_string, NORMAL_MODE);
			}
			break;
	}





}

/** Funzione di disegno finestra */
static void draw(void)
{
	static uint8_t backlight = 0;
	
	// Attivazione graduale della retroilluminazione
	if (backlight < Parameters.DisplayBacklight)
	{
		backlight += 5;
		Windows_SetBacklight(backlight);
	}
}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{

	switch (cmd)
	{
		// Pressione tasto M
		case (TASTO_M | !TASTO_M_PREC):
		case (TASTO_M | TASTO_M_PREC):
			++cont_menu;
		break;
		
		default:
		break;
	}
	
	
	if (++cont_run >= TIME_VISUALIZZAZIONE_FINESTRA)
	{
		if (cont_menu == cont_run)
		{
			Dati.menuSlave = FALSE;
			Dati.level = LEVEL_ADMIN;
			Windows_Load(&winMenu);
		}
		else
		{
			Windows_Load(&winMain);
		}
	}
	
}


/** Finestra */
const tWindow winInit = { load, draw, run, 0 };








