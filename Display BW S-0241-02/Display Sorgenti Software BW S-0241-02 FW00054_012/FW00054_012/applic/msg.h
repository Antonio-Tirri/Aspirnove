#ifndef _MSG_
#define _MSG_



// Supporto C++
#ifdef __cplusplus
	extern "C" {
#endif


/** Lingue utilizzate */
enum {ITALIANO, INGLESE, SPAGNOLO, NUMERO_LINGUE};

void SetLanguage(unsigned char lang);

unsigned char GetLanguage(void);

/** Puntatore alla tabella di lingua utilizzata	*/
extern char **tabMsg;



enum
{
    _NO_,                               
    _SI_,

	_OK_,
	_ERRORE_,

	_MENU_,

	_SCHEDE_CONTROLLO_,

	_LINGUA_,	
	_ITALIANO_,
	_INGLESE_,
	_SPAGNOLO_,
	
	_BUZZER_,
	_OFF_,
	_ON_,

	_BAUD_RATE_,
	_BAUD_RATE_1_,
	_BAUD_RATE_2_,
	_BAUD_RATE_3_,

	_DISPLAY_,
	_CONTRASTO_,
	_ILLUMINAZIONE_,

	_ESC_,
	
	_STERILIZER_,
	_STERILIZER_CALENDAR_,
	_ANTIFIRE_,
	_HUMIDITY_SENSOR_,

	_FIRMWARE_UPGRADE_,
	_SLAVE_FIRMWARE_UPGRADE_,
	_CONFERMA_UPGRADE_,
	
	_OROLOGIO_,
	_TACHO_FAN_,
	_HOUR_METERS_,
	_TOTAL_HOURS_,
	_PARTIAL_HOURS_,
	_PARTIAL_INFO_,

	_SEASON_,
	_THERMOSTAT_,
	_AIR_QUALITY_,
	_CO2_SETPOINT_,
	_HUM_SETPOINT_,
	_ROOM_SETPOINT_,

	_FAN_1_,
	_FAN_2_,
	_MIN_FAN_,
	_MAX_FAN_,

	_SALVATAGGIO_,

	_WINTER_,
	_SUMMER_,

	_NO_AIR_QUALITY_,
	_AIR_QUALITY_0_2000_,
	_AIR_QUALITY_0_5000_,

	_MONDAY_,
	_TUESDAY_,
	_WEDNESDAY_,
	_THURSDAY_,
	_FRIDAY_,
	_SATURDAY_,
	_SUNDAY_,

	_COMMUNICATION_ERROR_1_,
	_COMMUNICATION_ERROR_2_,

	_PROGRAMS_,
	_MONDAY_P1_,
	_MONDAY_P2_,
	_TUESDAY_P1_,
	_TUESDAY_P2_,
	_WEDNESDAY_P1_,
	_WEDNESDAY_P2_,
	_THURSDAY_P1_,
	_THURSDAY_P2_,
	_FRIDAY_P1_,
	_FRIDAY_P2_,
	_SATURDAY_P1_,
	_SATURDAY_P2_,
	_SUNDAY_P1_,
	_SUNDAY_P2_,

	_PROGRAM_,
	_START_,
	_STOP_,

	_INVALID_,
	_TIME_,

	_LOGO_,
	_LOGO_NO_,
	_LOGO_ASPIRNOVA_,
	_LOGO_2_,
	_LOGO_3_,
	_LOGO_4_,
	_LOGO_5_,
	_LOGO_6_,
	_LOGO_7_,
	_LOGO_8_,
	_LOGO_9_,
	_LOGO_10_,



	_MANUAL_,
	_AUTOMATIC_,
	_SCHEDULER_,

	_RUN_,
	_NO_RUN_,
	_IMPOSTAZIONI_,

	_SAVE_,
	_ABORT_,

	_ANTIFIRE_MSG_1_,
	_ANTIFIRE_MSG_2_,

	_REPLACE_AIR_FILTER_,
	_SANITIZATION_ALARM_,

	_DISPLAY_HEATER_,

	_MODALITA_,
	_STANDARD_,
	_FILLBOX_,

    NUM_MSG
};




// Supporto C++
#ifdef __cplusplus
	}
#endif


#endif // _MSG_
