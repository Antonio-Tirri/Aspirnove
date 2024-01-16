#include "msg.h"

static unsigned char Language;





const char * _table_ITALIANO_[] =
{
    "NO",                               
    "SI",				

	"OK!",
	"ERRORE",

	"MENU",

	"SCHEDE CONTROLLO",

	"LINGUA",	
	"ITALIANO",
	"ENGLISH",
	"ESPANOL",
	
	"BUZZER",
	"OFF",
	"ON",

	"BAUD RATE",
	"115200",
	"19200",
	"9600",

	"DISPLAY",
	"CONTRASTO",
	"ILLUMINAZIONE",
	
	"ESC",
	
	"STERILIZZATORE",
	"CALENDARIO STERILIZZATORE",

	"ANTINCENDIO",
	"SENSORE UMIDITA'",

	"AGGIORNAMENTO FW",
	"AGGIOR. FW SLAVE",

	"PROCEDERE CON\n L'AGGIORNAMENTO\n DEL FIRMWARE?",
	
	"OROLOGIO",
	"TACHO VENTOLE",
	"CONTA ORE",

	"ORE TOTALI:",
	"ORE PARZIALI:",
	"Per azzerare il parziale\npremere ON per 5 sec",

	"STAGIONE",
	"TERMOSTATO",
	"QUALITA' ARIA",
	"SOGLIA CO2",
	"SOGLIA UMIDITA'",
	"SOGLIA TEMPERATURA",

	"VENTOLA 1",
	"VENTOLA 2",
	"MIN VENTOLA",
	"MAX VENTOLA",

	"SALVATAGGIO\n       IN CORSO",

	"INVERNO",
	"ESTATE",

	"NO",
	"SONDA 0-2000",
	"SONDA 0-5000",

	"LUN",
	"MAR",
	"MER",
	"GIO",
	"VEN",
	"SAB",
	"DOM",

	"ERRORE",
	"COMUNICAZIONE",

	"PROGRAMMI",

	"LUNEDI' P1",
	"LUNEDI' P2",
	"MARTEDI' P1",
	"MARTEDI' P2",
	"MERCOLEDI' P1",
	"MERCOLEDI' P2",
	"GIOVEDI' P1",
	"GIOVEDI' P2",
	"VENERDI' P1",
	"VENERDI' P2",
	"SABATO P1",
	"SABATO P2",
	"DOMENICA P1",
	"DOMENICA P2",

	"PROGRAMMA",
	"INIZIO",
	"FINE",

	"ORARIO",
	"NON VALIDO",

	"LOGO",
	"NESSUN LOGO",
	"ASPIRNOVA",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	"MANUALE",
	"AUTOMATICO",
	"PROGRAMMA",
	"RUN",
	"STOP",

	"IMPOSTAZIONI",

	"SALVA",
	"ANNULLA",

	"ALLARME",
	"ANTINCENDIO",

	"SOSTITUIRE FILTRO ARIA",
	"ALLARME SANIFICAZIONE",

	"VISUAL.RISCALDATORE",

	"MODALITA'",
	"STANDARD",
	"FILL BOX"

};


const char * _table_INGLESE_[] =
{
	"NO",
	"YES",

	"OK!",
	"ERROR",

	"MENU",

	"CONTROL BOARDS",

	"LANGUAGE",
	"ITALIANO",
	"ENGLISH",
	"ESPANOL",

	"BUZZER",
	"OFF",
	"ON",

	"BAUD RATE",
	"115200",
	"19200",
	"9600",

	"DISPLAY",
	"CONTRAST",
	"BACKLIGHT",

	"ESC",

	"STERILIZER",
	"STERILIZER CALENDAR",
	"FIRE FIGHTING",
	"HUMIDITY SENSOR",

	"FW UPGRADE",
	"SLAVE FW UPGRADE",
	"PROCEED WITH THE\n FIRMWARE UPGRADE?",

	"CLOCK",
	"TACHO FAN",
	"HOUR METERS",

	"TOTAL HOURS:",
	"PARTIAL HOURS:",
	"To reset the partial\npress ON for 5 sec",

	"SEASON",
	"THERMOSTAT",
	"AIR QUALITY",
	"CO2 SETPOINT",
	"HUMIDITY SETPOINT",
	"ROOM SETPOINT",

	"FAN 1",
	"FAN 2",
	"MIN FAN",
	"MAX FAN",

	"   SAVING",

	"WINTER",
	"SUMMER",

	"NO",
	"PROBE 0-2000",
	"PROBE 0-5000",


	"MON",
	"TUE",
	"WED",
	"THU",
	"FRI",
	"SAT",
	"SUN",

	"COMMUNICATION",
	"ERROR",

	"PROGRAMS",

	"MONDAY P1",
	"MONDAY P2",
	"TUESDAY P1",
	"TUESDAY P2",
	"WEDNESDAY P1",
	"WEDNESDAY P2",
	"THURSDAY P1",
	"THURSDAY P2",
	"FRIDAY P1",
	"FRIDAY P2",
	"SATURDAY P1",
	"SATURDAY P2",
	"SUNDAY P1",
	"SUNDAY P2",

	"PROGRAM",
	"START",
	"STOP",

	"INVALID",
	"TIME",

	"LOGO",
	"NO LOGO",
	"ASPIRNOVA",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	"MANUAL",
	"AUTOMATIC",
	"SCHEDULER",
	"RUN",
	"STOP",

	"SETTINGS",

	"SAVE",
	"ANNUL",

	"FIRE",
	"FIGHTING",

	"REPLACE AIR FILTER",
	"SANIFICATION ALARM",

	"DISPLAY HEATER",

	"MODE",
	"STANDARD",
	"FILL BOX"
};




const char * _table_SPAGNOLO_[NUM_MSG] =
{
    "NO",
    "SI",

	"OK!",
	"ERROR",

	"MENU",

	"TARJETA DE CONTROL",

	"IDIOMA",
	"ITALIANO",
	"ENGLISH",
	"ESPANOL",

	"BUZZER",
	"OFF",
	"ON",

	"VELOCIDAD DE TRANSMISION",
	"115200",
	"19200",
	"9600",

	"PANTALLA",
	"CONTRASTE",
	"ILUMINACION",

	"ESC",

	"ESTERILIZADOR",
	"CALENDARIO ESTERILIZADOR",

	"ANTIINCENDIOS",
	"SENSOR DE HUMEDAD",

	"ACTUALIZACION FW",
	"ACTUA. FW SLAVE",

	"PROCEDER CON\n LA ACTUALIZACION\n DEL FIRMWARE?",

	"RELOJ",
	"TACHO VENTILADOR",
	"CONTADOR DE HORAS",

	"HORAS TOT.:",
	"HORAS PARCIAL:",
	"Para poner a 0 el parcial\npulsar ON 5 segundos",

	"TEMPORADA",
	"TERMOSTATO",
	"CALIDAD DEL AIRE",
	"UMBRELA CO2",
	"UMBRELA HUMEDAD",
	"UMBRELA TEMPERATURA",

	"VENTILADOR 1",
	"VENTILADOR 2",
	"MIN VENTILADOR",
	"MAX VENTILADOR",

	"ALMACENAMIENTO",

	"INVIERNO",
	"VERANO",

	"NO",
	"SONDA 0-2000",
	"SONDA 0-5000",


	"LUN",
	"MAR",
	"MER",
	"JUE",
	"VIE",
	"SAB",
	"DOM",

	"ERROR",
	"COMUNICACION",

	"PROGRAMAS",

	"LUNES P1",
	"LUNES P2",
	"MARTES P1",
	"MARTES P2",
	"MIERCOLES P1",
	"MIERCOLES P2",
	"JUEVES P1",
	"JUEVES P2",
	"VIERNES P1",
	"VIERNES P2",
	"SABADO P1",
	"SABADO P2",
	"DOMINGO P1",
	"DOMINGO P2",

	"PROGRAMA",
	"INICIO",
	"STOP",

	"HORARIO",
	"INVALIDO",

	"LOGOTIPO",
	"NO LOGOTIPO",
	"ASPIRNOVA",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	"MANUAL",
	"AUTOMATICO",
	"PROGRAMA",
	"RUN",
	"STOP",

	"CONFIGURACION",

	"GUARDAR",
	"CANCELAR",

	"ANTIINCENDIOS",
	" ",

	"REEMPLAZAR FILTRO DE AIRE",
	"ALARMA SANEAMIENTO",

	"MUESTRA CALENTADOR",

	"MODALIDAD",
	"STANDARD",
	"FILL BOX"
};




/** Puntatore alla tabella di lingua utilizzata	*/
char **tabMsg = (char **)_table_ITALIANO_;


/** La funzione setta il puntatore della lingua alla tabella della lingua richiesta.
    \param Language
    Indice della lingua da utilizzare.
*/
void SetLanguage(unsigned char lang)
{
	Language = lang;
    
    switch(Language)
    {

		default:
		case INGLESE:
			tabMsg = (char **)_table_INGLESE_;
		break;

		case ITALIANO:
			tabMsg = (char **)_table_ITALIANO_;
        break;

        case SPAGNOLO:
            tabMsg = (char **)_table_SPAGNOLO_;
        break;
    }

}

unsigned char GetLanguage(void)
{
	return Language;
}

