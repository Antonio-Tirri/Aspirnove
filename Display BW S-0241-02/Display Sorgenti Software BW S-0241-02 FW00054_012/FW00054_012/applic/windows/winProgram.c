#include "windows.h"
#include "main.h"
#include "application.h"
#include "board.h"
#include "dati.h"
#include "tastiera.h"
#include "msg.h"
#include "board.h"
#include "img.h"
#include "rtc.h"
#include "modbusMaster.h"




// Tempo visualizzazione messaggio di errore
#define TIME_SHOW_ERROR			(3000 / TIME_WINDOW_DRAW)	// 3 secondi


#define WINPROGRAM_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)

enum
{
	CURSOR_TIME_INIT_H = 0,
	CURSOR_TIME_INIT_M,
	CURSOR_TIME_END_H,
	CURSOR_TIME_END_M,

	CURSOR_MANAUTO,
	CURSOR_FAN1,
	CURSOR_FAN2,

	CURSOR_ESC,
	CURSOR_OK,
	
	CURSOR_NUM
};



// Videate collegate
extern tWindow winMenu;


uint8_t editMode;

static char cursor = CURSOR_TIME_INIT_H;

static unsigned char refresh = TRUE;

static unsigned short contPress = 0;

static uint8_t click_pressed = FALSE;

static uint16_t contSaveDateTime = 0;

static uint16_t contShowError = 0;

static uint8_t h1, m1, h2, m2;

static uint8_t man, fan1, fan2;



/** Funzione di inizializzazione finestra */
static void load(void)
{
	refresh = TRUE;
	click_pressed = FALSE;
	editMode = FALSE;

	contShowError = 0;
	contSaveDateTime = 0;
	cursor = CURSOR_TIME_INIT_H;

	if ( (Dati.currentProg < 1) || (Dati.currentProg > MAX_PROG) )
		Dati.currentProg = 1;

	h1 = (Dati.registers[REGISTER_PROG_1_START + ((Dati.currentProg - 1) * 2)] >> 8) & 0xff;
	m1 = Dati.registers[REGISTER_PROG_1_START + ((Dati.currentProg - 1) * 2)] & 0xff;
	h2 = (Dati.registers[REGISTER_PROG_1_STOP + ((Dati.currentProg - 1) * 2)] >> 8) & 0xff;
	m2 = Dati.registers[REGISTER_PROG_1_STOP + ((Dati.currentProg - 1) * 2)] & 0xff;

	man = (Dati.registers[REGISTER_PROG_1_DETAILS + (Dati.currentProg - 1)] & PROG_DETAILS_MANUAL)? 1 : 0;
	fan1 = Dati.registers[REGISTER_PROG_1_DETAILS + (Dati.currentProg - 1)] & 0x7f;
	fan2 = (Dati.registers[REGISTER_PROG_1_DETAILS + (Dati.currentProg - 1)] >> 7) & 0x7f;

} 

/** Funzione di disegno finestra */
static void draw(void)
{
	uint8_t mode;
	static uint16_t cont_draw = 0;
	
	Windows_setfont(&SMALL_FONT);
	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();

		if (contShowError == 0)
		{
			sprintf(c_string, "%s %s", tabMsg[_PROGRAM_], tabMsg[_MONDAY_P1_ + (Dati.currentProg - 1)]);
			Windows_Write(0, 9, c_string, NORMAL_MODE);

			sprintf(c_string, "%s:", tabMsg[_START_]);
			Windows_Write(15, 18, c_string, NORMAL_MODE);

			sprintf(c_string, "%s:", tabMsg[_STOP_]);
			Windows_Write(15, 27, c_string, NORMAL_MODE);

			sprintf(c_string, "%s:", tabMsg[_FAN_1_]);
			Windows_Write(15, 45, c_string, NORMAL_MODE);

			sprintf(c_string, "%s:", tabMsg[_FAN_2_]);
			Windows_Write(15, 54, c_string, NORMAL_MODE);
		}
		else
		{
			sprintf(c_string, "%s", tabMsg[_INVALID_]);
			Windows_CenteredWrite(25, c_string, NORMAL_MODE);

			sprintf(c_string, "%s", tabMsg[_TIME_]);
			Windows_CenteredWrite(43, c_string, NORMAL_MODE);
		}
	}


	if (contShowError)
	{
		if (--contShowError == 0)
			refresh = TRUE;

		return;
	}


	mode = (cursor == CURSOR_TIME_INIT_H)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", h1);
	Windows_Write(70, 18, c_string, mode);
	sprintf(c_string, ":");
	Windows_Write(85, 18, c_string, NORMAL_MODE);
	
	mode = (cursor == CURSOR_TIME_INIT_M)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", m1);
	Windows_Write(90, 18, c_string, mode);

	
	mode =  (cursor == CURSOR_TIME_END_H)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", h2);
	Windows_Write(70, 27, c_string, mode);
	sprintf(c_string, ":");
	Windows_Write(85, 27, c_string, NORMAL_MODE);
	
	mode = (cursor == CURSOR_TIME_END_M)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", m2);
	Windows_Write(90, 27, c_string, mode);

	mode = (cursor == CURSOR_MANAUTO)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
			mode = NORMAL_MODE;
	sprintf(c_string, "%s", man? tabMsg[_MANUAL_] : tabMsg[_AUTOMATIC_]);
	while (strlen(c_string) < 10)
		strcat(c_string, " ");
	Windows_CenteredWrite(36, c_string, mode);
	
	
	mode = (cursor == CURSOR_FAN1)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%3u ", fan1);
	Windows_Write(90, 45, c_string, mode);

	mode = (cursor == CURSOR_FAN2)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%3u ", fan2);
	Windows_Write(90, 54, c_string, mode);




	sprintf(c_string, "%s", tabMsg[_ESC_]);
	Windows_Write(0, 63, c_string, (cursor == CURSOR_ESC)? INVERSE_MODE : NORMAL_MODE);
	sprintf(c_string, "OK");
	Windows_Write(100, 63, c_string, (cursor == CURSOR_OK)? INVERSE_MODE : NORMAL_MODE);


	++cont_draw;
}


/** Metodo interno. Gestione incremento su pressione mantenuta del tasto */
static float gstRepeat(void)
{
	if (contPress % (50 / TIME_MAIN_CYCLE) == 0)
	{
		//if (contPress > (3000 / TIME_MAIN_CYCLE))
		//	return 10;
		//else
		if (contPress > (1000 / TIME_MAIN_CYCLE))
			return 1;
	}

	if (contPress == 0)
		return 1;

	return 0;
}


/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{
	uint16_t speed;
	uint8_t reg;

	if (contSaveDateTime)
	{
		if ((contSaveDateTime % WINPROGRAM_MODBUS_REFRESH_PERIOD) == 0)
		{
			switch (contSaveDateTime / WINPROGRAM_MODBUS_REFRESH_PERIOD)
			{
				default:
				break;

				case (1):
					reg = REGISTER_PROG_1_START  + ((Dati.currentProg - 1) * 2);
					Dati.registers[reg] = ((uint16_t)h1 << 8) + m1;
					ModbusMaster_WriteRegisters(Parameters.currentBoard, reg, Dati.registers[reg]);
				break;

				case (2):
					reg = REGISTER_PROG_1_DETAILS  + (Dati.currentProg - 1) ;

					Dati.registers[reg] = (man)? PROG_DETAILS_MANUAL : 0;
					Dati.registers[reg] += (fan2 & 0x7f) << 7;
					Dati.registers[reg] += fan1 & 0x7f;

					ModbusMaster_WriteRegisters(Parameters.currentBoard, reg, Dati.registers[reg]);
				break;

				case (3):
					reg = REGISTER_PROG_1_STOP  + ((Dati.currentProg - 1) * 2);
					Dati.registers[reg] = ((uint16_t)h2 << 8) + m2;
					ModbusMaster_WriteRegisters(Parameters.currentBoard, reg, Dati.registers[reg]);
				break;

				case (4):
					Windows_Load(&winMenu);
				break;

			}
		}

		++contSaveDateTime;
		return;
	}


	switch (cmd)
	{
		// Pressione tasto M
		case (TASTO_M | !TASTO_M_PREC):
			click_pressed = TRUE;
		break;

		
		// Rilascio tasto M
		case ((!TASTO_M) | TASTO_M_PREC):
			// M premuto in questa finestra?
			if (click_pressed)
			{
				click_pressed = TRUE;
				
				switch (cursor)
				{
					case (CURSOR_TIME_INIT_H):
					case (CURSOR_TIME_INIT_M):
					case (CURSOR_TIME_END_H):
					case (CURSOR_TIME_END_M):
					case (CURSOR_MANAUTO):
					case (CURSOR_FAN1):
					case (CURSOR_FAN2):
						editMode ^= 1;
						refresh = TRUE;
					break;
					
					case (CURSOR_OK):

						// Verifica validità orario
						if ( ((h2*60)+m2) < ((h1*60)+m1) )
						{
							if (contShowError == 0)
								contShowError = TIME_SHOW_ERROR;

							cursor = CURSOR_ESC;
							refresh = TRUE;
						}
						else
							contSaveDateTime = 1;

					break;

					case (CURSOR_ESC):
					default:
						Windows_Load(&winMenu);
					break;
				}
										
			}
		break;
		

		// Pressione tasto DOWN
		case (TASTO_DOWN | !TASTO_DOWN_PREC):
		case (TASTO_DOWN | TASTO_DOWN_PREC):

		speed = gstRepeat();


		if (editMode)
			{
				switch (cursor)
				{
					case (CURSOR_TIME_INIT_H):
						if (h1 > speed)
							h1 -= speed;
						else
							h1 = 0;
					break;

					case (CURSOR_TIME_INIT_M):

						if (m1 > speed)
							m1 -= speed;
						else
							m1 = 0;
					break;

					case (CURSOR_TIME_END_H):
						if (h2 > speed)
							h2 -= speed;
						else
							h2 = 0;
					break;

					case (CURSOR_TIME_END_M):
						if (m2 > speed)
							m2 -= speed;
						else
							m2 = 0;
					break;

					case (CURSOR_MANAUTO):
						if (!(cmd & TASTO_DOWN_PREC))
							man = !man;
					break;

					case (CURSOR_FAN1):
						if (fan1 > speed)
							fan1 -= speed;
						else
							fan1 = 0;
					break;

					case (CURSOR_FAN2):
						if (fan2 > speed)
							fan2 -= speed;
						else
							fan2 = 0;
					break;

					default:
					break;
				}
			}
			else
			{
				if (speed)
					if (++cursor >= CURSOR_NUM)
						cursor = CURSOR_NUM - 1;
			}
		break;

		// Pressione tasto UP
		case (TASTO_UP | !TASTO_UP_PREC):
		case (TASTO_UP | TASTO_UP_PREC):
			speed = gstRepeat();
			if (editMode)
			{

				switch (cursor)
				{
					case (CURSOR_TIME_INIT_H):
						h1 += speed;
						if (h1 > 23)
							h1 = 23;
					break;

					case (CURSOR_TIME_INIT_M):
						m1 += speed;
						if (m1 > 59)
							m1 = 59;
					break;

					case (CURSOR_TIME_END_H):
						h2 += speed;
						if (h2 > 23)
							h2 = 23;
					break;

					case (CURSOR_TIME_END_M):
						m2 += speed;
						if (m2 > 59)
							m2 = 59;
					break;


					case (CURSOR_MANAUTO):
						if (!(cmd & TASTO_UP_PREC))
							man = !man;
					break;

					case (CURSOR_FAN1):
						fan1 += speed;
						if (fan1 > 100)
							fan1 = 100;
					break;

					case (CURSOR_FAN2):
						fan2 += speed;
						if (fan2 > 100)
							fan2 = 100;
					break;


					default:
					break;
				}
			}
			else
			{
				if (speed)
					if (cursor)
						--cursor;
			}

		break;


		default:
		break;
	}
	
	// Pressione mantenuta
	if (cmd & (TASTO_UP | TASTO_DOWN))
		++contPress;
	else
		contPress = 0;

}


/** Finestra */
const tWindow winProgram = { load, draw, run, 0 };








