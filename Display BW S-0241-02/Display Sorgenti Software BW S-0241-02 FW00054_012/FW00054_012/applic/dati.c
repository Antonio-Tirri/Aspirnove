#include "dati.h" 
#include "board.h"
#include "buzzer.h"
#include "pwm.h"
#include "application.h" 
#include "stm32f2xx_hal.h"
#include <string.h>



#define FLASH_PARAMETERS_ADDRESS		0x08004000


// Struttura RAM
tDati           Dati;


// Struttura FLASH 
tParameters     Parameters;


/** Metodo interno. Calcola checksum su struttura buff di lunghezza len */
unsigned char checksum (unsigned char *buff, int len)
{
    int i;
    unsigned char check = 0;

    for(i=0; i<len; i++)
    {
        check += (unsigned char)*(buff + i);
    }
	
    return 0xff - check;
}


void Dati_Init(void)
{

	// Inizializza struttura in RAM
	memset(&Dati, 0, sizeof(tDati));

	// Inizializza struttura in EEPROM
	Dati_Load();

	// Parto sempre con la scheda #1
	Parameters.currentBoard = 1;

}






void Dati_LoadDefaultParameters(void)
{
	memset(&Parameters, 0, sizeof(tParameters));

	Parameters.Logo = ASPIRNOVA_LOGO;
	Parameters.ControlBoards = 1;
	Parameters.DisplayBacklight = 100;

	Parameters.BaudRate = BAUD_RATE_115200;

	Parameters.Buzzer = 1;
	Parameters.DisplayHeater = FALSE;

}


void Dati_Load(void)
{
#ifdef USE_BOOTLOADER
	uint16_t w;
	unsigned char *ptr = (unsigned char *)&Parameters;

    for(w=0; w<sizeof(tParameters); w++)
	{
		*ptr = *(uint8_t *)(FLASH_PARAMETERS_ADDRESS + w);
		++ptr;
	}
	
    // Se checksum sbagliata carico valori di default
    if (Parameters.checksum != checksum( (unsigned char *)&Parameters, (sizeof(tParameters) - 1) ) )
#endif
    {
    	Buzzer_Beep();
    	Dati_LoadDefaultParameters();
        Dati_Save();
    } 

}


void Dati_Save(void)
{
#ifdef USE_BOOTLOADER
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SectorError;
	uint16_t w;
	unsigned char *ptr = (unsigned char *)&Parameters;

	Parameters.checksum = checksum( (unsigned char *)&Parameters, (sizeof(tParameters) - 1) );
	
	__disable_irq();

	// Cancella area flash riservata ai parametri
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_1;	// 0x0801 4000 - 0x0801 7FFF
	EraseInitStruct.NbSectors = 1;				// FLASH_SECTOR_1 .. FLASH_SECTOR_1
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
	HAL_FLASH_Lock();

	// SCRITTURA in flash
	HAL_FLASH_Unlock();
	for (w=0; w<sizeof(tParameters); w++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_PARAMETERS_ADDRESS + w, *ptr);
		++ptr;
	}
	HAL_FLASH_Lock();

	__enable_irq();
#endif
}







