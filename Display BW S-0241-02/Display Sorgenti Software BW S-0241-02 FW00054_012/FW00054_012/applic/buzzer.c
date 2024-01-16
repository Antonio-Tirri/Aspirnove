#include "buzzer.h"
#include "main.h"
#include "application.h"
#include "board.h"
#include "pwm.h"
#include "dati.h"

#define TIME_BEEP		50

static uint16_t contBeep;


/** Attiva beep singolo */
uint8_t Buzzer_Beep(void)
{
	contBeep = TIME_BEEP;
	return 0;
}

/** Da richiamare ogni ms */
void Buzzer_OnTick()
{

	if (contBeep > 0)
	{
		--contBeep;
		// Se buzzer abilitato, attiva pwm al 50% su uscita
		if (Parameters.Buzzer)
		{
			PWM_SetPercent(PWM_BUZZER, 50);
		}
	}
	else
	{
		PWM_SetPercent(PWM_BUZZER, 0);
	}

}
