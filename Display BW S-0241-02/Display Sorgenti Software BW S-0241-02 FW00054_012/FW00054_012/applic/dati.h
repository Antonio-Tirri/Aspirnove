/**	\file   dati.h
 *
 *	\brief  Strutture Dati (RAM / FLASH) applicazione.
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *  \version    v1.0 - 21/02/2019
 * 	-   Versione iniziale

 *
 */

#ifndef __DATI_H__
#define __DATI_H__

#include "stdint.h"
#include "stm32f2xx_hal.h"




enum
{
	REGISTER_1 = 0,
	REGISTER_2,
	REGISTER_3,
	REGISTER_4,
	REGISTER_5,
	REGISTER_6,
	REGISTER_7,
	REGISTER_8,
	REGISTER_9,
	REGISTER_10,
	REGISTER_11,
	REGISTER_12,
	REGISTER_13,
	REGISTER_14,
	REGISTER_15,
	REGISTER_16,
	REGISTER_17,
	REGISTER_18,
	REGISTER_19,
	REGISTER_20,
	REGISTER_21,
	REGISTER_22,
	REGISTER_23,
	REGISTER_24,
	REGISTER_25,
	REGISTER_26,
	REGISTER_27,
	REGISTER_28,
	REGISTER_29,
	REGISTER_30,
	REGISTER_31,
	REGISTER_32,
	REGISTER_33,
	REGISTER_34,
	REGISTER_35,
	REGISTER_36,
	REGISTER_37,
	REGISTER_38,
	REGISTER_39,
	REGISTER_40,
	REGISTER_41,
	REGISTER_42,
	REGISTER_43,
	REGISTER_44,
	REGISTER_45,
	REGISTER_46,
	REGISTER_47,
	REGISTER_48,
	REGISTER_49,
	REGISTER_50,
	REGISTER_51,
	REGISTER_52,
	REGISTER_53,
	REGISTER_54,
	REGISTER_55,
	REGISTER_56,
	REGISTER_57,
	REGISTER_58,
	REGISTER_59,
	REGISTER_60,
	REGISTER_61,
	REGISTER_62,
	REGISTER_63,
	REGISTER_64,
	REGISTER_65,
	REGISTER_66,
	REGISTER_67,
	REGISTER_68,
	REGISTER_69,
	REGISTER_70,
	REGISTER_71,
	REGISTER_72,
	REGISTER_73,
	REGISTER_74,
	REGISTER_75,
	REGISTER_76,
	REGISTER_77,
	REGISTER_78,
	REGISTER_79,
	REGISTER_80,
	REGISTER_81,
	REGISTER_82,
	REGISTER_83,
	REGISTER_84,
	REGISTER_85,
	REGISTER_86,
	REGISTER_87,
	REGISTER_88,
	REGISTER_89,
	REGISTER_90,
	REGISTER_91,
	REGISTER_92,
	REGISTER_93,
	REGISTER_94,
	REGISTER_95,
	REGISTER_96,
	REGISTER_97,
	REGISTER_98,
	REGISTER_99,
	REGISTER_100,
	REGISTER_101,
	REGISTER_102,
	REGISTER_103,
	REGISTER_104,
	REGISTER_105,
	REGISTER_106,
	REGISTER_107,
	REGISTER_108,
	REGISTER_109,

	REGISTERS_NUM
};


#define REGISTER_SLAVE_VERSION				REGISTER_1
#define REGISTER_COMMAND					REGISTER_2

#define REGISTER_DATETIME_DD_MM				REGISTER_3
#define REGISTER_DATETIME_YY_H				REGISTER_4
#define REGISTER_DATETIME_M_S				REGISTER_5

#define REGISTER_IO							REGISTER_6
#define REGISTER_CO2						REGISTER_7
#define REGISTER_AUX1						REGISTER_8
#define REGISTER_AUX2						REGISTER_9
#define REGISTER_TEMP_EXT					REGISTER_10
#define REGISTER_TEMP_INT					REGISTER_11
#define REGISTER_H2O_VALUE					REGISTER_12

#define REGISTER_STATUS						REGISTER_13
#define REGISTER_SEASON						REGISTER_14
#define REGISTER_THERMOSTAT 				REGISTER_15
#define REGISTER_AIR_QUALITY				REGISTER_16
#define REGISTER_CO2_SETPOINT				REGISTER_17
#define REGISTER_ROOM_SETPOINT				REGISTER_18
#define REGISTER_FAN_2_FAN_1				REGISTER_19
#define REGISTER_MAX_FAN_MIN_FAN			REGISTER_20
#define REGISTER_REAL_FAN_2_FAN_1			REGISTER_21

#define REGISTER_PROG_1_START				REGISTER_22
#define REGISTER_PROG_1_STOP				REGISTER_23
#define REGISTER_PROG_2_START				REGISTER_24
#define REGISTER_PROG_2_STOP				REGISTER_25
#define REGISTER_PROG_3_START				REGISTER_26
#define REGISTER_PROG_3_STOP				REGISTER_27
#define REGISTER_PROG_4_START				REGISTER_28
#define REGISTER_PROG_4_STOP				REGISTER_29
#define REGISTER_PROG_5_START				REGISTER_30
#define REGISTER_PROG_5_STOP				REGISTER_31
#define REGISTER_PROG_6_START				REGISTER_32
#define REGISTER_PROG_6_STOP				REGISTER_33
#define REGISTER_PROG_7_START				REGISTER_34
#define REGISTER_PROG_7_STOP				REGISTER_35
#define REGISTER_PROG_8_START				REGISTER_36
#define REGISTER_PROG_8_STOP				REGISTER_37
#define REGISTER_PROG_9_START				REGISTER_38
#define REGISTER_PROG_9_STOP				REGISTER_39
#define REGISTER_PROG_10_START				REGISTER_40
#define REGISTER_PROG_10_STOP				REGISTER_41
#define REGISTER_PROG_11_START				REGISTER_42
#define REGISTER_PROG_11_STOP				REGISTER_43
#define REGISTER_PROG_12_START				REGISTER_44
#define REGISTER_PROG_12_STOP				REGISTER_45
#define REGISTER_PROG_13_START				REGISTER_46
#define REGISTER_PROG_13_STOP				REGISTER_47
#define REGISTER_PROG_14_START				REGISTER_48
#define REGISTER_PROG_14_STOP				REGISTER_49

#define REGISTER_STERILIZER_PROG_1_START	REGISTER_50
#define REGISTER_STERILIZER_PROG_1_STOP		REGISTER_51
#define REGISTER_STERILIZER_PROG_2_START	REGISTER_52
#define REGISTER_STERILIZER_PROG_2_STOP		REGISTER_53
#define REGISTER_STERILIZER_PROG_3_START	REGISTER_54
#define REGISTER_STERILIZER_PROG_3_STOP		REGISTER_55
#define REGISTER_STERILIZER_PROG_4_START	REGISTER_56
#define REGISTER_STERILIZER_PROG_4_STOP		REGISTER_57
#define REGISTER_STERILIZER_PROG_5_START	REGISTER_58
#define REGISTER_STERILIZER_PROG_5_STOP		REGISTER_59
#define REGISTER_STERILIZER_PROG_6_START	REGISTER_60
#define REGISTER_STERILIZER_PROG_6_STOP		REGISTER_61
#define REGISTER_STERILIZER_PROG_7_START	REGISTER_62
#define REGISTER_STERILIZER_PROG_7_STOP		REGISTER_63
#define REGISTER_STERILIZER_PROG_8_START	REGISTER_64
#define REGISTER_STERILIZER_PROG_8_STOP		REGISTER_65
#define REGISTER_STERILIZER_PROG_9_START	REGISTER_66
#define REGISTER_STERILIZER_PROG_9_STOP		REGISTER_67
#define REGISTER_STERILIZER_PROG_10_START	REGISTER_68
#define REGISTER_STERILIZER_PROG_10_STOP	REGISTER_69
#define REGISTER_STERILIZER_PROG_11_START	REGISTER_70
#define REGISTER_STERILIZER_PROG_11_STOP	REGISTER_71
#define REGISTER_STERILIZER_PROG_12_START	REGISTER_72
#define REGISTER_STERILIZER_PROG_12_STOP	REGISTER_73
#define REGISTER_STERILIZER_PROG_13_START	REGISTER_74
#define REGISTER_STERILIZER_PROG_13_STOP	REGISTER_75
#define REGISTER_STERILIZER_PROG_14_START	REGISTER_76
#define REGISTER_STERILIZER_PROG_14_STOP	REGISTER_77

#define REGISTER_STERILIZER 				REGISTER_78
#define REGISTER_ANTIFIRE	 				REGISTER_79
#define REGISTER_HUMIDITY_SENSOR			REGISTER_80
#define REGISTER_HUMIDITY_VALUE				REGISTER_81
#define REGISTER_TACHO_FAN_1				REGISTER_82
#define REGISTER_TACHO_FAN_2				REGISTER_83
#define REGISTER_WORK_MODE					REGISTER_84

#define REGISTER_HUMIDITY_SETPOINT			REGISTER_85

#define REGISTER_PROG_1_DETAILS				REGISTER_86
#define REGISTER_PROG_2_DETAILS				REGISTER_87
#define REGISTER_PROG_3_DETAILS				REGISTER_88
#define REGISTER_PROG_4_DETAILS				REGISTER_89
#define REGISTER_PROG_5_DETAILS				REGISTER_90
#define REGISTER_PROG_6_DETAILS				REGISTER_91
#define REGISTER_PROG_7_DETAILS				REGISTER_92
#define REGISTER_PROG_8_DETAILS				REGISTER_93
#define REGISTER_PROG_9_DETAILS				REGISTER_94
#define REGISTER_PROG_10_DETAILS			REGISTER_95
#define REGISTER_PROG_11_DETAILS			REGISTER_96
#define REGISTER_PROG_12_DETAILS			REGISTER_97
#define REGISTER_PROG_13_DETAILS			REGISTER_98
#define REGISTER_PROG_14_DETAILS			REGISTER_99

#define REGISTER_PARTIAL_HOUR_METERS_H		REGISTER_100
#define REGISTER_PARTIAL_HOUR_METERS_L		REGISTER_101
#define REGISTER_TOTAL_HOUR_METERS_H		REGISTER_102
#define REGISTER_TOTAL_HOUR_METERS_L		REGISTER_103



// Revisioni hardware
#define HARDWARE_S0242						0
#define HARDWARE_S0283						1


#define STATUS_MODE							0x0003	// Bit0 e Bit1
#define STATUS_BYPASS						0x0004	// Bit2
#define STATUS_ON							0x0008	// Bit3
#define STATUS_SPEED_FAN_GREATER_THAN_30	0x0010	// Bit4
#define STATUS_FILTRO						0x0020	// Bit5
#define STATUS_ANTIFIRE						0x0040	// Bit6
#define STATUS_TACHO						0x0080	// Bit7
#define STATUS_SANITIZE						0x0100	// Bit8



#define CMD_SAVE_PARAMETERS					0x01
#define CMD_ON_OFF							0x02
#define CMD_BYPASS							0x03
#define CMD_SLAVE_UPGRADE					0xFF

#define SEASON_WINTER						0x00
#define SEASON_SUMMER						0x01


#define AIR_QUALITY_OFF						0x00
#define AIR_QUALITY_PROBE_0_2000			0x01
#define AIR_QUALITY_PROBE_0_5000			0x02


#define MODBUS_BUFFER_SIZE					128

#define MAX_PROG							14		// 2 * 7 giorni

// Flag man/auto all'interno del programma giornaliero
#define PROG_DETAILS_MANUAL					0x8000



// Numero massimo di schede slave controllabili
#define CONTROL_BOARDS_MAX					32

// L'allarme sanificazione rimane visualizzato ancora per questo tempo dopo la disattivazione, in secondi.
#define SANITIZE_TIMEOUT_SEC			(600)	// 10 minuti




// MENU




#define MENU_MASTER_MAIN					0
#define MENU_ESC							1
#define MENU_DISPLAY						4
#define MENU_UPGRADE						10

#define MENU_DISPLAY_ESC					3

#define MENU_SLAVE_MAIN						100
#define MENU_PROGRAMS						8
#define MENU_OROLOGIO						9
#define MENU_TACHO							10
#define MENU_SLAVE_STERILIZER_CALENDAR		11
#define MENU_HOUR_METERS					12

#define MENU_SLAVE_UPGRADE					21


#define MENU_PROGRAMS_LUN_1					1
#define MENU_PROGRAMS_LUN_2					2
#define MENU_PROGRAMS_MAR_1					3
#define MENU_PROGRAMS_MAR_2					4
#define MENU_PROGRAMS_MER_1					5
#define MENU_PROGRAMS_MER_2					6
#define MENU_PROGRAMS_GIO_1					7
#define MENU_PROGRAMS_GIO_2					8
#define MENU_PROGRAMS_VEN_1					9
#define MENU_PROGRAMS_VEN_2					10
#define MENU_PROGRAMS_SAB_1					11
#define MENU_PROGRAMS_SAB_2					12
#define MENU_PROGRAMS_DOM_1					13
#define MENU_PROGRAMS_DOM_2					14
#define MENU_PROGRAMS_ESC					15






/** Modalità di lavoro */
enum {MODE_STANDARD, MODE_FILLBOX, MODE_NUMS};

/** Loghi gestiti */
enum {NO_LOGO, ASPIRNOVA_LOGO, LOGOS_NUMS};

/** Baud rate gestiti */
enum {BAUD_RATE_115200=0, BAUD_RATE_19200, BAUD_RATE_9600, BAUD_RATE_NUM};

// Status
typedef enum
{
	STATUS_INIT,
	STATUS_WORK,
}tStatus;


// Mode
typedef enum
{
	MODE_MANUAL,
	MODE_AUTO,
	MODE_SCHEDULE
}tMode;


/** Modalità cursore all'interno del menù. */
typedef enum
{
    CURSOR_MODE_NORMAL,
    CURSOR_MODE_COMBO,
	CURSOR_MODE_EDIT
}tCursorMode;




// Indirizzi
#define ADDRESS_APPLIC            	0x08008000



/** Confronta 2 stringhe restituendo 0 se uguali */
#define CMP(a, b)	((strlen(a) != strlen(b))? 1 : strncmp(a, b, strlen(b)))



#define IO_DIN_HV1			((uint16_t)0x01)
#define IO_DIN_HV2			((uint16_t)0x02)
#define IO_DIN_TERMOST		((uint16_t)0x04)
#define IO_DIN_FILTRO		((uint16_t)0x08)

#define IO_RELE_FAN1		((uint16_t)0x0100)
#define IO_RELE_FAN2		((uint16_t)0x0200)
#define IO_RELE_BATT		((uint16_t)0x0400)
#define IO_RELE_FCOM		((uint16_t)0x0800)
#define IO_RELE_AUX1		((uint16_t)0x1000)
#define IO_RELE_BYP			((uint16_t)0x2000)


/** Livelli di password */
enum
{
	LEVEL_USER = 0,
	LEVEL_ADMIN
};




/** Struttura Time */
typedef struct __packed
{
	unsigned char Hour;		/**< 0..23 */
	unsigned char Min;		/**< 0..59 */
	unsigned char Sec;		/**< 0..59 */

} tTime;

/** Struttura Date */
typedef struct __packed
{
	unsigned char Year;		/**< 0..99 */
	unsigned char Month;	/**< 1..12 */
	unsigned char Day;		/**< 1..31 */

} tDate;

/** Struttura Timestamp */
typedef struct __packed
{
	tDate Date;
	tTime Time;

} tDateTime;





/** Parametri FLASH applicazione */
 typedef struct __packed _tParameters
{
	uint16_t 		Language;

	// Numero di schede slave collegate
	uint16_t		ControlBoards;

	// Scheda selezionata
	uint16_t 		currentBoard;


	// Display
	uint16_t 		DisplayContrast;
	uint16_t 		DisplayBacklight;

	// Logo visualizzato all'avvio
	uint16_t	 	Logo;

	// Abilitazione Buzzer
	uint16_t		Buzzer;

	// Baud rate utilizzato nella comunicazione 485 (0=115200, 1=19200, 2=9600)
	uint16_t		BaudRate;

	// Attiva/Disattiva visualizzazione simbolo serpentina e percentuale riscaldante
	uint16_t		DisplayHeater;

	// Modalità di funzionamento: standard / fillbox
	uint16_t		WorkMode;


	uint16_t		free;


	// Checksum
	uint8_t			checksum;


}  tParameters;







typedef struct __packed _tDati
{
	tStatus			status;

	// Stato tastiera
	uint32_t 		key;

	// Livello accesso menù
	uint8_t			level;

	// Stato dip switch
	uint8_t			dipSwitch;

	tCursorMode		cursorMode;


	uint16_t		modbusTimeout;

	char			modbusBuffer[MODBUS_BUFFER_SIZE];
	uint8_t			modbusBufferLen;

	uint16_t		registers[REGISTERS_NUM];

	uint8_t			menuSlave;

	uint32_t		fanStatus;
	uint32_t		filterStatus;
	uint32_t		antifireStatus;
	uint32_t		tachoStatus;
	uint32_t		sanitizeStatus;

	uint16_t		sanitizeTimeout[CONTROL_BOARDS_MAX];

	uint16_t		season;
	uint16_t		thermostat;
	uint16_t		airQuality;
	uint16_t		co2SetPoint;
	uint16_t		humiditySetPoint;
	int16_t			roomSetPoint;
	uint16_t		fan1;
	uint16_t 		fan2;
	uint16_t		minFan;
	uint16_t		maxFan;
	uint8_t			sterilizer;
	uint8_t			antifire;
	uint8_t			humiditySensor;
	uint8_t			slaveWorkMode;

	uint8_t			currentProg;

	uint8_t			hardwareSlave;

	uint8_t			currentMenu;

}tDati;






// Extern strutture
extern tParameters Parameters;
extern tDati Dati;




/** Inizializza strutture */
void Dati_Init(void);


/** Carica struttura dati dall'eeprom */
void Dati_Load(void);

/** Carica valori di default nella struttura Parameters in eeprom */
void Dati_LoadDefaultParameters(void);

/** Salva struttura Parameters in flash */
void Dati_Save(void);



#endif

