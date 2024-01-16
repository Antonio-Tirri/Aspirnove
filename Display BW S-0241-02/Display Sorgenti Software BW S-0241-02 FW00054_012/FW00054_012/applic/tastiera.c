#include "tastiera.h"
#include "main.h"
#include "board.h"
#include "dati.h"

static unsigned int prev_cmd_key = 0;

static uint8_t forceEnterCmd;


void Tastiera_Read(uint32_t *k)
{
	unsigned int cmd_key = 0;

	// Lettura tasti
	if (SW1_Pressed())		cmd_key |= TASTO_ON;
	if (SW2_Pressed())		cmd_key |= TASTO_UP;
	if (SW3_Pressed())		cmd_key |= TASTO_DOWN;
	if (SW4_Pressed())		cmd_key |= TASTO_M;
	if (SW5_Pressed())		cmd_key |= TASTO_BP;

	
	*k = (long)(((long)prev_cmd_key << 16) + cmd_key);
	
	// Aggiorna comando tastiera precedente	
	prev_cmd_key = cmd_key;	
}

void Tastiera_ForceEnterCmd(void)
{
	forceEnterCmd = TRUE;	
}








