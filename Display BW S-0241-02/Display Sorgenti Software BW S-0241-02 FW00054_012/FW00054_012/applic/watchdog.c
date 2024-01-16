#include "watchdog.h" 
#include "board.h" 
#include "application.h" 
#include "iwdg.h"


#define TIME_WATCHDOG			3000


static char enableWatchdog = FALSE;

// Controllore watchdog. (il refresh del watchdog viene eseguito nell'interrupt del timer)
// Tramite il controllore vengono anche identificati eventuali blocchi nel ciclo di main
static unsigned int contWatchdog = 0;

static uint8_t resetRequest = FALSE;


void Watchdog_Init(void)
{		
	enableWatchdog = TRUE;
	contWatchdog = TIME_WATCHDOG;
#ifdef USE_BOOTLOADER
		HAL_IWDG_Refresh(&hiwdg);
#endif
}


void Watchdog_Poll(void)
{
	// Carico il contatore "controllore"
	contWatchdog = TIME_WATCHDOG;
}


void Watchdog_OnTimer()
{
	// Se è stato richiesto un reset, abbandono la funzione
	if (resetRequest)
		return;

	// Finchè il controllore assume valori superiori allo 0
	if ((enableWatchdog) && (contWatchdog))
	{
		contWatchdog--;
#ifdef USE_BOOTLOADER
		HAL_IWDG_Refresh(&hiwdg);
#endif
	}
}
	

void Watchdog_ForcedReset(void)
{
	resetRequest = TRUE;
}




