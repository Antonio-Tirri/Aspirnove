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
#include "rtc.h"
#include "usart.h"


#define WINMAIN_MODBUS_REFRESH_PERIOD		(100 / TIME_MAIN_CYCLE)

#define WINMAIN_POWER_OFF					(40000 / TIME_MAIN_CYCLE)

#define WINMAIN_BACKLIGHT_STANDBY			1



// Videate collegate
extern tWindow winSyncro;



static uint8_t refresh = TRUE;

static int16_t contMenu = 0;

static uint16_t contRun = 0;

static uint8_t msg_err_showed;

static uint8_t displayIconExtInt = FALSE;

static uint16_t cont_draw = 0;

static uint16_t contPowerOff = 0;

static uint8_t contPollSlave = 0;

static uint8_t currentPollSlave = 1;

enum
{
	CURSOR_BOARD = 0,
	CURSOR_MODE,
	CURSOR_FAN1,
	CURSOR_FAN2,

	CURSOR_NUM
};


static char cursor = CURSOR_BOARD;
static uint8_t editMode = FALSE;
static uint16_t contPress = 0;



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



/** Funzione di inizializzazione finestra */
static void load(void)
{
	// Reinizializza le UART con il baud rate impostato
	MX_UART1_ReInit(Parameters.BaudRate);

	refresh = TRUE;
	contMenu = -1;

	Dati.registers[REGISTER_STATUS] = 0;

	Dati.registers[REGISTER_TEMP_EXT] = 0;
	Dati.registers[REGISTER_TEMP_INT] = 0;

	msg_err_showed = 0;
	displayIconExtInt = TRUE;

	contRun = WINMAIN_MODBUS_REFRESH_PERIOD;

	// Imposta a 1 per evitare refresh dei dati già al primo ciclo
	cont_draw = 1;

	contPowerOff = 0;

	Windows_SetBacklight(Parameters.DisplayBacklight);

	 cursor = CURSOR_BOARD;
	 editMode = FALSE;
} 

/** Funzione di disegno finestra */
static void draw(void)
{
	static uint8_t indexMsgAlarm = 0;
	static uint8_t contMsgAlarm = 0;
	uint16_t tmp;
	uint8_t mode;


	// Attivazione errore comunicazione o allarme antincendio
	if ( (((msg_err_showed & 0x01) == 0) &&  (Dati.modbusTimeout == 0)) ||
		 (((msg_err_showed & 0x02) == 0) &&   (Dati.antifireStatus & (1L << (Parameters.currentBoard-1))) ))
	{
		// Se non è appena stata caricata la videata
		if (refresh == FALSE)
		{
			refresh = TRUE;
			displayIconExtInt = FALSE;
		}
	}




	// Disattivazione errore comunicazione o allarme antincendio
	if ( (((msg_err_showed & 0x01)) &&  (Dati.modbusTimeout > 0)) ||
		 (((msg_err_showed & 0x02)) &&  ((Dati.antifireStatus & (1L << (Parameters.currentBoard-1))) == 0) ) )
	{
		// Se non è appena stata caricata la videata
		if (refresh == FALSE)
		{
			refresh = TRUE;
			// Entrambi disattivati?
			if ( (Dati.modbusTimeout > 0) && ((Dati.antifireStatus & (1L << (Parameters.currentBoard-1))) == 0) )
				displayIconExtInt = TRUE;
		}
	}


	if (refresh)
	{
		refresh = FALSE;		
		Windows_Clear();

		// Simboli EXT e INT
		if  (displayIconExtInt )
		{
			displayIconExtInt = FALSE;

			// Nella modalità Fillbox non ci sono questi simboli
			if (Parameters.WorkMode != MODE_FILLBOX)
			{
				// Icona EXT
				Windows_PutImg(0, 0,  (PGSYMBOL) &Ext, NORMAL_MODE);

				// Icona INT
				Windows_PutImg(0, 28,  (PGSYMBOL) &Int, NORMAL_MODE);
			}
		}
	}

	// Nessuna comunicazione con scheda corrente?
	if (Dati.modbusTimeout == 0)
	{
		displayIconExtInt = FALSE;
		// Errore comunicazione
		if ((cont_draw % 10) == 0)
		{
			if (cursor != CURSOR_BOARD)
			{
				cursor = CURSOR_BOARD;
				editMode = FALSE;
			}

			msg_err_showed |= 0x01;
			Windows_setfont(&BIG_FONT);
			sprintf(c_string, "%s", tabMsg[_COMMUNICATION_ERROR_1_]);
			Windows_CenteredWrite(32, c_string, NORMAL_MODE);
			sprintf(c_string, "%s", tabMsg[_COMMUNICATION_ERROR_2_]);
			Windows_CenteredWrite(48, c_string, NORMAL_MODE);
		}
	}

	// Antincendio attivo?
	else if (Dati.antifireStatus & (1L << (Parameters.currentBoard-1)) )
	{
		displayIconExtInt = FALSE;

		if ((cont_draw % 10) == 0)
		{
			if (cursor != CURSOR_BOARD)
			{
				cursor = CURSOR_BOARD;
				editMode = FALSE;
			}

			msg_err_showed = 0x02;
			Windows_setfont(&BIG_FONT);
			sprintf(c_string, "%s", tabMsg[_ANTIFIRE_MSG_1_]);
			Windows_CenteredWrite(32, c_string, NORMAL_MODE);
			sprintf(c_string, "%s", tabMsg[_ANTIFIRE_MSG_2_]);
			Windows_CenteredWrite(48, c_string, NORMAL_MODE);
		}
	}

	else
	{
		msg_err_showed = 0;
		displayIconExtInt = TRUE;

		if ((cont_draw % 10) == 0)
		{
			// Icona stagione
			Windows_PutImg(112, 0,  (Dati.registers[REGISTER_SEASON] == SEASON_SUMMER)?  (PGSYMBOL) &Sun : (PGSYMBOL) &Ice, NORMAL_MODE);

			Windows_setfont(&BIG_FONT);

			// Nella modalità Fillbox non c'è la temperatura esterna se la sonda non è connessa (temperatura corrisponde al valore minimo)
			if ((Parameters.WorkMode != MODE_FILLBOX) || ((int16_t)Dati.registers[REGISTER_TEMP_EXT] != -250) )
			{
				// Temperatura EXT
				sprintf(c_string, "%3d.%1u$ ", (int16_t)((int16_t)Dati.registers[REGISTER_TEMP_EXT] / 10), abs((int16_t)Dati.registers[REGISTER_TEMP_EXT] % 10));
			}
			else
				sprintf(c_string, "        ");

			Windows_Write(21, 12, c_string, NORMAL_MODE);



			// Nella modalità Fillbox non c'è la temperatura interna
			if (Parameters.WorkMode != MODE_FILLBOX)
			{
				// Temperatura INT
				sprintf(c_string, "%3d.%1u$", (int16_t)((int16_t)Dati.registers[REGISTER_TEMP_INT] / 10), abs((int16_t)Dati.registers[REGISTER_TEMP_INT] % 10));
				Windows_Write(21, 40, c_string, NORMAL_MODE);
			}

		}


		// TACHO
		if (Dati.tachoStatus & (1L << (Parameters.currentBoard-1)))
			sprintf(c_string, "!");
		else
			sprintf(c_string, " ");
		Windows_setfont(&BIG_FONT);
		Windows_Write(20, 30, c_string, NORMAL_MODE);


		Windows_setfont(&SMALL_FONT);

		// FAN 1
		if ( (cursor == CURSOR_FAN1) && (editMode) )	// Se modifica FAN1 in corso, visualizzo valore copia locale altrimenti quello proveniente dalla scheda slave
			tmp = Dati.fan1;
		else
			tmp = (Dati.registers[REGISTER_REAL_FAN_2_FAN_1] & 0xff);


		if ((cursor == CURSOR_FAN1) && (editMode) && ((cont_draw % 10) < 5) )
		{
			mode = NORMAL_MODE;
			sprintf(c_string, "       ");
		}
		else
		{
			mode = (cursor == CURSOR_FAN1)? INVERSE_MODE : NORMAL_MODE;
			if (mode == NORMAL_MODE)
				sprintf(c_string, "%u%%%s", tmp, (tmp > 99)? "  ": "   ");
			else
				sprintf(c_string, "%u%%%s", tmp, (tmp > 99)? "": ((tmp > 9)?" " : "  "));
		}

		// Nella modalità Fillbox visualizzo Fan1 più in basso
		if (Parameters.WorkMode == MODE_FILLBOX)
		{
			Windows_Write(28, 40, c_string, mode);
		}
		else
			Windows_Write(28, 21, c_string, mode);


		// Nella modalità Fillbox non c'è la fan2
		if (Parameters.WorkMode != MODE_FILLBOX)
		{
			// FAN 2
			if ( (cursor == CURSOR_FAN2) && (editMode) )	// Se modifica FAN2 in corso, visualizzo valore copia locale altrimenti quello proveniente dalla scheda slave
				tmp = Dati.fan2;
			else
				tmp = (Dati.registers[REGISTER_REAL_FAN_2_FAN_1] >> 8);

			if ((cursor == CURSOR_FAN2) && (editMode) && ((cont_draw % 10) < 5) )
			{
				mode = NORMAL_MODE;
				sprintf(c_string, "       ");
			}
			else
			{
				mode = (cursor == CURSOR_FAN2)? INVERSE_MODE : NORMAL_MODE;
				sprintf(c_string, "%u%%%s", tmp, (tmp > 99)? " ": "  ");
			}
			Windows_Write(28, 49, c_string, mode);
		}



		if ((cursor == CURSOR_MODE) && (editMode) && ((cont_draw % 10) < 5) )
		{
			mode = NORMAL_MODE;
			Windows_PutImg(112, 16,  (PGSYMBOL) &Vuota16x16, mode);
		}
		else
		{
			mode = (cursor == CURSOR_MODE)? INVERSE_MODE : NORMAL_MODE;

			// Modalità
			switch (Dati.registers[REGISTER_STATUS] & STATUS_MODE)
			{
				default:
				case (MODE_MANUAL):
					Windows_PutImg(112, 16,  (PGSYMBOL) &Man, mode);
				break;

				case (MODE_AUTO):
					Windows_PutImg(112, 16,  (PGSYMBOL) &Auto, mode);
				break;

				case (MODE_SCHEDULE):
					Windows_PutImg(112, 16,  (PGSYMBOL) &Schedule, mode);
				break;
			}
		}

		// ON/OFF
		Windows_PutImg(112, 48,  (Dati.registers[REGISTER_STATUS] & STATUS_ON)?  (PGSYMBOL) &Vuota16x16 : (PGSYMBOL) &ButtonOn, NORMAL_MODE);

		// Serpentina
		if (Parameters.DisplayHeater)
		{
			Windows_PutImg(64, 0, (PGSYMBOL)&Serpentina, NORMAL_MODE);
			sprintf(c_string, "%3u%%", Dati.registers[REGISTER_H2O_VALUE]);
			Windows_Write(80, 10, c_string, NORMAL_MODE);
		}

		// Humidity
		if (Dati.registers[REGISTER_HUMIDITY_SENSOR])
		{
			Windows_PutImg(64, 16, (PGSYMBOL)&Humidity, NORMAL_MODE);
			sprintf(c_string, "%3u", Dati.registers[REGISTER_HUMIDITY_VALUE]);
			Windows_Write(80, 26, c_string, NORMAL_MODE);
		}

		// CO2
		if (Dati.registers[REGISTER_AIR_QUALITY])
		{
			Windows_PutImg(64, 32, (PGSYMBOL)&Co2, NORMAL_MODE);
			sprintf(c_string, "%3u ", Dati.registers[REGISTER_CO2]);
			Windows_Write(80, 42, c_string, NORMAL_MODE);
		}


		// Icona sterilizzatore
		if (Dati.registers[REGISTER_STERILIZER])
		{
			if (Dati.registers[REGISTER_IO] & IO_RELE_AUX1)
				Windows_PutImg(96, 16,  (PGSYMBOL) &SterilizerOn, NORMAL_MODE);
			else
				Windows_PutImg(96, 16,  (PGSYMBOL) &SterilizerOff, NORMAL_MODE);
		}
		else
			Windows_PutImg(96, 16,  (PGSYMBOL) &Vuota16x16, NORMAL_MODE);




		// Nella modalità Fillbox non c'è il bypass
		if (Parameters.WorkMode != MODE_FILLBOX)
		{
			// BYPASS
			Windows_PutImg(96, 48,  (Dati.registers[REGISTER_IO] & IO_RELE_BYP)?  (PGSYMBOL) &Vuota16x16 : (PGSYMBOL) &BypassOff, NORMAL_MODE);
		}


		// TERMOSTATO
		Windows_PutImg(112, 32,  ((Dati.registers[REGISTER_THERMOSTAT]) && (Dati.registers[REGISTER_IO] & IO_DIN_TERMOST))?  (PGSYMBOL) &Termostato : (PGSYMBOL) &Vuota16x16, NORMAL_MODE);



		// FILTRO
		if ( (Dati.filterStatus & (1L << (Parameters.currentBoard-1))) && (Dati.fanStatus & (1L << (Parameters.currentBoard-1))))
		{
			Windows_PutImg(59, 48, (PGSYMBOL)&Bell, NORMAL_MODE);

		    sprintf(c_string,  "  %s     ", tabMsg[_REPLACE_AIR_FILTER_]);

		    if (++contMsgAlarm > 3)
		    {
		    	contMsgAlarm = 0;
		    	if (++indexMsgAlarm >= strlen(c_string))
		    		indexMsgAlarm = 0;
		    }
			c_string[indexMsgAlarm + 10] = ' ';
			c_string[indexMsgAlarm + 11] = 0;
		}
		else if (Dati.sanitizeTimeout[Parameters.currentBoard-1])
		{
			Windows_PutImg(59, 48, (PGSYMBOL)&Bell, NORMAL_MODE);

		    sprintf(c_string, "  %s      ", tabMsg[_SANITIZATION_ALARM_]);

		    if (++contMsgAlarm > 3)
		    {
		    	contMsgAlarm = 0;
		    	if (++indexMsgAlarm >= strlen(c_string))
		    		indexMsgAlarm = 0;
		    }
			c_string[indexMsgAlarm + 10] = ' ';
			c_string[indexMsgAlarm + 11] = 0;
		}
		else
		{
			Windows_PutImg(59, 48, (PGSYMBOL)&Vuota16x16, NORMAL_MODE);

			// Data/Ora
			sprintf(c_string, "%s %02u:%02u   ",
					RTC_GetDayName((Dati.registers[REGISTER_DATETIME_DD_MM] >> 12) & 0x0f),
					Dati.registers[REGISTER_DATETIME_YY_H] & 0xff, Dati.registers[REGISTER_DATETIME_M_S] >> 8 );

			contMsgAlarm = 0;
			indexMsgAlarm = 0;
		}

		Windows_Write(0, 63, &c_string[indexMsgAlarm], NORMAL_MODE);
	}

	// Numero macchina
	Windows_setfont(&SMALL_FONT);
	if ((cursor == CURSOR_BOARD) && (editMode) && ((cont_draw % 10) < 5) )
	{
		mode = NORMAL_MODE;
		sprintf(c_string, "    ");
	}
	else
	{
		mode = (cursor == CURSOR_BOARD)? INVERSE_MODE : NORMAL_MODE;
		sprintf(c_string, "#%u", Parameters.currentBoard);
		if (strlen(c_string) < 3)
			strcat(c_string, " ");
	}
	Windows_Write(76, 63, c_string, mode);



	++cont_draw;
}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{
	static uint8_t newMode = FALSE;
	static uint8_t changeBypass = FALSE;
	static uint8_t changeStatusOn = FALSE;
	static uint8_t modFan = FALSE;
	static uint8_t contRefresh = 0;
	static uint8_t waitReleaseAfterStandBy;
	uint16_t speed;



	// Standby
	if (cmd != 0)
	{
		if (contPowerOff == WINMAIN_POWER_OFF)
		{
			Windows_SetBacklight(Parameters.DisplayBacklight);
			waitReleaseAfterStandBy = TRUE;
		}

		contPowerOff = 0;
	}
	else
	{
		if (contPowerOff < WINMAIN_POWER_OFF)
		{
			if (++contPowerOff == WINMAIN_POWER_OFF)
			{
				Windows_SetBacklight(WINMAIN_BACKLIGHT_STANDBY);
			}
		}
	}

	if (waitReleaseAfterStandBy)
	{
		if (cmd == 0)
			waitReleaseAfterStandBy = FALSE;
		else
			cmd = 0;
	}



	switch (cmd)
	{
		// Pressione tasto ON
		case (TASTO_ON | !TASTO_ON_PREC):
			// Se sono in manuale o in automatico
			if ( ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_MANUAL) ||
				 ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_AUTO) )
			{
				changeStatusOn = TRUE;
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
					case (CURSOR_BOARD):
						if (Parameters.currentBoard > speed)
							Parameters.currentBoard -= speed;
						else
							Parameters.currentBoard = 1;
					break;

					case (CURSOR_MODE):
						if (speed)
							newMode = TRUE;
					break;


					case (CURSOR_FAN1):

						if (speed)
						{
							if (Dati.fan1 > speed)
								Dati.fan1 -= speed;
							else
								speed = 0;

							if (Dati.fan1 < Dati.minFan)
							{
								Dati.fan1 = Dati.minFan;
							}

							modFan = TRUE;
						}
					break;


					case (CURSOR_FAN2):
						if (speed)
						{
							if (Dati.fan2 > speed)
								Dati.fan2 -= speed;
							else
								speed = 0;

							if (Dati.fan2 < Dati.minFan)
							{
								Dati.fan2 = Dati.minFan;
							}

							modFan = TRUE;
						}
					break;


					default:
					break;
				}
			}
			else
			{
				if (speed)
				{
					if (++cursor >= CURSOR_NUM)
						cursor = 0;

					// Se non sono in manuale non posso modificare le percentuali ventole
					if ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) != MODE_MANUAL)
					{
						if ( (cursor == CURSOR_FAN1) || (cursor == CURSOR_FAN2) )
							cursor = CURSOR_MODE;
					}

					// Nella modalità Fillbox non c'è la ventola 2
					if (Parameters.WorkMode == MODE_FILLBOX)
					{
						if (cursor == CURSOR_FAN2)
							cursor = CURSOR_BOARD;
					}
				}
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
					case (CURSOR_BOARD):
						Parameters.currentBoard += speed;
						if (Parameters.currentBoard > Parameters.ControlBoards)
							Parameters.currentBoard = Parameters.ControlBoards;
					break;

					case (CURSOR_MODE):
						if (speed)
							newMode = TRUE;
					break;

					case (CURSOR_FAN1):
						if (speed)
						{
							Dati.fan1 += speed;
							if (Dati.fan1 > Dati.maxFan)
							{
								Dati.fan1 = Dati.maxFan;
							}

							modFan = TRUE;
						}

					break;

					case (CURSOR_FAN2):
						if (speed)
						{
							Dati.fan2 += speed;
							if (Dati.fan2 > Dati.maxFan)
							{
								Dati.fan2 = Dati.maxFan;
							}

							modFan = TRUE;
						}
					break;

					default:
					break;
				}
			}
			else
			{
				if (speed)
				{
					if (cursor)
						--cursor;
					else
						cursor = CURSOR_NUM - 1;

					// Se non sono in manuale non posso modificare le percentuali ventole
					if ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) != MODE_MANUAL)
					{
						if ( (cursor == CURSOR_FAN1) || (cursor == CURSOR_FAN2) )
							cursor = CURSOR_MODE;
					}

					// Nella modalità Fillbox non c'è la ventola 2
					if (Parameters.WorkMode == MODE_FILLBOX)
					{
						if (cursor == CURSOR_FAN2)
							cursor = CURSOR_FAN1;
					}
				}
			}

		break;

		// Pressione tasto M
		case (TASTO_M | !TASTO_M_PREC):
			contMenu = 0;
		break;

		// Pressione mantenuta tasto M
		case (TASTO_M | TASTO_M_PREC):
		case (TASTO_M | TASTO_BP | TASTO_M_PREC | TASTO_BP_PREC):
		// Inizio pressione avvenuta in questa videata
			if (contMenu >= 0)
			{
				// Errore comunicazione presente? --> disabilito accesso menu
				if (Dati.modbusTimeout == 0)
					contMenu = 1;

				++contMenu;

				if (cmd & TASTO_BP)
				{
					if (contMenu > (5000 / TIME_MAIN_CYCLE) )
					{
						contMenu = 0;
						Dati.menuSlave = TRUE;

						Dati.level = LEVEL_ADMIN;
						Windows_Load(&winSyncro);
						return;
					}
				}
				else
				{
					if (contMenu > (2000 / TIME_MAIN_CYCLE) )
					{
						contMenu = 0;
						Dati.menuSlave = TRUE;

						Dati.level = LEVEL_USER;
						Windows_Load(&winSyncro);
						return;
					}
				}
			}
		break;

		// Rilascio tasto M
		case ((!TASTO_M) | TASTO_M_PREC):
			// Inizio pressione avvenuta in questa videata
			if (contMenu >= 0)
			{
				if (contMenu < (1000 / TIME_MAIN_CYCLE))
				{
					editMode ^= 1;

					if (editMode)
					{
						Dati.fan1 = Dati.registers[REGISTER_FAN_2_FAN_1] & 0xff;
						Dati.fan2 = Dati.registers[REGISTER_FAN_2_FAN_1] >> 8;
						Dati.maxFan = Dati.registers[REGISTER_MAX_FAN_MIN_FAN] >> 8;
						Dati.minFan = Dati.registers[REGISTER_MAX_FAN_MIN_FAN] & 0xff;
					}
					else
					{
						if (cursor == CURSOR_BOARD)
						{
							load();	 // Reload
						}


					}
				}

				//newMode = TRUE;
			}
		break;

		// Pressione tasto BP
		case (TASTO_BP | !TASTO_BP_PREC):

			// Nella modalità Fillbox non c'è il bypass
			if (Parameters.WorkMode != MODE_FILLBOX)
			{
				changeBypass = TRUE;
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


	if (++contRun >= WINMAIN_MODBUS_REFRESH_PERIOD)
	{
		contRun = 0;

		if ( (modFan) && ((contRefresh % 2) == 0) )	// In caso di pressione mantenuta del tasto, modFan viene sempre attivato. Con il controllo su contRefresh alterno l'aggiornamento in modo da continuare a leggere i registri e non andare in errore di comunicazione.
		{
			modFan = FALSE;
			ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_FAN_2_FAN_1, (Dati.fan2 << 8) + Dati.fan1);
		}
		else if ((newMode) && ((contRefresh % 2) == 0) )	// In caso di pressione mantenuta del tasto, newMode viene sempre attivato. Con il controllo su contRefresh alterno l'aggiornamento in modo da continuare a leggere i registri e non andare in errore di comunicazione.
		{
			newMode = FALSE;
			if ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_MANUAL)
			{
				Dati.registers[REGISTER_STATUS] &= ~STATUS_MODE;
				// Nella modalità Fillbox non viene gestita la modalità AUTO
				if (Parameters.WorkMode == MODE_FILLBOX)
					Dati.registers[REGISTER_STATUS] |= MODE_SCHEDULE;
				else
					Dati.registers[REGISTER_STATUS] |= MODE_AUTO;
			}
			else if ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_AUTO)
			{
				Dati.registers[REGISTER_STATUS] &= ~STATUS_MODE;
				Dati.registers[REGISTER_STATUS] |= MODE_SCHEDULE;
			}
			else
			{
				Dati.registers[REGISTER_STATUS] &= ~STATUS_MODE;
				Dati.registers[REGISTER_STATUS] |= MODE_MANUAL;
			}

			ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_STATUS, Dati.registers[REGISTER_STATUS]);
		}
		else if (changeBypass)
		{
			changeBypass = FALSE;
			ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_COMMAND, CMD_BYPASS);
		}
		else if (changeStatusOn)
		{
			changeStatusOn = FALSE;
			ModbusMaster_WriteRegisters(Parameters.currentBoard, REGISTER_COMMAND, CMD_ON_OFF);
		}
		else
		{
			// Ogni 10 * WINMAIN_MODBUS_REFRESH_PERIOD interrogo un altro slave per recuperarne lo stato del filtro
			if (++contPollSlave >= 10)
			{
				contPollSlave = 0;
				ModbusMaster_RequestRegisters(currentPollSlave, REGISTER_STATUS, 1);

				if (++currentPollSlave > Parameters.ControlBoards)
					currentPollSlave = 1;
			}
			else
			{
				switch (contPollSlave)
				{
					case (1):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_DATETIME_DD_MM, 3);	break;
					case (2):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_IO, 4);	break;
					case (3):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_TEMP_EXT, 3);	break;
					case (4):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_STATUS, 3);	break;
					case (5):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_AIR_QUALITY, 3);	break;
					case (6):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_FAN_2_FAN_1, 3);	break;
					case (7):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_STERILIZER, 3);	break;
					case (8):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_HUMIDITY_VALUE, 4);	break;
					case (9):	ModbusMaster_RequestRegisters(Parameters.currentBoard, REGISTER_HUMIDITY_SETPOINT, 1);	break;

				}
			}
		}



		++contRefresh;
	}


}


/** Finestra */
const tWindow winMain = { load, draw, run, 0 };








