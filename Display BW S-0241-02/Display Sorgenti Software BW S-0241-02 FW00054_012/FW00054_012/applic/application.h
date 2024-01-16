/**	\mainpage FW00054 - ASPIRNOVA Scheda Display Biano-Nero
 *
 * 	\author Walter Giusiano - lab3841 srl \n
 *  walter.giusiano@lab3841.it
 *
 *
 *	\version \b FW00054.001 - 29/03/2019
 *	- Versione demo Aspirnova
 *
 *	\version \b FW00054.002 - 04/04/2019
 *	- Versione con modifiche richieste dal cliente
 *
 *	\version \b FW00054.003 - 16/04/2019
 *	- Aggiunto gestione orari
 *
 *	\version \b FW00054.004 - 27/06/2019
 *	- Impostato numero slave di default = 1 anzichè 10
 *	- Aggiunto, nel menu di programmazione del display, la possibilità di selezionare il logo da visualizzare all'accensione. (NESSUNO - ASPIRNOVA)
 *
 *	\version \b FW00054.005 - 26/09/2019
 *	- Aumentato il tempo pressione tasti per accedere al menu amministratore da 2 a 15 secondi.
 *
 *	\version \b FW00054.006 - 19/12/2019
 *	- Ridotto il tempo pressione tasti per accedere al menu amministratore da 15 a 5 secondi.
 *	- Nel menu invertito le diciture di mandata ventola e ritorno ventola. Da questa versione: Mandata Ventola = FAN1, Ritorno Ventola = FAN2, .
 *
 *	\version \b FW00054.007 - 13/11/2020
 *	- COMPATIBILE SOLO CON VERSIONI BOOTLOADER FW00052.002 O SUCCESSIVE
 *	- Impostato micro corretto nel progetto: STM32F205RC (256K) anzichè STM32F205RB (128K). A parte il primo prototipo, tutti i lotti prodotti utilizzano già questo micro.
 *	- Modificato la grafica della videata principale per creare spazio per le nuove funzioni
 *	- Gestione tasti come su display a colore (frecce + tasto modifica/conferma)
 *	- Baud rate configurabile da menu display (115200 - 9600 - 19200)
 *	- Aggiunto possibilità di disabilitare il buzzer (da menu display)
 *	- Aggiunto gestione ingressi ritorno motori (se la scheda di controllo è una 0283)
 *	- Aggiunto videata ventole per vedere il valore degli ingressi ritorno motori
 *	- Modificato segnalazioni allarmi generici (led e campana + messaggio di errore)
 *	- Aggiunto funzione antincendio
 *	- Aggiunto funzioni sterilizzatore e ingresso sanificazione
 *	- Aggiunto lingua spagnolo
 *	- Aggiunto visualizzazione percentuale umidità (letta dal sensore sulla scheda di controllo)
 *	- Modificato funzione standby: mantengo la videata di lavoro riducendo soltanto al minimo la retroilluminazione
 *	- Gestione nuovo hardware Rev.02 del 9/11/2020 (6 dip anzichè 4, 2 pin per identificare l'hardware, UART per RS485 slave. Mantenuta compatibilità con revisioni hardware precedente)
 *
 *	\version \b FW00054.008 - 25/01/2021
 *	- Aggiunto voce, nel menu di programmazion del display, per impostare o meno la visualizzazione del riscaldatore (icona serpentina + percentuale riscaldatore) nella videata principale.
 *	- Buzzer: attivo di default.
 *
 *	\version \b FW00054.009 - 23/09/2021
 *	- Aggiunto gestione Fill Box
 *	- Modifica visualizzazione percentuali ventole: con valori >= 100 il rettangolo del cursore non veniva correttamente cancellato (rimanevano alcuni pixel a ON a destra del valore).
 *
 *	\version \b FW00054.010 - 17/02/2022
 *	- Aggiunto parametro soglia umidità (voce menu e registro modbus).
 *
 *	\version \b FW00054.011 - 09/05/2022
 *	- Modalità programmi: per ogni orario del calendario aggiunto modalità lavoro (auto/manuale) e percentuale ventole.
 *
 *	\version \b FW00054.012 - 11/01/2023
 *	- Aggiunto voce menu per la visualizzazione dei conta ore.
 *
 *
 *
 *
 *
 *	<br> <br> <br>
 *	<b>Descrizione segnalazioni del led presente su scheda</b>\n
 *	<table>
 *	<tr><th>Stato LED</th>										<th>Segnalazione</th> </tr>
 *	<tr> <td>Alternato ROSSO/ARANCIONE/VERDE</td>	<td><b>Bootloader in esecuzione.</b></td>	</tr>
 *	<tr> <td>SPENTO</td>							<td>Le ventole (di tutte le schede controllo) sono spente o azionate con un comando inferiore al 30%</td>	</tr>
 *	<tr> <td>VERDE</td>								<td>Ventole azionate (su almeno una delle schede di controllo) senza anomalie rilevate</td>	</tr>
 *	<tr> <td>ROSSO</td>								<td>Ventole azionate con ingresso filtro sporco (su almeno una delle schede di controllo) oppure ingresso allarme sanificazione attivo</td>	</tr>
 *	<tr> <td>ARANCIONE</td>							<td>Ventole azionate ma ingressi ritorno ventole non rilevati (su almeno una delle schede di controllo) </td>	</tr>
 *	</table> \n
 *
 *
 *	<b>Menu</b>\n
 *	Per scorrere le voci utilizzare i tasti frecce. Per confermare utilizzare il tasto M. Per abbandonare il menu selezionare la voce ESC oppure premere il tasto ON. \n
 *
 *
 *	\n
 *	\n
 *	******************* \n
 *	Per accedere al menu di programmazione del display, alimentare il sistema mantenendo il tasto M premuto. \n
 *	Voci menu: \n
 *	- SCHEDE CONTROLLO: per impostare il numero di schede controllo collegate al display
 *	- LINGUA: per impostare la lingua di sistema (ITA - ENG - SPA)
 *	- DISPLAY: per regolare contrasto e retroilluminazione
 *	- LOGO: per selezionare il logo da visualizzare all'accensione (NESSUNO - ASPIRNOVA)
 *	- BUZZER: abilita/disabilita l'uscita BUZZER
 *	- BAUD RATE: per selezionare la velocità di comunicazione sulle linee 485 (115200 - 19200 - 9600)
 *	- VISUAL.RISCALDATORE: abilita/disabilita visualizzazione serpentina+percentuale nella videata principale
 *	- MODALITA': imposta modalità di lavoro del display (STANDARD - FILLBOX)
 *	- AGGIORNAMENTO FW: per abilitare il sistema alla ricezione di un nuovo firmware
 *	\n
 *	\n
 *	******************* \n
 *	Per accedere al menu di programmazione delle schede controllo con LIVELLO UTILIZZATORE, spostarsi nella videata relativa alla scheda desiderata e premere per alcuni secondi il tasto M. \n
 *	Voci menu: \n
 *	- VENTOLA 1: percentuale comando ventola 1
 *	- VENTOLA 2: percentuale comando ventola 2
 *	- STAGIONE: Inverno/Estate
 *	- SOGLIA TEMPERATURA: temperatura di riferimento
 *	- SOGLIA CO2: soglia CO2 di riferimento
 *	- SOGLIA UMIDITA': soglia umidità di riferimento
 *	- PROGRAMMI: orari di attivazione nella modalità PROGRAMMI
 *	- OROLOGIO: settaggio orologio presente sulla scheda controllo
 *	- TACHO VENTOLE: videata per il debug degli ingressi ritorno ventole
 *	- CALENDARIO STERILIZZATORE: orari di attivazione della funzione sterilizzatore
 *	- CONTA ORE: visualizza conta ore totali e parziali. Se si accede con LIVELLO AMMINISTRATORE sarà inoltre possibile azzerare le ore parziali.
 *	\n
 *	\n
 *	******************* \n
 *	Per accedere al menu di programmazione delle schede controllo con LIVELLO AMMINISTRATORE, spostarsi nella videata relativa alla scheda desiderata, premere il tasto M e successivamente
 *	il tasto BP e mantenere la pressione di entrambi per 15 secondi. \n
 *	Voci menu: (oltre a quelle già descritte per il livello utilizzatore)
 *	- TERMOSTATO: abilita/disabilita l'ingresso TERMOSTATO
 *	- QUALITA' ARIA: abilita/disabilita il controllo di CO2
 *	- MIN VENTOLA: minima percentuale di comando applicabile alle uscite ventole
 *	- MAX VENTOLA: massima percentuale di comando applicabile alle uscite ventole
 *	- STERILIZZATORE: abilita/disabilita la funzione sterilizzatore / ingresso sanificazione
 *	- ANTINCENDIO: abilita/disabilita la funzione antincendio
 *	- SENSORE UMIDITA': abilita/disabilita il sensore di umidità
 *	- MODALITA': imposta modalità di lavoro della scheda di controllo (STANDARD - FILLBOX)
 *	- AGGIORNAMENTO FW: per abilitare il sistema alla ricezione di un nuovo firmware
 *	\n
 *	\n
 *
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
 *		- Impostare, in TrueSTUDIO\FW00054\STM32F205RC_FLASH_APPLIC.ld, la seguente riga per il dimensionamento della flash: \n
 *			FLASH (rx)      : ORIGIN = 0x8008000, LENGTH = 224K
 *		- Impostare VECT_TAB_OFFSET = 0x8000 in system_stm32f2xx.c
 *		- Definire USE_BOOTLOADER in application.h
 * 
 * 	<b>Rilascio versione</b>\n
 *		- Generare la documentazione del progetto, lanciando il file batch doc\genera_doc.bat
 *
 * 	<b>Note</b>\n
 * 	- Font times13: sostituito carattere $ con °
 *  - Ottimizzazione impostata a "Optimize (-O1)" per ridurre occupazione memoria.
 *
 */

// Application info
#define APP_NAME		"FW00054"
#define APP_VERSION		"012"
#define APP_BUILD		"(Build: " __DATE__ " - " __TIME__ ")"
#define APP_COMPANY		"lab3841"
#define APP_AUTHOR		"W.Giusiano"
#define APP_HARDWARE	"LB00104"




// Commentare la define se non si intende utilizzare il bootloader (x debug)
#define USE_BOOTLOADER			1



// Time main cycle, in ms
#define TIME_MAIN_CYCLE		10

#define TIME_WINDOW_DRAW	100


#define DIP_FILTER			(100 / TIME_MAIN_CYCLE)




/** Gestione applicazione. */
void Application_Run(void);


/** Da richiamare nel main */
void Application_Poll(void);
