#include "application.h"
#include "maintimer.h"
#include "main.h"
#include "dati.h"
#include "board.h"
#include "pwm.h"
#include "debug.h"
#include "watchdog.h"
#include "usb.h"
#include "usart.h"
#include "uart1.h"
#include "modbus.h"
#include "crc.h"
#include "usb.h"
#include "input.h"
#include "analog.h"
#include "temperature_sensor.h"
#include "humidity_sensor.h"
#include "rtc.h"
#include <ctype.h>



void CheckCode(void);


#define TIME_DELAY_OFF				(30000 / TIME_MAIN_CYCLE)

// Parametri riscaldamento iniziale
#define THRESHOLD_INITIAL_WARM_UP	(-50)	// -5.O°C
#define TIME_INITIAL_WARM_UP		(120000 / TIME_MAIN_CYCLE)	// 2 minuti

#define TIME_DELAY_RELE_AUX1		(2000 / TIME_MAIN_CYCLE)

#define TIME_HOUR					(3600000 / TIME_MAIN_CYCLE)





// Contiene l'istante in cui è iniziato il ciclo main corrente
static unsigned int startCiclo = 0;

// Contatore sistema in OFF
static uint16_t contStatusOff = TIME_DELAY_OFF;

// Stato uscita riscaldatore
static uint8_t heaterLastStatus = 0;



void MainTimer_Callback(void)
{
	static uint16_t contMs = 0;

	// Gestione watchdog
	Watchdog_OnTimer();

	// Gestione ingressi digitali
	Input_Tick();

	// Funzione per identificare la completa ricezione di un pacchetto dall'uart
	Uart1_RTOManagement();

	// Ogni 250ms
	if ((++contMs % 250) == 0)
		LED_ON_Toggle();

	// Ogni secondo
	if (contMs >= 1000)
	{
		contMs = 0;
		Dati.registers[REGISTER_TACHO_FAN_1] = Dati.contTachF1;
		Dati.registers[REGISTER_TACHO_FAN_2] = Dati.contTachF2;
		Dati.contTachF1 = 0;
		Dati.contTachF2 = 0;
	}
}


void Application_Run(void)
{
	// Spegne i led
	LED_ERR_Off();
	LED_ON_Off();
	LED_PC1_Off();
	LED_PC2_Off();

	// Inizializza watchdog
	Watchdog_Init();

	// Attesa iniziale di 50 ms
	startCiclo = MainTimer_GetValue();
	while ((MainTimer_GetValue() - startCiclo) < 50);

	Dati_Init();

	PWM_Init();
	Input_Init();
	Analog_Init();


	// Reinizializza UART con il baud rate impostato tramite dip switch
	MX_UART1_ReInit(Board_GetBaudRate());
	Uart1_Init();

	// Inizializza modbus su 485
	Modbus_Init();

	// Carica registro versione firmware / revisione hardware
	Dati.registers[REGISTER_SLAVE_VERSION] = atoi(APP_VERSION);
	if (Board_GetHWRev() == HARDWARE_S0283)
		Dati.registers[REGISTER_SLAVE_VERSION] |= 0x0100;


	while (1)
	{

		// Memorizza istante inizio ciclo main
		startCiclo = MainTimer_GetValue();

		// Gestione contatore watchdog
		Watchdog_Poll();

		// Lettura ingressi
		Dati.input = Input_GetFilteredValue();

		Application_Poll();

		// Gestione valori AD convertiti al ciclo precedente
		Analog_Management();

		// Lettura AD
		Analog_Measure();

		// Attesa ciclo
		while ((MainTimer_GetValue() - startCiclo) < TIME_MAIN_CYCLE);

	}

}


char str[32];
uint8_t dw, dd, mm, yy, h, m, s, contRtc;;
void Application_Poll(void)
{
	static uint8_t dipFilter = 0;
	static uint8_t fan1 = 0, fan2 = 0;
	static uint16_t contReleAux1 = 0;
	static uint32_t contHour = 0;

	uint8_t modify = FALSE;
	int16_t setpoint = ((int16_t)Dati.registers[REGISTER_ROOM_SETPOINT] * 10);
	uint8_t pidOnFans;
	uint8_t bypassEnabled = TRUE;
	uint8_t fan1Threshold;
	uint8_t fan2Threshold;

	// Lettura Data/Ora
	RTC_GetDateTime(&dw, &dd, &mm, &yy, &h, &m, &s);

	Dati.registers[REGISTER_DATETIME_DD_MM] = (dw << 12) + (dd << 4) + mm;
	Dati.registers[REGISTER_DATETIME_YY_H] = (yy << 8) + h;
	Dati.registers[REGISTER_DATETIME_M_S] = (m << 8) + s;

	// I valori AD vengono letti in modalità OVERSAMPLING_FILTERED_VALUE se sono già presenti dei campioni oppure in OVERSAMPLING_VALUE
	Dati.registers[REGISTER_AUX1] = (Analog_Get(AIN_AUX1, Analog_FilteredValuePresent()? OVERSAMPLING_FILTERED_VALUE : OVERSAMPLING_VALUE)) >> 2;	// Richiedo ad Analog_Get il valore con oversampling, quindi dovrò riportarlo nel range 0..4095
	Dati.registers[REGISTER_AUX2] = (Analog_Get(AIN_AUX2, Analog_FilteredValuePresent()? OVERSAMPLING_FILTERED_VALUE : OVERSAMPLING_VALUE)) >> 2;	// Richiedo ad Analog_Get il valore con oversampling, quindi dovrò riportarlo nel range 0..4095

	Dati.registers[REGISTER_TEMP_EXT] = TemperatureSensor_Convert(TEMPERATURE_SENSOR_TYPE_NTC_10K_3435, Analog_Get(AIN_TEMP_EXT, Analog_FilteredValuePresent()? OVERSAMPLING_FILTERED_VALUE : OVERSAMPLING_VALUE));
	Dati.registers[REGISTER_TEMP_INT] = TemperatureSensor_Convert(TEMPERATURE_SENSOR_TYPE_NTC_10K_3435, Analog_Get(AIN_TEMP_INT, Analog_FilteredValuePresent()? OVERSAMPLING_FILTERED_VALUE : OVERSAMPLING_VALUE));

	Dati.registers[REGISTER_CO2] =  AirQuality_Convert(Dati.registers[REGISTER_AIR_QUALITY], Analog_GetVolt(AIN_CO2, OVERSAMPLING_FILTERED_VALUE));

	Dati.registers[REGISTER_IO] = Dati.input;

	Dati.registers[REGISTER_REAL_FAN_2_FAN_1] = (fan2 << 8) + fan1;

	// Sensore umidità abilitato?
	if (Dati.registers[REGISTER_HUMIDITY_SENSOR])
	{
		Dati.registers[REGISTER_HUMIDITY_VALUE] = HumiditySensor_Convert(HUMIDITY_SENSOR_TYPE_EE10_ELEKTRONIK, Analog_GetVolt(AIN_AUX1, OVERSAMPLING_FILTERED_VALUE));
	}
	else
		Dati.registers[REGISTER_HUMIDITY_VALUE] = 0;


	// Riporta lo stato del filtro anche nel registro di stato
	if (Dati.registers[REGISTER_IO] & IO_DIN_FILTRO)
		Dati.registers[REGISTER_STATUS] |= STATUS_FILTRO;
	else
		Dati.registers[REGISTER_STATUS] &= ~STATUS_FILTRO;


	if (RELE_FAN1_Get())
		Dati.registers[REGISTER_IO] |= IO_RELE_FAN1;
	if (RELE_FAN2_Get())
		Dati.registers[REGISTER_IO] |= IO_RELE_FAN2;
	if (RELE_BATT_Get())
		Dati.registers[REGISTER_IO] |= IO_RELE_BATT;
	if (RELE_AUX1_Get())
		Dati.registers[REGISTER_IO] |= IO_RELE_AUX1;
	if (RELE_BYP_Get())
		Dati.registers[REGISTER_IO] |= IO_RELE_BYP;


	switch (Dati.registers[REGISTER_COMMAND])
	{

		case (CMD_ON_OFF):
			Dati.registers[REGISTER_COMMAND] = 0;

			// In MANUALE o in AUTOMATICO
			if ( ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_MANUAL) ||
				 ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_AUTO) )
			{
				// Se termostato non presente
				if (Dati.registers[REGISTER_THERMOSTAT] == FALSE)
				{
					if (Dati.registers[REGISTER_STATUS] & STATUS_ON)
						Dati.registers[REGISTER_STATUS] &= ~STATUS_ON;
					else
						Dati.registers[REGISTER_STATUS] |= STATUS_ON;

					registerStatus = Dati.registers[REGISTER_STATUS];
					Dati_StatusSave();
				}
			}
		break;

		case (CMD_BYPASS):
			Dati.registers[REGISTER_COMMAND] = 0;

			// Se sono in manuale
			if ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_MANUAL)
			{
				if (Dati.registers[REGISTER_STATUS] & STATUS_BYPASS)
					Dati.registers[REGISTER_STATUS] &= ~STATUS_BYPASS;
				else
					Dati.registers[REGISTER_STATUS] |= STATUS_BYPASS;

				registerStatus = Dati.registers[REGISTER_STATUS];
				Dati_StatusSave();
			}
		break;


		case (CMD_SAVE_PARAMETERS):

			Dati.registers[REGISTER_COMMAND] = 0;

			if (Dati.registers[REGISTER_STATUS] != registerStatus)
			{
				registerStatus = Dati.registers[REGISTER_STATUS];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_SEASON] != Parameters.registerSeason)
			{
				Parameters.registerSeason = Dati.registers[REGISTER_SEASON];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_THERMOSTAT] != Parameters.registerThermostat)
			{
				Parameters.registerThermostat = Dati.registers[REGISTER_THERMOSTAT];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_AIR_QUALITY] != Parameters.registerAirQuality)
			{
				Parameters.registerAirQuality = Dati.registers[REGISTER_AIR_QUALITY];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_CO2_SETPOINT] != Parameters.registerCo2SetPoint)
			{
				Parameters.registerCo2SetPoint = Dati.registers[REGISTER_CO2_SETPOINT];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_ROOM_SETPOINT] != Parameters.registerRoomSetPoint)
			{
				Parameters.registerRoomSetPoint = Dati.registers[REGISTER_ROOM_SETPOINT];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_FAN_2_FAN_1] != Parameters.registerFan2Fan1)
			{
				Parameters.registerFan2Fan1 = Dati.registers[REGISTER_FAN_2_FAN_1];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_MAX_FAN_MIN_FAN] != Parameters.registerMaxFanMinFan)
			{
				Parameters.registerMaxFanMinFan = Dati.registers[REGISTER_MAX_FAN_MIN_FAN];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_STERILIZER] != Parameters.registerSterilizer)
			{
				Parameters.registerSterilizer = Dati.registers[REGISTER_STERILIZER];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_ANTIFIRE] != Parameters.registerAntifire)
			{
				Parameters.registerAntifire = Dati.registers[REGISTER_ANTIFIRE];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_HUMIDITY_SENSOR] != Parameters.registerHumiditySensor)
			{
				Parameters.registerHumiditySensor = Dati.registers[REGISTER_HUMIDITY_SENSOR];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_WORK_MODE] != Parameters.registerWorkMode)
			{
				Parameters.registerWorkMode = Dati.registers[REGISTER_WORK_MODE];
				modify = TRUE;
			}

			if (Dati.registers[REGISTER_HUMIDITY_SETPOINT] != Parameters.registerHumiditySetPoint)
			{
				Parameters.registerHumiditySetPoint = Dati.registers[REGISTER_HUMIDITY_SETPOINT];
				modify = TRUE;
			}

			if (modify)
				Dati_Save();

		break;


		case (CMD_SLAVE_UPGRADE):

			Dati.registers[REGISTER_COMMAND] = 0;

			// Richiesta aggiornamento
			Board_RequestUpgrade();
			Watchdog_ForcedReset();
		break;



		default:
		break;

	}





	switch (Dati.status)
	{
		// Stato iniziale. Lettura filtrata dei dip switch
		default:
		case (STATUS_INIT):

			if (Dati.idBoard == Board_GetId())
			{
				if (++dipFilter >= DIP_FILTER )
				{
					Dati.status = STATUS_WORK;
				}
			}
			else
			{
				dipFilter = 0;
				Dati.idBoard = Board_GetId();
			}

		break;



		case (STATUS_WORK):

			// Gestione pacchetto modbus, se presente
			Modbus_Poll();

			// Antincendio abilitato?
			if (Dati.registers[REGISTER_ANTIFIRE])
			{
				// Riporta lo stato dell'antincendio anche nel registro di stato
				if (Dati.registers[REGISTER_IO] & IO_DIN_HV1)	// Se 220 presente --> segnalo allarme
				{
					Dati.registers[REGISTER_STATUS] |= STATUS_ANTIFIRE;

					// Disabilita PID
					PID_FAN_Poll(0, 0, -1);
					PID_FAN_Poll(1, 0, -1);
					FANS_Cmd(0, 0);
					PID_H2O_Poll(-1);

					BATT_Cmd(0);
					RELE_AUX1_Off();
					RELE_BYP_Off();

					break;
				}
				else
					Dati.registers[REGISTER_STATUS] &= ~STATUS_ANTIFIRE;
			}
			else
				Dati.registers[REGISTER_STATUS] &= ~STATUS_ANTIFIRE;


			// Sterilizzatore configurato nel sistema?
			if (Dati.registers[REGISTER_STERILIZER])
			{
				// Riporta lo stato dell'allarme sanificazione anche nel registro di stato
				if ( (Dati.registers[REGISTER_IO] & IO_DIN_HV2) == 0)	// Se 220 non presente --> segnalo allarme
				{
					Dati.registers[REGISTER_STATUS] |= STATUS_SANITIZE;
				}
				else
					Dati.registers[REGISTER_STATUS] &= ~STATUS_SANITIZE;
			}
			else
				Dati.registers[REGISTER_STATUS] &= ~STATUS_SANITIZE;


			// Gestione uscita sterilizzatore (attivata in base al calendario sterilizzatore, sempre che non sia attivo l'allarme sanificazione)
			if (Dati.registers[REGISTER_STERILIZER])
			{
				if ( (Sterilizer_IsSetTime(dw, h, m) == 0) &&
					((Dati.registers[REGISTER_STATUS] & STATUS_SANITIZE) == 0) )
					RELE_AUX1_On();
				else
					RELE_AUX1_Off();
			}


			// In MANUALE o in AUTOMATICO
			if ( ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_MANUAL) ||
				 ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_AUTO) )
			{
				// Termostato presente?
				if (Dati.registers[REGISTER_THERMOSTAT])
				{
					// Stato ON dato dall'ingresso TERMOSTATO
					if (Dati.input & DIN_TERMOST)
						Dati.registers[REGISTER_STATUS] |= STATUS_ON;
					else
						Dati.registers[REGISTER_STATUS] &= ~STATUS_ON;
				}
			}
			else if ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_SCHEDULE) 	// Modalità programmi orari
			{
				// Stato ON dato dagli orari impostati
				if (Schedule_IsSetTime(dw, h, m, &Dati.currentProgDetails) == 0)
					Dati.registers[REGISTER_STATUS] |= STATUS_ON;
				else
					Dati.registers[REGISTER_STATUS] &= ~STATUS_ON;

			}

			// Verifica se è cambiata la modalità
			if ( (Dati.registers[REGISTER_STATUS] & STATUS_MODE) != (registerStatus & STATUS_MODE) )
			{
				registerStatus = Dati.registers[REGISTER_STATUS];
				Dati_StatusSave();
			}

			switch (Dati.registers[REGISTER_STATUS] & STATUS_MODE)
			{
				default:
				case (MODE_MANUAL):

				break;

				case (MODE_SCHEDULE):

					// Se il programma corrente è configurato in MAN, non effettua le istruzioni sottostanti
					if (Dati.currentProgDetails & PROG_DETAILS_MANUAL)	// Bit15: Modalità lavoro del programma: 0=Automatico / 1=Manuale
					{
						break;
					}

					// break non presente volontariamente!

				case (MODE_AUTO):

					if (Dati.registers[REGISTER_SEASON] == SEASON_WINTER)
					{
						// Se temperatura interna < temperatura esterna e temperatura interna < setpoint
						if ( ((int16_t)Dati.registers[REGISTER_TEMP_INT] < (int16_t)Dati.registers[REGISTER_TEMP_EXT]) &&
							 ((int16_t)Dati.registers[REGISTER_TEMP_INT] < setpoint) )
						{
							Dati.registers[REGISTER_STATUS] |= STATUS_BYPASS;
						}
						else
						{
							Dati.registers[REGISTER_STATUS] &= ~STATUS_BYPASS;
						}
					}
					else
					{
						// Se temperatura interna > temperatura esterna e temperatura interna > setpoint
						if ( ((int16_t)Dati.registers[REGISTER_TEMP_INT] > (int16_t)Dati.registers[REGISTER_TEMP_EXT]) &&
							 ((int16_t)Dati.registers[REGISTER_TEMP_INT] > setpoint) )
						{
							Dati.registers[REGISTER_STATUS] |= STATUS_BYPASS;
						}
						else
						{
							Dati.registers[REGISTER_STATUS] &= ~STATUS_BYPASS;
						}
					}

				break;


			}



			// Gestione ventole
			if (Dati.registers[REGISTER_STATUS] & STATUS_ON)	// Sistema in ON?
			{
				pidOnFans = FALSE;

				if (contStatusOff > 0)
				{
					contStatusOff = 0;

					// Temperatura esterna inferiore alla soglia THRESHOLD_INITIAL_WARM_UP
					if ((int16_t)Dati.registers[REGISTER_TEMP_EXT] <= THRESHOLD_INITIAL_WARM_UP)
					{
						Dati.contInitialWarmUp = TIME_INITIAL_WARM_UP;
					}
				}

				// Se riscaldamento iniziale in corso (si attiva quando il sistema va in ON e la temperatura esterna è inferiore a THRESHOLD_INITIAL_WARM_UP °C)
				if (Dati.contInitialWarmUp)
				{
					// Ventola1 = 0 e Ventola2 = 100%
					fan1Threshold = 0;
					fan2Threshold = 100;
				}
				else
				{
					// Recupera valori ventola dal registro
					fan1Threshold = (uint8_t)(Dati.registers[REGISTER_FAN_2_FAN_1] & 0xff);
					fan2Threshold = (uint8_t)(Dati.registers[REGISTER_FAN_2_FAN_1] >> 8);

					// Se modalità PROGRAMMI
					if ( ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_SCHEDULE) )
					{
						// Recupera valori ventola
						fan1Threshold = (uint8_t)(Dati.currentProgDetails & 0x7f);
						fan2Threshold = (uint8_t)((Dati.currentProgDetails >> 7) & 0x7f);
					}

				}



				// Nella modalità Fillbox non è presente il pid sulle ventole
				if (Dati.registers[REGISTER_WORK_MODE] != MODE_FILLBOX)
				{
					// In AUTOMATICO o in PROGRAMMI ORARI configurato in automatico
					if ( ((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_AUTO) ||
						 (((Dati.registers[REGISTER_STATUS] & STATUS_MODE) == MODE_SCHEDULE) && ((Dati.currentProgDetails & PROG_DETAILS_MANUAL)==0) ) )
					{
						// Se controllo qualità aria abilitata
						if (Dati.registers[REGISTER_AIR_QUALITY])
						{
							// Se CO2 letta maggiore del setpoint
							if ( (Dati.registers[REGISTER_CO2]) > (Dati.registers[REGISTER_CO2_SETPOINT]) )
							{
								pidOnFans = TRUE;
								fan1 = PID_FAN_Poll(0, fan1Threshold, (Dati.registers[REGISTER_CO2]) - (Dati.registers[REGISTER_CO2_SETPOINT]));
								fan2 = PID_FAN_Poll(1, fan2Threshold, (Dati.registers[REGISTER_CO2]) - (Dati.registers[REGISTER_CO2_SETPOINT]));

								// Se PID CO2 attivo, disabilito il BYPASS
								bypassEnabled = FALSE;
							}
						}

						// Se il PID non è già abilitato per CO2
						if (pidOnFans == FALSE)
						{
							// Se umidità letta maggiore del setpoint
							if ( (Dati.registers[REGISTER_HUMIDITY_VALUE]) > (Dati.registers[REGISTER_HUMIDITY_SETPOINT]) )
							{
								pidOnFans = TRUE;
								fan1 = PID_FAN_Poll(0, fan1Threshold, ((Dati.registers[REGISTER_HUMIDITY_VALUE]) - (Dati.registers[REGISTER_HUMIDITY_SETPOINT])) * 50);
								fan2 = PID_FAN_Poll(1, fan2Threshold, ((Dati.registers[REGISTER_HUMIDITY_VALUE]) - (Dati.registers[REGISTER_HUMIDITY_SETPOINT])) * 50);

								// La regolazione sull'umidità non genera comandi superiori al 90% (richiesta cliente)
								if (fan2 > 90)
									fan2 = 90;
								if (fan1 > 90)
									fan1 = 90;

							}
						}
					}
				}

				if (!pidOnFans)
				{
					// Disabilita PID
					PID_FAN_Poll(0, 0, -1);
					PID_FAN_Poll(1, 0, -1);

					if (fan1 < fan1Threshold)
						++fan1;
					else if (fan1 > fan1Threshold)
						--fan1;

					if (fan2 < fan2Threshold)
						++fan2;
					else if (fan2 > fan2Threshold)
						--fan2;
				}

				// Se funzione sterilizzatore non attiva, l'uscite RELE_AUX1 viene utilizzate per indicare sistema in ON
				if (Dati.registers[REGISTER_STERILIZER] == FALSE)
				{
					if (contReleAux1 >= TIME_DELAY_RELE_AUX1)
						RELE_AUX1_On();
					else
					{
						RELE_AUX1_Off();
						++contReleAux1;
					}

				}
			}
			else
			{
				Dati.contInitialWarmUp = 0;

				// Se non stavo scaldando
				if (heaterLastStatus == 0)
				{
					contStatusOff = TIME_DELAY_OFF;	// Spengo subito le ventole
				}

				// Azzera le uscite FAN soltanto dopo 30 secondi
				if (contStatusOff >= TIME_DELAY_OFF )
				{
					fan2 = 0;
					fan1 = 0;

					// Disabilita PID
					PID_FAN_Poll(0, 0, -1);
					PID_FAN_Poll(1, 0, -1);
				}
				else
				{
					++contStatusOff;
				}

				// Se sono in OFF disabilito il BYPASS
				bypassEnabled = FALSE;

				// Se funzione sterilizzatore non attiva, l'uscite RELE_AUX1 viene utilizzate per indicare sistema in ON
				if (Dati.registers[REGISTER_STERILIZER] == FALSE)
				{
					contReleAux1 = 0;
					RELE_AUX1_Off();
				}
			}



			// Nella modalità Fillbox il bypass non è presente
			if (Dati.registers[REGISTER_WORK_MODE] == MODE_FILLBOX)
			{
				bypassEnabled = FALSE;
				fan2 = 0;
			}


			// Verifica limite massimo comando
			uint16_t maxFan = (Parameters.registerMaxFanMinFan >> 8) & 0xff;
			if (fan2 > maxFan)
				fan2 = maxFan;
			if (fan1 > maxFan)
				fan1 = maxFan;

			FANS_Cmd(fan1, fan2);


			// BYPASS
			if ( (Dati.registers[REGISTER_STATUS] & STATUS_BYPASS) && (bypassEnabled) )
				RELE_BYP_On();
			else
				RELE_BYP_Off();


			if (Dati.registers[REGISTER_STATUS] & STATUS_ON)	// Sistema in ON?
			{
				// H2O - BATT
				if ( (Dati.registers[REGISTER_SEASON] == SEASON_WINTER) && 	// INVERNO
					 (Dati.contInitialWarmUp == 0) )	// e riscaldamento iniziale non in corso
				{
					Dati.registers[REGISTER_H2O_VALUE] = PID_H2O_Poll(setpoint - Dati.registers[REGISTER_TEMP_INT]);

					BATT_Cmd(Dati.registers[REGISTER_H2O_VALUE]);	// Riscaldatore con stesso comando dell'H2O

					// Memorizza stato dell'ultimo comando dato al riscaldatore
					if (Dati.registers[REGISTER_H2O_VALUE] > 0)
						heaterLastStatus = 1;
					else
						heaterLastStatus = 0;
				}
				else
				{
					Dati.registers[REGISTER_H2O_VALUE] = PID_H2O_Poll(Dati.registers[REGISTER_TEMP_INT] - setpoint);
					BATT_Cmd(0);	// Riscaldatore spento
					heaterLastStatus = 0;
				}
			}
			else
			{
				PID_H2O_Poll(-1);	// Disabilita PID
				Dati.registers[REGISTER_H2O_VALUE] = 0;
				BATT_Cmd(0);
			}

		break;
	}

	// Gestione riscaldamento iniziale (si attiva quando il sistema va in ON e la temperatura esterna è inferiore a THRESHOLD_INITIAL_WARM_UP °C)
	if (Dati.contInitialWarmUp)
	{
		--Dati.contInitialWarmUp;
		Dati.registers[REGISTER_STATUS] |= STATUS_INITIAL_WARM_UP;
	}
	else
	{
		Dati.registers[REGISTER_STATUS] &= ~STATUS_INITIAL_WARM_UP;
	}

	// Decrementa contatori TACHO
	if (Dati.contTimeoutTachF1)
		--Dati.contTimeoutTachF1;
	if (Dati.contTimeoutTachF2)
		--Dati.contTimeoutTachF2;

	// Sistema in ON?
	if (Dati.registers[REGISTER_STATUS] & STATUS_ON)
	{
		// Ogni ora
		if (++contHour > TIME_HOUR)
		{
			contHour = 0;

			// Incremento le ore totali e parziali
			++Parameters.partialHourMeters;
			++Parameters.totalHourMeters;
			Dati_Save();

			Dati.registers[REGISTER_PARTIAL_HOUR_METERS_H] = ((Parameters.partialHourMeters >> 16) & 0xffff);
			Dati.registers[REGISTER_PARTIAL_HOUR_METERS_L] = (Parameters.partialHourMeters & 0xffff);
			Dati.registers[REGISTER_TOTAL_HOUR_METERS_H] = ((Parameters.totalHourMeters >> 16) & 0xffff);
			Dati.registers[REGISTER_TOTAL_HOUR_METERS_L] = (Parameters.totalHourMeters & 0xffff);
		}
	}

}


