#include "application.h"
#include "maintimer.h"
#include "tastiera.h"
#include "main.h"
#include "dati.h"
#include "board.h"
#include "pwm.h"
#include "debug.h"
#include "watchdog.h"
#include "windows.h"
#include "usb.h"
#include "uart1.h"
#include "uart3.h"
#include "modbusMaster.h"
#include "buzzer.h"
#include "crc.h"
#include <ctype.h>



void CheckCode(void);



// Contiene l'istante in cui è iniziato il ciclo main corrente
static unsigned int startCiclo = 0;

void MainTimer_Callback(void)
{
	static uint16_t contMs = 0;

	// Gestione watchdog
	Watchdog_OnTimer();

	// Funzione per identificare la completa ricezione di un pacchetto dall'uart
	Uart1_RTOManagement();

	// Gestione Buzzer
	Buzzer_OnTick();

	++contMs;


	// Ogni 250 ms
	if ( (contMs % 250) == 0)
	{
		if (Dati.sanitizeStatus) 	// Allarme sanificazione (e qualsiasi stato delle ventole)
		{
			LED_GREEN_Off();
			LED_RED_On();
		}
		else if (Dati.fanStatus == 0)	// Ventole in OFF
		{
			LED_GREEN_Off();
			LED_RED_Off();
		}
		else if (Dati.fanStatus & Dati.filterStatus)  // Allarme filtro (e ventole in ON)
		{
			LED_GREEN_Off();
			LED_RED_On();
		}
		else if (Dati.fanStatus & Dati.tachoStatus)
		{
			LED_GREEN_On();
			LED_RED_On();
		}
		else
		{
			LED_RED_Off();
			LED_GREEN_On();
		}
	}

	// Ogni secondo
	if (contMs >= 1000)
	{
		contMs = 0;

		// Decrementa timeout allarmi sanificazione
		for (uint8_t w=0; w<CONTROL_BOARDS_MAX; w++)
			if (Dati.sanitizeTimeout[w])
				--Dati.sanitizeTimeout[w];

	}
}


void Application_Run(void)
{
	static uint8_t contDraw = 0;

	LED_RED_Off();

	// Inizializza watchdog
	Watchdog_Init();

	// Attesa iniziale di 50 ms
	startCiclo = MainTimer_GetValue();
	while ((MainTimer_GetValue() - startCiclo) < 50);

	Dati_Init();

	PWM_Init();

	Uart1_Init();
	Uart3_Init();

	// Inizializza modbus su 485
	ModbusMaster_Init();

	// Inizializza modulo gestione videate
	Windows_Init();



	while (1)
	{

		// Memorizza istante inizio ciclo main
		startCiclo = MainTimer_GetValue();

		// Gestione contatore watchdog
		Watchdog_Poll();

		// Lettura tasti
		Tastiera_Read(&Dati.key);

		// Buzzer
		if ((Dati.key & 0xffff) > (Dati.key >> 16))
			Buzzer_Beep();


		Application_Poll();

		// Gestione videata
	    Windows_Run(Dati.key);

	    if (++contDraw >= (TIME_WINDOW_DRAW / TIME_MAIN_CYCLE) )
	    {
	    	contDraw = 0;
	    	// Disegna videata
	    	Windows_Draw();
	    }

		// Attesa ciclo
		while ((MainTimer_GetValue() - startCiclo) < TIME_MAIN_CYCLE);

	}

}


char str[32];
void Application_Poll(void)
{
	static uint8_t dipFilter = 0;

	switch (Dati.status)
	{
		// Stato iniziale. Lettura filtrata dei dip switch
		default:
		case (STATUS_INIT):

			if (Dati.dipSwitch == Board_GetDipSwitch())
			{
				if (++dipFilter >= DIP_FILTER )
				{
					Dati.status = STATUS_WORK;
				}
			}
			else
			{
				dipFilter = 0;
				Dati.dipSwitch = Board_GetDipSwitch();
			}

		break;


		case (STATUS_WORK):
			//sprintf(str, "%x", Dati.key);
			//Debug(DBG_INFO, str, 0);

			ModbusMaster_Poll();

		break;
	}

}

