#include "dati.h" 
#include "main.h"
#include "board.h"
#include "debug.h"
#include "application.h" 
#include "stm32f2xx_hal.h"
#include <string.h>

char str[64];

#define FLASH_PARAMETERS_ADDRESS					0x08004000
#define FLASH_STATUS_PARAMETER_START_ADDRESS		0x4400
#define FLASH_STATUS_PARAMETER_END_ADDRESS			0x7FFF


// Struttura RAM
tDati           Dati;


// Struttura FLASH 
tParameters     Parameters;

uint16_t		registerStatus;


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






uint16_t Dati_StatusLoad(void)
{
	uint16_t data = 0;
	uint16_t status = 0;

	Dati.indexRegisterStatus = FLASH_STATUS_PARAMETER_START_ADDRESS;

	while ( (data != 0xffff) && (Dati.indexRegisterStatus < FLASH_STATUS_PARAMETER_END_ADDRESS) )
	{
		status = data;
		data = *(uint16_t *)(0x08000000 +Dati.indexRegisterStatus);

		if (data != 0xffff)
		{
			Dati.indexRegisterStatus += 2;

		}
	}


	return status;
}





void Dati_StatusSave(void)
{
	// Se sono arrivato alla fine dell'area STATUS_PARAMETER devo cancellare il settore (utilizzo la Dati_Save per fare questo)
	if (Dati.indexRegisterStatus >= FLASH_STATUS_PARAMETER_END_ADDRESS)
	{
		//sprintf(str, "SETTORE PIENO **********************************");
		//Debug(DBG_INFO, str, 0);
		Dati_Save();
	}

	//sprintf(str, "DATI_STATUS_SAVE %04x", Dati.indexRegisterStatus);
	//Debug(DBG_INFO, str, 0);

	__disable_irq();

	// SCRITTURA in flash
	HAL_FLASH_Unlock();

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Dati.indexRegisterStatus, registerStatus);
	Dati.indexRegisterStatus += 2;
	HAL_FLASH_Lock();

	__enable_irq();

	LED_PC2_Toggle();
}



void Dati_Init(void)
{
	uint8_t w;

	// Inizializza struttura in RAM
	memset(&Dati, 0, sizeof(tDati));

	// Inizializza struttura in EEPROM
	Dati_Load();


	// Per il registro status, recupera soltanto la modalità
	Dati.registers[REGISTER_STATUS] = Dati_StatusLoad();


	Dati.registers[REGISTER_SEASON] = Parameters.registerSeason;
	Dati.registers[REGISTER_THERMOSTAT] = Parameters.registerThermostat;
	Dati.registers[REGISTER_AIR_QUALITY] = Parameters.registerAirQuality;
	Dati.registers[REGISTER_CO2_SETPOINT] = Parameters.registerCo2SetPoint;
	Dati.registers[REGISTER_ROOM_SETPOINT] = Parameters.registerRoomSetPoint;
	Dati.registers[REGISTER_FAN_2_FAN_1] = Parameters.registerFan2Fan1;
	Dati.registers[REGISTER_MAX_FAN_MIN_FAN] = Parameters.registerMaxFanMinFan;
	Dati.registers[REGISTER_STERILIZER] = Parameters.registerSterilizer;
	Dati.registers[REGISTER_ANTIFIRE] = Parameters.registerAntifire;
	Dati.registers[REGISTER_HUMIDITY_SENSOR] = Parameters.registerHumiditySensor;
	Dati.registers[REGISTER_WORK_MODE] = Parameters.registerWorkMode;
	Dati.registers[REGISTER_HUMIDITY_SETPOINT] = Parameters.registerHumiditySetPoint;

	for (w=0; w<MAX_PROG; w++)
	{
		Dati.registers[REGISTER_PROG_1_START + (w*2)]  = Parameters.registerProgStart[w];
		Dati.registers[REGISTER_PROG_1_STOP + (w*2)]  = Parameters.registerProgStop[w];
		Dati.registers[REGISTER_PROG_1_DETAILS + w]  = Parameters.registerProgDetails[w];

		Dati.registers[REGISTER_STERILIZER_PROG_1_START + (w*2)]  = Parameters.registerSterilizerProgStart[w];
		Dati.registers[REGISTER_STERILIZER_PROG_1_STOP + (w*2)]  = Parameters.registerSterilizerProgStop[w];
	}

	Dati.registers[REGISTER_PARTIAL_HOUR_METERS_H] = ((Parameters.partialHourMeters >> 16) & 0xffff);
	Dati.registers[REGISTER_PARTIAL_HOUR_METERS_L] = (Parameters.partialHourMeters & 0xffff);
	Dati.registers[REGISTER_TOTAL_HOUR_METERS_H] = ((Parameters.totalHourMeters >> 16) & 0xffff);
	Dati.registers[REGISTER_TOTAL_HOUR_METERS_L] = (Parameters.totalHourMeters & 0xffff);

}







void Dati_LoadDefaultParameters(void)
{
	uint8_t w;

	memset(&Parameters, 0, sizeof(tParameters));

	Parameters.registerSeason = SEASON_WINTER;
	Parameters.registerThermostat = 0;
	Parameters.registerAirQuality = 0;
	Parameters.registerCo2SetPoint = 600;
	Parameters.registerRoomSetPoint = 25;
	Parameters.registerFan2Fan1 = (100 << 8) + 100; // (42 << 8) + 28;
	Parameters.registerMaxFanMinFan = (100 << 8) + 20;
	Parameters.registerSterilizer = 0;
	Parameters.registerAntifire = 0;
	Parameters.registerHumiditySensor = 0;
	Parameters.registerWorkMode = MODE_STANDARD;

	for (w=0; w<MAX_PROG; w++)
	{
		Parameters.registerProgStart[w] = 0;
		Parameters.registerProgStop[w] = 0;
		Parameters.registerProgDetails[w] = 0;

		Parameters.registerSterilizerProgStart[w] = 0;
		Parameters.registerSterilizerProgStop[w] = 0;
	}

}


void Dati_Load(void)
{
	uint16_t w;
	unsigned char *ptr = (unsigned char *)&Parameters;

    for(w=0; w<sizeof(tParameters); w++)
	{
		*ptr = *(uint8_t *)(FLASH_PARAMETERS_ADDRESS + w);
		++ptr;
	}
	
    // Se checksum sbagliata carico valori di default
    if (Parameters.checksum != checksum( (unsigned char *)&Parameters, (sizeof(tParameters) - 1) ) )
    {
    	LED_ERR_On();
    	Dati_LoadDefaultParameters();
        Dati_Save();
    } 

}




void Dati_Save(void)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SectorError;
	uint16_t w;
	unsigned char *ptr = (unsigned char *)&Parameters;


	//sprintf(str, "DATI_SAVE");
	//Debug(DBG_INFO, str, 0);

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

	// In quanto il settore è stato completamente cancellato, riscrive lo stato all'inizio dell'area STATUS_PARAMETER
	Dati.indexRegisterStatus = FLASH_STATUS_PARAMETER_START_ADDRESS;
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Dati.indexRegisterStatus, registerStatus);
	Dati.indexRegisterStatus += 2;

	HAL_FLASH_Lock();

	__enable_irq();

	LED_PC2_Toggle();

}



