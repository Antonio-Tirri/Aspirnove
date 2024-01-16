/**	\mainpage FW00055 - ASPIRNOVA Scheda Controllo
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *	\version \b FW00055.001 - 29/03/2019
 *	- Versione demo Aspirnova
 *
 *	\version \b FW00055.002 - 04/04/2019
 *	- Versione con modifiche richieste dal cliente
 *
 *	\version \b FW00055.003 - 16/04/2019
 *	- Aggiunto gestione orari
 *
 *	\version \b FW00055.004 - 16/09/2019
 *	- Aggiunto ritardo spegnimento ventole: vengono spente 30 secondi dopo che il sistema è andato in OFF.
 *
 *	\version \b FW00055.005 - 26/09/2019
 *	- Ritardo spegnimento ventole: da gestire soltanto se stavo riscaldando (uscita BATT attiva).
 *
 *	\version \b FW00055.006 - 10/10/2019
 *	- Abilitato pull up sugli ingressi DIP Switch
 *
 *	\version \b FW00055.007 - 19/12/2019
 *	- Portato i valori di default delle ventole al 100%
 *
 *	\version \b FW00055.008 - 12/08/2020
 *		- Impostato micro corretto nel progetto: STM32F205RC (256K) anzichè STM32F205RB (128K). A parte il primo prototipo, tutti i lotti prodotti utilizzavano già questo micro.
 *		- Rimosso ottimizzazione. Nelle precedenti versioni veniva utilizzato "Optimize (-O1)". Durante le modifiche per HARDWARE 2020 a volte non comunicava correttamente con il display.
 *		- Gestione HARDWARE_S0283 (7 dip anzichè 4, 2 pin per identificare l'hardware, TACH_F1 e TACH_F2 per misurare la velocità dei motori. Mantenuta compatibilità con hardware S0242):
 *			- Rimosso gestione RELE_FCOM in quanto non necessario (era attivo su azionamento ventole)
 *			- Rimosso UART5 di debug. Al posto definito i pin PC12 (ex UART5_TX) e PD2 (ex UART5_RX) come ingressi per rilevare revisione hardware (entrambi a 0 --> HARDWARE_2020, entrambi a 1 --> HARDWARE_2019)
 *			- Aggiunto DIP_5 alla selezione ID scheda (possibilità di selezionare gli ID da 1 a 32. Nelle precedenti versioni arrivavo solo a 16)
 *			- Aggiunto DIP_6 e DIP_7 per selezionare il baud rate della 485 (DIP_6=OFF DIP_7=OFF --> 115200, DIP_6=ON DIP_7=OFF --> 9600, DIP_6=OFF DIP_7=ON --> 19200)
 *			- Aggiunto ingressi interrupt TACH_F1 (PB0) e TACH_F2 (PB1) per leggere le velocità ventole
 *
 *	\version \b FW00055.009 - 25/09/2020
 *		- Aggiunto gestione ingresso allarme sanificazione (IO_DIN_HV2)
 *		- Aggiunto lo stato dell'allarme sanificazione, nel registro di stato inviato su modbus.
 *
 *	\version \b FW00055.010 - 25/01/2021
 *		- Corretto caricamento registri MODBUS 8 (AUX1) e 9 (AUX2)
 *		- Corretto calcolo percentuale umidità da caricare nel registro MODBUS 81
 *
 *	\version \b FW00055.011 - 09/02/2021
 *		- Modificato le percentuali di default di ventola 1 e ventola 2: adesso sono entrambe a 100% mentre nelle precedenti versioni erano rispettivamente a 28% e 42%.
 *
 *	\version \b FW00055.012 - 04/06/2021
 *		- Aggiunto gestione riscaldamento iniziale: ogni volta che il sistema va in ON, se la temperatura è inferiore a -5°C viene attivata al 100% per 2 minuti la sola ventola 2.
 *			Tutte le altre uscite (ventola 1, batt, h20)  rimangono in OFF.
 *		- Durante i 2 minuti di riscaldamento, viene attivato il bit STATUS_INITIAL_WARM_UP all'interno del registro REGISTER_STATUS.
 *
 *	\version \b FW00055.013 - 13/07/2021
 *	- Se non è attiva la funzione sterilizzatore, l'uscita RELE_AUX1 si attiva quando il sistema è in ON (con 2 secondi di ritardo) e si disattiva quando il sistema è in OFF.
 *	- Aggiunto gestione Fill Box
 *
 *	\version \b FW00055.014 - 17/02/2022
 *	- Aggiunto registro modbus soglia umidità.
 *	- Aggiunto PID su soglia umidità nel caso di sensore umidità presente.
 *
 *	\version \b FW00055.015 - 04/05/2022
 *	- Modalità programmi: per ogni orario del calendario aggiunto modalità lavoro (auto/manuale) e percentuale ventole.
 *
 *	\version \b FW00055.016 - 11/01/2023
 *	- Aggiunto gestione contaore totale e parziale (filtro)
 *
 *
 *
 *	<br> <br>
 *	<b>Funzionamento</b> \n
 *	<br>
 * 
 *	MANUALE / INVERNO: \n
 *	Se sistema in ON: (per pressione tasto o logica termostato abilitata)\n
 *	Ventola 1 = Percentuale Mandata ventola\n
 *	Ventola 2 = Percentuale Ritorno ventola\n
 *
 *	Uscita H2O: PID di regolazione se Temperatura di riferimento > temperatura interna\n
 *
 *	Se uscita H2O attiva, abilito uscita RELE_BATT ed imposta PWM BATT = PWM H2O (RELE BATT E PWM BATT utilizzati per riscaldare)\n
 *
 *	Relè bypass attivabile tramite tasto BP.\n
 *
 *
 *	MANUALE / ESTATE: \n
 *	Se sistema in ON: (per pressione tasto o logica termostato abilitata)\n
 *	Ventola 1 = Percentuale Mandata ventola\n
 *	Ventola 2 = Percentuale Ritorno ventola\n
 *
 *	Uscita H2O: PID di regolazione se Temperatura di riferimento < temperatura interna\n
 *
 *	RELE BATT E PWM BATT sempre disabilitati\n
 *
 *	Relè bypass attivabile tramite tasto BP.\n
 *
 *
 *	AUTOMATICO / INVERNO: \n
 *	Se sistema in ON: (per pressione tasto o logica termostato abilitata)\n
 *	Ventola 1 = Percentuale Mandata ventola\n
 *	Ventola 2 = Percentuale Ritorno ventola\n
 *
 *	Uscita H2O: PID di regolazione se Temperatura di riferimento > temperatura interna\n
 *
 *	Se uscita H2O attiva, abilito uscita RELE_BATT ed imposta PWM BATT = PWM H2O (RELE BATT E PWM BATT utilizzati per riscaldare)\n
 *
 *	Se gestione CO2 abilitata e valore CO2 > soglia CO2, abilito PID sulle uscite ventole tra percentuale impostata (mandata e ritorno) e massima percentuale ventola.
 *	Se sensore umidità abilitato e percentuale umidità > soglia umidità, abilito PID sulle uscite ventole tra percentuale impostata (mandata e ritorno) e massima percentuale ventola.
 *
 *	Relè bypass attivo se temperatura interna < temperatura esterna e temperatura interna < temperatura di riferimento.\n
 *  Se valore CO2 > soglia CO2 --> relè bypass sempre spento.
 *
 *
 *	AUTOMATICO / ESTATE: \n
 *	Se sistema in ON: (per pressione tasto o logica termostato abilitata)\n
 *	Ventola 1 = Percentuale Mandata ventola\n
 *	Ventola 2 = Percentuale Ritorno ventola\n
 *
 *	Uscita H2O: PID di regolazione se Temperatura di riferimento < temperatura interna\n
 *
 *	RELE BATT E PWM BATT sempre disabilitati\n
 *
 *	Se gestione CO2 abilitata e valore CO2 > soglia CO2, abilito PID sulle uscite ventole tra percentuale impostata (mandata e ritorno) e massima percentuale ventola.
 *	Se sensore umidità abilitato e percentuale umidità > soglia umidità, abilito PID sulle uscite ventole tra percentuale impostata (mandata e ritorno) e massima percentuale ventola.
 *
 *	Relè bypass attivo se temperatura interna > temperatura esterna e temperatura interna > temperatura di riferimento.\n
 *  Se valore CO2 > soglia CO2 --> relè bypass sempre spento.
 *
 *
 *
 *	\n\n\n
 *
 *	MODALITA' PROGRAMMA: il sistema va in ON ad orari definiti (possibilità di inserire due orari per ogni giorno della settimana).\n
 *	Oltre all'orario è possibile definire il comportamento del sistema durante l'attivazione (manuale o automatico) e le percentuali di comando da applicare alle ventole.\n
 *
 *	<br> <br>
 *	<b>LED</b> \n
 *	<br>
 * 
 *	LED ON: cambia stato ogni 250 ms \n
 *	LED_PC1: cambia stato ad ogni ricezione di un pacchetto MODBUS \n
 *	LED_PC2: cambia stato ad ogni scrittura in flash \n
 *	LED_ERR: acceso se flash ripristinata con i valori di default \n
 *
 *	<br> <br> <br> <br>
 *
 *  \page Dettagli Dettagli
 *
 * 	<b>Micro</b>\n
 *		- STM32F205RC
 *
 * 	<b>Compilatore utilizzato</b>\n
 *		- TrueSTUDIO for STM32 v9.0.0
 *
 *  <b>Hardware compatibile</b>\n
 *		LB00104
 * 
 *	<br><br>
 *

 *
 *
 *  <b>Compatibilita' con il bootloader: </b>\n
 *		- Impostare, in TrueSTUDIO\FW00055\STM32F205RC_FLASH_APPLIC.ld, la seguente riga per il dimensionamento della flash: \n
 *			FLASH (rx)      : ORIGIN = 0x8008000, LENGTH = 96K
 *		- Impostare VECT_TAB_OFFSET = 0x8000 in system_stm32f2xx.c
 *		- Definire USE_BOOTLOADER in application.h
 * 
 * 	<b>Rilascio versione</b>\n
 *		- Generare la documentazione del progetto, lanciando il file batch doc\genera_doc.bat
 *
 * 	<b>Note</b>\n
 *  - Ottimizzazione impostata a "None (-O0)"
 *
 */

// Application info
#define APP_NAME		"FW00055"
#define APP_VERSION		"016"
#define APP_BUILD		"(Build: " __DATE__ " - " __TIME__ ")"
#define APP_COMPANY		"lab3841"
#define APP_AUTHOR		"W.Giusiano"
#define APP_HARDWARE	"LB00104"




// Commentare la define se non si intende utilizzare il bootloader (x debug)
#define USE_BOOTLOADER			1




// Time main cycle, in ms
#define TIME_MAIN_CYCLE		10

#define DIP_FILTER			(300 / TIME_MAIN_CYCLE)		// Impostare un tempo superiore ad una lettura AD completa con oversampling (160ms). In questo modo il sistema va in STATUS_WORK con dei valori validi

#define TIMEOUT_TACHO		(5000 / TIME_MAIN_CYCLE)



/** Gestione applicazione. */
void Application_Run(void);


/** Da richiamare nel main */
void Application_Poll(void);
