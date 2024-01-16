#include "debug.h"
#include "usb.h"
#include <string.h>

/** Buffer invio dati */
static char bufDebug[32];

/** Massimo livello di severità delle informazioni di debug. */
static int dbg_level = DBG_DEBUG;



void Debug(int level, char *data, uint8_t wait_tx)
{
    // Verifica livello di debug
    if ((level < 0) || (level > dbg_level))
    {
        return;
    }

	sprintf(bufDebug, "%s\r\n", data);
	
	USB_Send((uint8_t *)bufDebug, strlen(bufDebug));
	
	if (wait_tx)
	{
		while (Debug_GetTxLen());
	}
}

void Debug_SetLevel(int level)
{
	dbg_level = level;
}

int Debug_GetTxLen(void)
{
	return 0; // !Uart1_TxEmpty();
}
