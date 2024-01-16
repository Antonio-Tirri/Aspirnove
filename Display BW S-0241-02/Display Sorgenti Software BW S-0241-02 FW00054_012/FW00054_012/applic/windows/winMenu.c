
#include "windows.h"
#include "main.h"
#include "application.h"
#include "board.h"
#include "GMenu.h"
#include "GCombo.h"
#include "GEdit.h"
#include "tastiera.h"
#include "dati.h"
#include "msg.h"


// Time out finestra
#define TIME_OUT_FINESTRA		(60000 / TIME_MAIN_CYCLE)



// Videate collegate
extern tWindow winUpgrade, winSlaveSave, winMain, winOrologio, winProgram, winProgramSterilizer, winTacho, winHourMeters;



static unsigned short cont_run = 0;

static char strDays[7 * 2][24];

int menuLoad(unsigned int mn)
{
	int i = 0;
	
	cont_run = 0;
	Dati.currentMenu = mn;
	
	switch (mn)
	{

		case (MENU_MASTER_MAIN):
			sprintf(c_string, "MASTER %s", APP_VERSION);

			// Versione hardware display
			switch (Board_GetHWRev())
			{
				default:
				case (HARDWARE_2019):
					strcat(c_string, "-2019");
				break;

				case (HARDWARE_2020):
					strcat(c_string, "-2020");
				break;
			}
			GMenu_AddMono(c_string, 0, 0, 0, 0, 0, 0, 0, i++, 1);	// Titolo

			GMenu_AddMono(tabMsg[_ESC_], 0, 0, 0, 0, 0, 0, 0, i++, 1);		
			GMenu_AddUShort(tabMsg[_SCHEDE_CONTROLLO_], "%2u%", 1, CONTROL_BOARDS_MAX, 1, (uint16_t *)&Parameters.ControlBoards, 0, 1, i++, 1);
			GMenu_AddCombo(tabMsg[_LINGUA_], 0, _ITALIANO_, NUMERO_LINGUE, 1, (uint16_t *)&Parameters.Language, 0, 1, i++, 1);
			GMenu_AddMono(tabMsg[_DISPLAY_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddCombo(tabMsg[_LOGO_], 0, _LOGO_NO_, LOGOS_NUMS, 1, (uint16_t *)&Parameters.Logo, 0, 1, i++, 1);
			GMenu_AddCombo(tabMsg[_BUZZER_], 0, _OFF_, 2, 1, (uint16_t *)&Parameters.Buzzer, 0, 1, i++, 1);
			GMenu_AddCombo(tabMsg[_BAUD_RATE_], 0, _BAUD_RATE_1_, BAUD_RATE_NUM, 1, (uint16_t *)&Parameters.BaudRate, 0, 1, i++, 1);
			GMenu_AddCombo(tabMsg[_DISPLAY_HEATER_], 0, _NO_, 2, 1, (uint16_t *)&Parameters.DisplayHeater, 0, 1, i++, 1);
			GMenu_AddCombo(tabMsg[_MODALITA_], 0, _STANDARD_, MODE_NUMS, 1, (uint16_t *)&Parameters.WorkMode, 0, 1, i++, 1);
			GMenu_AddMono(tabMsg[_FIRMWARE_UPGRADE_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddMono(0, 0, 0, 0, 0, "", 0, 0, i++, 0);	 // Fine 
		break;
					

		case (MENU_DISPLAY):
			GMenu_AddMono(tabMsg[_DISPLAY_],0,0,0,0,0,0,0,i++,1);	// Titolo 		

			GMenu_AddShort(tabMsg[_ILLUMINAZIONE_],"%3u%", 0, 100, 1, (uint16_t *)&(Parameters.DisplayBacklight), 0, 1,  i++, 1);
			GMenu_AddShort(tabMsg[_CONTRASTO_],"%2u%", 0, 25, 1, (uint16_t *)&(Parameters.DisplayContrast), 0, 1,  i++, 1);
			GMenu_AddMono(tabMsg[_ESC_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
		
			GMenu_AddMono(0, 0, 0, 0, 0, "", 0, 0, i++, 0);		// Fine 	
		break;


		case (MENU_SLAVE_MAIN):
			sprintf(c_string, "SLAVE %03u", Dati.registers[REGISTER_SLAVE_VERSION] & 0xff);

			Dati.hardwareSlave = (Dati.registers[REGISTER_SLAVE_VERSION] >> 8) & 0xff;

			// Versione hardware
			switch (Dati.hardwareSlave)
			{
				default:
				case (HARDWARE_S0242):
					strcat(c_string, "-S0242");
				break;

				case (HARDWARE_S0283):
					strcat(c_string, "-S0283");
				break;
			}


			GMenu_AddMono(c_string, 0, 0, 0, 0, 0, 0, 0, i++, 1);	// Titolo

			GMenu_AddMono(tabMsg[_ESC_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddUShort(tabMsg[_FAN_1_], "%5d%", 0, 100, 1, (uint16_t *)&Dati.fan1, 0, 1, i++, 1);
			GMenu_AddUShort(tabMsg[_FAN_2_], "%5d%", 0, 100, 1, (uint16_t *)&Dati.fan2, 0, 1, i++, 1);
			GMenu_AddCombo(tabMsg[_SEASON_], 0, _WINTER_, 2, 1, (uint16_t *)&Dati.season, 0, 1, i++, 1);
			GMenu_AddShort(tabMsg[_ROOM_SETPOINT_], "%5d%", -20, 80, 1, (int16_t *)&Dati.roomSetPoint, 0, 1, i++, 1);
			GMenu_AddUShort(tabMsg[_CO2_SETPOINT_], "%5d%", 0, 5000, 1, (uint16_t *)&Dati.co2SetPoint, 0, 1, i++, 1);
			GMenu_AddUShort(tabMsg[_HUM_SETPOINT_], "%3d%", 0, 100, 1, (uint16_t *)&Dati.humiditySetPoint, 0, 1, i++, 1);
			GMenu_AddMono(tabMsg[_PROGRAMS_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddMono(tabMsg[_OROLOGIO_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddMono(tabMsg[_TACHO_FAN_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddMono(tabMsg[_STERILIZER_CALENDAR_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			GMenu_AddMono(tabMsg[_HOUR_METERS_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			if (Dati.level == LEVEL_ADMIN)
			{
				GMenu_AddCombo(tabMsg[_THERMOSTAT_], 0, _NO_, 2, 1, (uint16_t *)&Dati.thermostat, 0, 1, i++, 1);
				GMenu_AddCombo(tabMsg[_AIR_QUALITY_], 0, _NO_AIR_QUALITY_, 3, 1, (uint16_t *)&Dati.airQuality, 0, 1, i++, 1);
				GMenu_AddUShort(tabMsg[_MIN_FAN_], "%5d%", 0, 100, 1, (uint16_t *)&Dati.minFan, 0, 1, i++, 1);
				GMenu_AddUShort(tabMsg[_MAX_FAN_], "%5d%", 0, 100, 1, (uint16_t *)&Dati.maxFan, 0, 1, i++, 1);

				GMenu_AddCombo(tabMsg[_STERILIZER_], 0, _NO_, 2, 1, (uint16_t *)&Dati.sterilizer, 0, 1, i++, 1);
				GMenu_AddCombo(tabMsg[_ANTIFIRE_], 0, _NO_, 2, 1, (uint16_t *)&Dati.antifire, 0, 1, i++, 1);
				GMenu_AddCombo(tabMsg[_HUMIDITY_SENSOR_], 0, _NO_, 2, 1, (uint16_t *)&Dati.humiditySensor, 0, 1, i++, 1);
				GMenu_AddCombo(tabMsg[_MODALITA_], 0, _STANDARD_, MODE_NUMS, 1, (uint16_t *)&Dati.slaveWorkMode, 0, 1, i++, 1);
				GMenu_AddMono(tabMsg[_SLAVE_FIRMWARE_UPGRADE_], 0, 0, 0, 0, 0, 0, 0, i++, 1);
			}
			GMenu_AddMono(0, 0, 0, 0, 0, "", 0, 0, i++, 0);	 // Fine
		break;

		case (MENU_PROGRAMS):
			GMenu_AddMono(tabMsg[_DISPLAY_],0,0,0,0,0,0,0,i++,1);	// Titolo

			// Prapara le stringhe con gli orari dei programmi
			for (int w=0; w<MAX_PROG; w++)
				sprintf(strDays[w], "%s %02u:%02u %02u:%02u", (Dati.registers[REGISTER_PROG_1_DETAILS + w] & 0x8000)?"MAN " : "AUTO ", (Dati.registers[REGISTER_PROG_1_START + (w*2)] >> 8) & 0xff, Dati.registers[REGISTER_PROG_1_START + (w*2)] & 0xff,
						(Dati.registers[REGISTER_PROG_1_STOP + (w*2)] >> 8) & 0xff, Dati.registers[REGISTER_PROG_1_STOP + (w*2)] & 0xff);

			GMenu_AddString(tabMsg[_MONDAY_P1_],"%s", 0, 0, 1, strDays[0], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_MONDAY_P2_],"%s", 0, 0, 1, strDays[1], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_TUESDAY_P1_],"%s", 0, 0, 1, strDays[2], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_TUESDAY_P2_],"%s", 0, 0, 1, strDays[3], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_WEDNESDAY_P1_],"%s", 0, 0, 1, strDays[4], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_WEDNESDAY_P2_],"%s", 0, 0, 1, strDays[5], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_THURSDAY_P1_],"%s", 0, 0, 1, strDays[6], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_THURSDAY_P2_],"%s", 0, 0, 1, strDays[7], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_FRIDAY_P1_],"%s", 0, 0, 1, strDays[8], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_FRIDAY_P2_],"%s", 0, 0, 1, strDays[9], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SATURDAY_P1_],"%s", 0, 0, 1, strDays[10], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SATURDAY_P2_],"%s", 0, 0, 1, strDays[11], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SUNDAY_P1_],"%s", 0, 0, 1, strDays[12], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SUNDAY_P2_],"%s", 0, 0, 1, strDays[13], 0, 1,  i++, 1);


			GMenu_AddMono(tabMsg[_ESC_], 0, 0, 0, 0, 0, 0, 0, i++, 1);

			GMenu_AddMono(0, 0, 0, 0, 0, "", 0, 0, i++, 0);		// Fine
		break;

		case (MENU_SLAVE_STERILIZER_CALENDAR):
			GMenu_AddMono(tabMsg[_DISPLAY_],0,0,0,0,0,0,0,i++,1);	// Titolo

			// Prapara le stringhe con gli orari
			for (int w=0; w<MAX_PROG; w++)
				sprintf(strDays[w], "%02u:%02u %02u:%02u", (Dati.registers[REGISTER_STERILIZER_PROG_1_START + (w*2)] >> 8) & 0xff, Dati.registers[REGISTER_STERILIZER_PROG_1_START + (w*2)] & 0xff,
						(Dati.registers[REGISTER_STERILIZER_PROG_1_STOP + (w*2)] >> 8) & 0xff, Dati.registers[REGISTER_STERILIZER_PROG_1_STOP + (w*2)] & 0xff);

			GMenu_AddString(tabMsg[_MONDAY_P1_],"%s", 0, 0, 1, strDays[0], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_MONDAY_P2_],"%s", 0, 0, 1, strDays[1], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_TUESDAY_P1_],"%s", 0, 0, 1, strDays[2], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_TUESDAY_P2_],"%s", 0, 0, 1, strDays[3], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_WEDNESDAY_P1_],"%s", 0, 0, 1, strDays[4], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_WEDNESDAY_P2_],"%s", 0, 0, 1, strDays[5], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_THURSDAY_P1_],"%s", 0, 0, 1, strDays[6], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_THURSDAY_P2_],"%s", 0, 0, 1, strDays[7], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_FRIDAY_P1_],"%s", 0, 0, 1, strDays[8], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_FRIDAY_P2_],"%s", 0, 0, 1, strDays[9], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SATURDAY_P1_],"%s", 0, 0, 1, strDays[10], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SATURDAY_P2_],"%s", 0, 0, 1, strDays[11], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SUNDAY_P1_],"%s", 0, 0, 1, strDays[12], 0, 1,  i++, 1);
			GMenu_AddString(tabMsg[_SUNDAY_P2_],"%s", 0, 0, 1, strDays[13], 0, 1,  i++, 1);


			GMenu_AddMono(tabMsg[_ESC_], 0, 0, 0, 0, 0, 0, 0, i++, 1);

			GMenu_AddMono(0, 0, 0, 0, 0, "", 0, 0, i++, 0);		// Fine
		break;

		default:
		return -1;	// KO
	}

	return 0;	// OK
}



/** Funzione di inizializzazione finestra */
static void load(void)
{
	uint8_t pos = 0;

	if (Dati.menuSlave)
	{

		if (Dati.currentProg > 0)
		{
			pos = Dati.currentProg;

			if (Dati.currentMenu == MENU_SLAVE_STERILIZER_CALENDAR)
				menuLoad(MENU_SLAVE_STERILIZER_CALENDAR);
			else
				menuLoad(MENU_PROGRAMS);
		}
		else
		{
			Dati.season = Dati.registers[REGISTER_SEASON];
			Dati.thermostat = Dati.registers[REGISTER_THERMOSTAT];
			Dati.airQuality = Dati.registers[REGISTER_AIR_QUALITY];
			Dati.co2SetPoint = (int16_t)Dati.registers[REGISTER_CO2_SETPOINT];
			Dati.humiditySetPoint = (int16_t)Dati.registers[REGISTER_HUMIDITY_SETPOINT];
			Dati.roomSetPoint = (int16_t)Dati.registers[REGISTER_ROOM_SETPOINT];
			Dati.fan1 = Dati.registers[REGISTER_FAN_2_FAN_1] & 0xff;
			Dati.fan2 = Dati.registers[REGISTER_FAN_2_FAN_1] >> 8;
			Dati.maxFan = Dati.registers[REGISTER_MAX_FAN_MIN_FAN] >> 8;
			Dati.minFan = Dati.registers[REGISTER_MAX_FAN_MIN_FAN] & 0xff;
			Dati.sterilizer = Dati.registers[REGISTER_STERILIZER];
			Dati.antifire = Dati.registers[REGISTER_ANTIFIRE];
			Dati.humiditySensor = Dati.registers[REGISTER_HUMIDITY_SENSOR];
			Dati.slaveWorkMode = Dati.registers[REGISTER_WORK_MODE];
			menuLoad(MENU_SLAVE_MAIN);
		}
	}
	else
		menuLoad(MENU_MASTER_MAIN);

	Dati.cursorMode = CURSOR_MODE_NORMAL;	
	GMenu_Init(pos, &Dati.cursorMode);
}




/** Funzione di disegno finestra */
static void draw(void)
{

	switch (Dati.cursorMode)
	{
		default:
		case (CURSOR_MODE_NORMAL):
			GMenu_Draw();
		break;
		
		case (CURSOR_MODE_COMBO):
			GCombo_Draw(&combo);
		break;
		
		case (CURSOR_MODE_EDIT):
			GEdit_Draw(&editMenu);
		break;
	}

}

/** Funzione di esecuzione finestra */
static void run(unsigned long cmd)
{

	unsigned int res, pos;
	
	switch (Dati.cursorMode)
	{
		default:
		case (CURSOR_MODE_NORMAL):
					
			
			res = GMenu_Run(NULL, &Dati.cursorMode);
			switch (res)
			{
				case (0):	// ESC
					
					switch (Dati.currentMenu)
					{
						case (MENU_MASTER_MAIN):
							// Memorizza parametri in FLASH
							Dati_Save();
						
							Windows_Load(&winMain);	 // Esce dal menù
						break;
						
						case (MENU_SLAVE_MAIN):
							Windows_Load(&winSlaveSave);	 // Esce dal menù
						break;

						case (MENU_PROGRAMS):
							pos = Dati.currentMenu;

							// Torna al menù principale
							menuLoad(MENU_SLAVE_MAIN);

							Dati.cursorMode = CURSOR_MODE_NORMAL;
							GMenu_Init(pos, &Dati.cursorMode);

							Dati.currentProg = 0;
						break;

						case (MENU_SLAVE_STERILIZER_CALENDAR):
							pos = Dati.currentMenu;

							// Torna al menù principale
							menuLoad(MENU_SLAVE_MAIN);

							Dati.cursorMode = CURSOR_MODE_NORMAL;
							GMenu_Init(pos, &Dati.cursorMode);

							Dati.currentProg = 0;
						break;


						default:
							pos = Dati.currentMenu;
						
							// Torna al menù principale
							menuLoad(MENU_MASTER_MAIN);

							Dati.cursorMode = CURSOR_MODE_NORMAL;
							GMenu_Init(pos, &Dati.cursorMode);
						break;
					}
				break;
				
				default:	// Enter su sottomenù	
					
					switch (Dati.currentMenu)
					{	
						case (MENU_MASTER_MAIN):
							switch (res)
							{
								case (MENU_ESC):
									// Memorizza parametri in FLASH
									Dati_Save();
									Windows_Load(&winMain);	 // Esce dal menù
								break;

								case (MENU_UPGRADE):
									Windows_Load(&winUpgrade);	 
									return;

								default:								
								break;
							}
						break;

						case (MENU_DISPLAY):
							if (res == MENU_DISPLAY_ESC)
							{
								pos = Dati.currentMenu;
								res = MENU_MASTER_MAIN;
							
								// Torna al menù principale	
								menuLoad(MENU_MASTER_MAIN);

								Dati.cursorMode = CURSOR_MODE_NORMAL;
								GMenu_Init(pos, &Dati.cursorMode);
							}
						break;




						case (MENU_SLAVE_MAIN):
							switch (res)
							{
								case (MENU_ESC):
									Windows_Load(&winSlaveSave);	 // Esce dal menù
								break;

								case (MENU_OROLOGIO):
									Windows_Load(&winOrologio);
									return;

								case (MENU_TACHO):
									Windows_Load(&winTacho);
									return;

								case (MENU_HOUR_METERS):
									Windows_Load(&winHourMeters);
									return;

								case (MENU_SLAVE_UPGRADE):
									Windows_Load(&winUpgrade);
									return;

								default:
								break;
							}
						break;

						case (MENU_PROGRAMS):
							switch (res)
							{
								default:
									Dati.currentProg = res;
									Windows_Load(&winProgram);
									return;
								break;

								case (MENU_PROGRAMS_ESC):
									pos = Dati.currentMenu;
									res = MENU_SLAVE_MAIN;

									// Torna al menù principale
									menuLoad(MENU_SLAVE_MAIN);

									Dati.cursorMode = CURSOR_MODE_NORMAL;
									GMenu_Init(pos, &Dati.cursorMode);

									Dati.currentProg = 0;
								break;


							}
						break;

						case (MENU_SLAVE_STERILIZER_CALENDAR):
							switch (res)
							{
								default:
									Dati.currentProg = res;
									Windows_Load(&winProgramSterilizer);
									return;
								break;

								case (MENU_PROGRAMS_ESC):
									pos = Dati.currentMenu;
									res = MENU_SLAVE_MAIN;

									// Torna al menù principale
									menuLoad(MENU_SLAVE_MAIN);

									Dati.cursorMode = CURSOR_MODE_NORMAL;
									GMenu_Init(pos, &Dati.cursorMode);

									Dati.currentProg = 0;

								break;


							}
						break;

							
						default:
						break;
					}
						

					if (menuLoad(res) == 0)
					{
						switch (res)
						{
							
							default:
							break;
						}
						
						Dati.cursorMode = CURSOR_MODE_NORMAL;
						GMenu_Init(0, &Dati.cursorMode);
					}
					
				break;
				

				case (255):	 // Nessuna operazione da gestire
				break;
			}
		break;
		
		case (CURSOR_MODE_COMBO):
			GCombo_Run(&combo, &Dati.cursorMode);
		break;
		
		case (CURSOR_MODE_EDIT):
			GEdit_Run(&editMenu, &Dati.cursorMode);
		break;
	}

	if (Dati.currentMenu == MENU_DISPLAY)
	{
		// Ricarica impostazioni display
		Windows_SetContrast(Parameters.DisplayContrast);	
		Windows_SetBacklight(Parameters.DisplayBacklight);
	}												
	
	// Verifica validità parametri SLAVE
	if (Dati.currentMenu == MENU_SLAVE_MAIN)
	{
		if (Dati.minFan > Dati.maxFan)
		{
			Dati.minFan = Dati.maxFan;
		}

		if (Dati.fan1 > Dati.maxFan)
		{
			Dati.fan1 = Dati.maxFan;
		}
		else if (Dati.fan1 < Dati.minFan)
		{
			Dati.fan1 = Dati.minFan;
		}

		if (Dati.fan2 > Dati.maxFan)
		{
			Dati.fan2 = Dati.maxFan;
		}
		else if (Dati.fan2 < Dati.minFan)
		{
			Dati.fan2 = Dati.minFan;
		}
	}


	// Ricarica Lingua, nel caso sia variata 
	if (GetLanguage() != Parameters.Language)
	{
		SetLanguage(Parameters.Language);
		menuLoad(Dati.currentMenu);
		
	}

	// Timeout
	if (cmd)
		cont_run = 0;
	else
		if (++cont_run > TIME_OUT_FINESTRA)
		{
			cont_run = 0;

			// Esce dal menù
			if (Dati.menuSlave)
				Windows_Load(&winSlaveSave);
			else
			{
				// Memorizza parametri
				Dati_Save();
				Windows_Load(&winMain);
			}
		}


}




/** Finestra */
const tWindow winMenu = { load, draw, run, 0 };








