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




// Tempo pressione encoder per accesso al menù
#define TIME_MENU			(4 * (1000 / TIME_MAIN_CYCLE))	// 4 secondi


#define WINOROLOGIO_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)

enum
{
	RTC_CURSOR_DD = 0,
	RTC_CURSOR_MM,
	RTC_CURSOR_YY,
	RTC_CURSOR_H,
	RTC_CURSOR_M,
	
	RTC_CURSOR_ESC,
	RTC_CURSOR_OK,
	
	RTC_CURSOR_NUM	
};



// Videate collegate
extern tWindow winMenu;


uint8_t editMode;

static char cursor = RTC_CURSOR_DD;

static unsigned char refresh = TRUE;

static unsigned short contPress = 0;

static uint8_t click_pressed = FALSE;

static uint16_t contSaveDateTime = 0;
static RTC_TimeTypeDef time;
static RTC_DateTypeDef date;


/** Funzione di inizializzazione finestra */
static void load(void)
{
	refresh = TRUE;
	click_pressed = FALSE;
	editMode = FALSE;

	contSaveDateTime = 0;
	cursor = RTC_CURSOR_DD;

	date.Date = (Dati.registers[REGISTER_DATETIME_DD_MM] >> 4) & 0xff;
	date.Month = Dati.registers[REGISTER_DATETIME_DD_MM] & 0x0f;
	date.Year = Dati.registers[REGISTER_DATETIME_YY_H] >> 8;
	time.Hours = Dati.registers[REGISTER_DATETIME_YY_H] & 0xff;
	time.Minutes = Dati.registers[REGISTER_DATETIME_M_S] >> 8;
	time.Seconds = Dati.registers[REGISTER_DATETIME_M_S] & 0xff;
} 

/** Funzione di disegno finestra */
static void draw(void)
{
	uint8_t mode;
	static uint16_t cont_draw = 0;
	
	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();
		
		Windows_setfont(&SMALL_FONT);
		sprintf(c_string, "%s:", tabMsg[_OROLOGIO_]);
		Windows_CenteredWrite(9, c_string, NORMAL_MODE);
	}

	Windows_setfont(&BIG_FONT);
	mode = (cursor == RTC_CURSOR_DD)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", date.Date);
	Windows_Write(16, 25, c_string, mode);
	sprintf(c_string, "/");
	Windows_Write(35, 25, c_string, NORMAL_MODE);
	
	mode = (cursor == RTC_CURSOR_MM)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", date.Month);
	Windows_Write(45, 25, c_string, mode);
	sprintf(c_string, "/");
	Windows_Write(65, 25, c_string, NORMAL_MODE);
	
	mode = (cursor == RTC_CURSOR_YY)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%04u ", 2000 + date.Year);
	Windows_Write(75, 25, c_string, mode);
	
	
	mode =  (cursor == RTC_CURSOR_H)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", time.Hours);
	Windows_Write(40, 45, c_string, mode);
	sprintf(c_string, ":");
	Windows_Write(60, 45, c_string, NORMAL_MODE);
	
	mode = (cursor == RTC_CURSOR_M)? INVERSE_MODE : NORMAL_MODE;
	if ((editMode) && ((cont_draw % 10) < 6))
		mode = NORMAL_MODE;
	sprintf(c_string, "%02u ", time.Minutes);
	Windows_Write(70, 45, c_string, mode);		
	
	
	sprintf(c_string, "%s", tabMsg[_ESC_]);
	Windows_Write(0, 64, c_string, (cursor == RTC_CURSOR_ESC)? INVERSE_MODE : NORMAL_MODE);
	sprintf(c_string, "OK");
	Windows_Write(100, 64, c_string, (cursor == RTC_CURSOR_OK)? INVERSE_MODE : NORMAL_MODE);		


	++cont_draw;
}


/** Metodo interno. Gestione incremento su pressione mantenuta del tasto */
static float gstRepeat(void)
{
	if (contPress % (50 / TIME_MAIN_CYCLE) == 0)
	{
		if (contPress > (3000 / TIME_MAIN_CYCLE))
			return 10;
		else if (contPress > (1000 / TIME_MAIN_CYCLE))
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

	if (contSaveDateTime)
	{
		if ((contSaveDateTime % WINOROLOGIO_MODBUS_REFRESH_PERIOD) == 0)
		{
			switch (contSaveDateTime / WINOROLOGIO_MODBUS_REFRESH_PERIOD)
			{
				default:
				break;

				case (1):
					Dati.registers[REGISTER_DATETIME_DD_MM] = (date.WeekDay << 12) + (date.Date << 4) + date.Month;
					ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_DATETIME_DD_MM, Dati.registers[REGISTER_DATETIME_DD_MM]);
				break;

				case (2):
					Dati.registers[REGISTER_DATETIME_YY_H] = (date.Year << 8) + time.Hours;
					ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_DATETIME_YY_H, Dati.registers[REGISTER_DATETIME_YY_H]);
				break;

				case (3):
					Dati.registers[REGISTER_DATETIME_M_S] = (time.Minutes << 8);
					ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_DATETIME_M_S, Dati.registers[REGISTER_DATETIME_M_S]);
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
					case (RTC_CURSOR_DD):
					case (RTC_CURSOR_MM):
					case (RTC_CURSOR_YY):
					case (RTC_CURSOR_H):
					case (RTC_CURSOR_M):
						editMode ^= 1;
						refresh = TRUE;
					break;
					
					case (RTC_CURSOR_OK):	
						contSaveDateTime = 1;
					break;

					case (RTC_CURSOR_ESC):
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
					case (RTC_CURSOR_DD):
						if (date.Date > speed)
							date.Date -= speed;
						else
							date.Date = 1;
					break;

					case (RTC_CURSOR_MM):

						if (date.Month > speed)
							date.Month -= speed;
						else
							date.Month = 1;
					break;

					case (RTC_CURSOR_YY):
						if (date.Year > speed)
							date.Year -= speed;
						else
							date.Year = 1;
					break;

					case (RTC_CURSOR_H):
						if (time.Hours > speed)
							time.Hours -= speed;
						else
							time.Hours = 0;
					break;

					case (RTC_CURSOR_M):
						if (time.Minutes > speed)
							time.Minutes -= speed;
						else
							time.Minutes = 0;
					break;

					default:
					break;
				}
			}
			else
			{
				if (speed)
					if (++cursor >= RTC_CURSOR_NUM)
						cursor = RTC_CURSOR_NUM - 1;
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
					case (RTC_CURSOR_DD):
						date.Date += speed;
						if (date.Date > 31)
							date.Date = 31;
					break;

					case (RTC_CURSOR_MM):
						date.Month += speed;
						if (date.Month > 12)
							date.Month = 12;
					break;

					case (RTC_CURSOR_YY):
						date.Year += speed;
						if (date.Year > 100)
							date.Year = 100;
					break;

					case (RTC_CURSOR_H):
						time.Hours += speed;
						if (time.Hours > 23)
							time.Hours = 23;
					break;

					case (RTC_CURSOR_M):
						time.Minutes += speed;
						if (time.Minutes > 59)
							time.Minutes = 59;
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
const tWindow winOrologio = { load, draw, run, 0 };








