#include "input.h"
#include "main.h"
#include "board.h"
#include "dati.h"
#include "application.h"




// Buffer gestione filtro
static uint8_t input[FILTERED_INPUT];

// Stato ingressi filtrati
static uint8_t filteredValue;




uint8_t Input_Init(void) 
{
	uint8_t w;

	// Reset filtered I/O
	for (w = 0; w < FILTERED_INPUT; w++)
	{
		input[w] = 0;
	}
	filteredValue = 0;

	return 0;
}

uint8_t Input_GetValue(void) 
{
	uint8_t value = 0;

	// DIN 1
	if (DIN_HV1_Get())
	{
		value |= DIN_HV1;
	}
	
	// DIN 2
	if (DIN_HV2_Get())
	{
		value |= DIN_HV2;
	}
	
	// DIN 3
	if (DIN_TERMOST_Get())
	{
		value |= DIN_TERMOST;
	}
	
	// DIN 4
	if (DIN_FILTRO_Get())
	{
		value |= DIN_FILTRO;
	}
	
	
	return value;
}

uint8_t Input_GetFilteredValue(void) 
{
	return filteredValue;
}

void Input_Tick(void) 
{
	static uint16_t tick = 0;
	static uint8_t idx = 0;
	uint8_t i, value;

	if (++tick > FILTER_TICK) 
	{
		tick = 0;

		// Read input 1..FILTERED_INPUT
		value = Input_GetValue();
		for (i = 0; i < FILTERED_INPUT; i++) 
		{
			if (value & (0x1 << i))
			{
				input[i]++;
			}
		}

		// Update filter
		if (++idx >= FILTER_LEN) 
		{
			idx = 0;
			value = 0;
			for (i = 0; i < FILTERED_INPUT; i++) 
			{
				if (input[i] >= (FILTER_LEN / 2))
				{
					value |= (0x01 << i);
				}
				input[i] = 0;
			}

			// Atomic update
			filteredValue = value;
		}
	}
}


void Input_EXTIEvent(uint8_t fan)
{


	if (fan == FAN_1)
	{
		++Dati.contTachF1;
		Dati.contTimeoutTachF1 = TIMEOUT_TACHO;
	}

	else if (fan == FAN_2)
	{
		++Dati.contTachF2;
		Dati.contTimeoutTachF2 = TIMEOUT_TACHO;
	}



}


