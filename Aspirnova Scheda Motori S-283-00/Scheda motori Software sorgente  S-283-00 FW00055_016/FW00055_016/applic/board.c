#include "board.h"
#include "main.h"
#include "dati.h"
#include "pwm.h"
#include "application.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// Indirizzo e valore richiesta upgrade (da richiedere al bootloader)
#define ADDR_REQUEST_UPGRADE		0x2001FFF0
#define VAL_REQUEST_UPGRADE			0xA8AD5CEF


#define MIN_POWER_OFF_TIME			(6000 / TIME_DEFAULT_TASK)

#define TIME_RELE_PWM				(200 / TIME_MAIN_CYCLE)

static char tmpStr[8];


void Board_Init(void)
{	
	Dati.idBoard = Board_GetId();
}


uint8_t Board_GetHWRev(void)
{
	uint8_t hw = 0;

	if (HW_REV_1_Get())	hw |= 0x01;
	if (HW_REV_2_Get())	hw |= 0x02;

	switch (hw)
	{
		case (0):
			return HARDWARE_S0283;

		default:
			return HARDWARE_S0242;
	}
}


uint8_t Board_GetBaudRate(void)
{
	uint8_t baud = 0;

	if (Board_GetHWRev() == HARDWARE_S0242)
		return BAUD_RATE_115200;

	if (DIP_6_IsOn())	baud |= 0x01;
	if (DIP_7_IsOn())	baud |= 0x02;

	switch (baud)
	{
		default:
		case (0):
			return BAUD_RATE_115200;

		case (1):
			return BAUD_RATE_9600;

		case (2):
			return BAUD_RATE_19200;
	}
}

uint8_t Board_GetId(void)
{
	uint8_t id = 0;

	if (DIP_1_IsOn())	id |= 0x01;
	if (DIP_2_IsOn())	id |= 0x02;
	if (DIP_3_IsOn())	id |= 0x04;
	if (DIP_4_IsOn())	id |= 0x08;

	if (Board_GetHWRev() == HARDWARE_S0283)
	{
		if (DIP_5_IsOn())	id |= 0x10;
	}

	// Valori da 1 a 32
	return (id + 1);
}


void Board_RequestUpgrade(void)
{
	*((uint32_t*)ADDR_REQUEST_UPGRADE) = VAL_REQUEST_UPGRADE;
}






uint8_t isNumber(char *data, uint16_t len, int32_t *value)
{
	if (len == 0)
		return FALSE;

	// Scorre la stringa
	for (uint16_t w=0; w<len; w++)
	{
		// Se non è un numero
		if (isdigit(data[w]) == 0)
		{
			if ( (data[w] != '-') && (data[w] != ' ') )	// E non è il segno - o lo spazio
				return FALSE;
		}
	}

	if (len >= sizeof(tmpStr))
		return FALSE;

	for (uint8_t w=0; w<len; w++)
		tmpStr[w] = data[w];

	tmpStr[len] = 0;	// Terminatore

	*value = atoi(tmpStr);

	return TRUE;
}




/** Configura pin come ingresso */
void GPIO_SetInput(GPIO_TypeDef* port, unsigned int bit, unsigned char pull)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStruct.Pin = bit;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = (pull)?GPIO_PULLUP:GPIO_NOPULL;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

/** Configura pin come uscita */
void GPIO_SetOutput(GPIO_TypeDef* port, unsigned int bit)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStruct.Pin = bit;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}



void BATT_Cmd(uint8_t cmd)
{
	static uint16_t contReleBatt = 0;

	// ON: Attiva RELE, attesa di TIME_RELE_PWM, attiva uscita PWM
	// OFF: Disattiva uscita PWM, attesa di TIME_RELE_PWM, disattiva RELE

	if (cmd > 0)
	{
		RELE_BATT_On();
		if (contReleBatt < TIME_RELE_PWM)
		{
			++contReleBatt;
			PWM_SetPercent(PWM_BATT, 0);
		}
		else
		{
			PWM_SetPercent(PWM_BATT, cmd);
		}
	}
	else
	{
		PWM_SetPercent(PWM_BATT, 0);
		if (contReleBatt > 0)
		{
			--contReleBatt;
		}
		else
		{
			RELE_BATT_Off();
		}
	}

}



void FANS_Cmd(uint8_t cmd1, uint8_t cmd2)
{
	static uint16_t contReleFan1 = 0;
	static uint16_t contReleFan2 = 0;


	// Almeno una ventola superiore al 30%
	if ( (cmd1 >= 30) || (cmd2 >= 30) )
		Dati.registers[REGISTER_STATUS] |= STATUS_SPEED_FAN_GREATER_THAN_30;
	else
		Dati.registers[REGISTER_STATUS] &= ~STATUS_SPEED_FAN_GREATER_THAN_30;


	// Stato ingressi TACHO relativi ai motori
	Dati.registers[REGISTER_STATUS] &= ~STATUS_TACHO;
	if (Board_GetHWRev() != HARDWARE_S0242)
	{
		if (cmd1 >= 10)	// Almeno 10% di comando
		{
			if (Dati.contTimeoutTachF1 == 0)
				Dati.registers[REGISTER_STATUS] |= STATUS_TACHO;
		}
		else
		{
			Dati.contTimeoutTachF1 = TIMEOUT_TACHO;
		}

		// Nella modalità Fillbox non è presente la ventola 2
		if (Dati.registers[REGISTER_WORK_MODE] != MODE_FILLBOX)
		{
			if (cmd2 >= 10)	// Almeno 10% di comando
			{
				if (Dati.contTimeoutTachF2 == 0)
					Dati.registers[REGISTER_STATUS] |= STATUS_TACHO;
			}
			else
			{
				Dati.contTimeoutTachF2 = TIMEOUT_TACHO;
			}
		}
	}

	// Ventola 1
	if (cmd1 > 0)
	{
		if (contReleFan1 > TIME_RELE_PWM)
			RELE_FAN1_On();

		if (contReleFan1 < (TIME_RELE_PWM * 2))
		{
			++contReleFan1;
			PWM_SetPercent(PWM_FAN1, 0);
		}
		else
		{
			PWM_SetPercent(PWM_FAN1, cmd1);
		}
	}
	else
	{
		PWM_SetPercent(PWM_FAN1, 0);
		if (contReleFan1 > 0)
		{
			--contReleFan1;
			if (contReleFan1 < TIME_RELE_PWM)
				RELE_FAN1_Off();
		}
		else
		{
			RELE_FAN1_Off();
		}
	}

	// Ventola 2
	if (cmd2 > 0)
	{
		if (contReleFan2 > TIME_RELE_PWM)
			RELE_FAN2_On();

		if (contReleFan2 <  (TIME_RELE_PWM * 2))
		{
			++contReleFan2;
			PWM_SetPercent(PWM_FAN2, 0);
		}
		else
		{
			PWM_SetPercent(PWM_FAN2, cmd2);
		}
	}
	else
	{
		PWM_SetPercent(PWM_FAN2, 0);
		if (contReleFan2 > 0)
		{
			--contReleFan2;
			if (contReleFan2 < TIME_RELE_PWM)
				RELE_FAN2_Off();
		}
		else
		{
			RELE_FAN2_Off();
		}
	}

}


uint16_t AirQuality_Convert(uint16_t type, float volt)
{
	uint16_t ppm;

	// Partitore sull'ingresso
	volt = volt * 3.564;

	switch (type)
	{
		case (AIR_QUALITY_PROBE_0_2000):
			ppm = (uint16_t)((volt * 2000) / 10.0);
		break;

		case (AIR_QUALITY_PROBE_0_5000):
			ppm = (uint16_t)((volt * 5000) / 10.0);
		break;

		default:
			ppm = 0;
		break;

	}

	return ppm;
}

uint8_t Schedule_IsSetTime(uint8_t day_week, uint8_t hour, uint8_t min, uint16_t *details)
{
	uint8_t w, prog, h1, m1, h2, m2;

	// day_week: RTC_WEEKDAY_MONDAY(1) ..RTC_WEEKDAY_SUNDAY (7)
	if ( (day_week < RTC_WEEKDAY_MONDAY) || (day_week > RTC_WEEKDAY_SUNDAY) )
		return 1;

	// Recupera programma in base al giorno della settimana
	prog = (day_week - 1) * 4;

	// Ogni giorno ha due programmi associati
	for (w=0; w<2; w++)
	{
		// Recupera ora e minuti di inizio e fine programma
		h1 = (Dati.registers[REGISTER_PROG_1_START + prog] >> 8) & 0xff;
		m1 = Dati.registers[REGISTER_PROG_1_START + prog] & 0xff;
		h2 = (Dati.registers[REGISTER_PROG_1_STOP + prog] >> 8) & 0xff;
		m2 = Dati.registers[REGISTER_PROG_1_STOP + prog] & 0xff;


		// Ora corrente all'interno dell'orario impostato per questo giorno?
		if ( ((h1*60)+m1) <= ((hour*60)+min) )
		{
			if ( ((hour*60)+min) <  ((h2*60)+m2))
			{
				*details =  Dati.registers[REGISTER_PROG_1_DETAILS +  ((day_week - 1) * 2) + w];
				return 0;
			}
		}

		// Passa al programma successivo
		prog = prog + 2;
	}


	return 1;
}



uint8_t Sterilizer_IsSetTime(uint8_t day_week, uint8_t hour, uint8_t min)
{
	uint8_t w, prog, h1, m1, h2, m2;

	// day_week: RTC_WEEKDAY_MONDAY(1) ..RTC_WEEKDAY_SUNDAY (7)
	if ( (day_week < RTC_WEEKDAY_MONDAY) || (day_week > RTC_WEEKDAY_SUNDAY) )
		return 1;

	// Recupera programma in base al giorno della settimana
	prog = (day_week - 1) * 4;

	// Ogni giorno ha due programmi associati
	for (w=0; w<2; w++)
	{
		// Recupera ora e minuti di inizio e fine programma
		h1 = (Dati.registers[REGISTER_STERILIZER_PROG_1_START + prog] >> 8) & 0xff;
		m1 = Dati.registers[REGISTER_STERILIZER_PROG_1_START + prog] & 0xff;
		h2 = (Dati.registers[REGISTER_STERILIZER_PROG_1_STOP + prog] >> 8) & 0xff;
		m2 = Dati.registers[REGISTER_STERILIZER_PROG_1_STOP + prog] & 0xff;


		// Ora corrente all'interno dell'orario impostato per questo giorno?
		if ( ((h1*60)+m1) <= ((hour*60)+min) )
		{
			if ( ((hour*60)+min) <  ((h2*60)+m2))
				return 0;
		}

		// Passa al programma successivo
		prog = prog + 2;
	}


	return 1;
}
