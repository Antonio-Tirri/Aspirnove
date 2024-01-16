#include "board.h"
#include "main.h"
#include "dati.h"
#include "application.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// Indirizzo e valore richiesta upgrade (da richiedere al bootloader)
#define ADDR_REQUEST_UPGRADE		0x2001FFF0
#define VAL_REQUEST_UPGRADE			0xA8AD5CEF


#define MIN_POWER_OFF_TIME			(6000 / TIME_DEFAULT_TASK)



static char tmpStr[8];


void Board_Init(void)
{	
	Dati.dipSwitch = Board_GetDipSwitch();
}


uint8_t Board_GetDipSwitch(void)
{
	uint8_t id = 0;

	if (DIP_1_IsOn())	id |= 0x01;
	if (DIP_2_IsOn())	id |= 0x02;
	if (DIP_3_IsOn())	id |= 0x04;
	if (DIP_4_IsOn())	id |= 0x08;

	if (Board_GetHWRev() == HARDWARE_2020)
	{
		if (DIP_5_IsOn())	id |= 0x10;
	}

	return id;
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


uint8_t Board_GetHWRev(void)
{
	uint8_t hw = 0;

	if (HW_REV_1_Get())	hw |= 0x01;
	if (HW_REV_2_Get())	hw |= 0x02;

	switch (hw)
	{
		case (0):
			return HARDWARE_2020;

		default:
			return HARDWARE_2019;
	}
}


