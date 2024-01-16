/**	\file   windows.h
 *
 *	\brief  Modulo gestione videate
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 05/05/2015
 * 	-   Versione iniziale

 *
 */
#ifndef _WINDOWS_H_
#define _WINDOWS_H_


#include <gdisp.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

// Modalità scrittura su display (normale-invertita)
#define NORMAL_MODE		0
#define INVERSE_MODE	1


// Font
#define BIG_FONT		times13
#define SMALL_FONT		ariel9


#define GetCar()		Dati.key





/** Funzione di callback di caricamento finestra. */
typedef void ( *twindow_load)(void);

/** Funzione di callback di disegno finestra. */
typedef void (* twindow_draw)(void);

/** Funzione di callback di azionamento finestra.
 * 	\param cmd
 * 	Comando da eseguire
 */
typedef void (* twindow_run)(unsigned long cmd);



/** Struttura finestra grafica: contiene le funzioni di riferimento per il disegno e l'esecuzione di comandi. */
typedef struct _tWindow
{
	twindow_load	Load;	/**< Puntatore a funzione di inizializzazione finestra */
	twindow_draw	Draw;	/**< Puntatore a funzione di disegno finestra */
	twindow_run		Run;	/**< Puntatore a funzione di esecuzione finestra */
	unsigned int	Flags;	/**< Flag vari */

} tWindow;


// Funzioni custom videate
extern void winInserimento_Config(char * tit, char *str, uint8_t len);
void winMsg_Set(char * tit, char *str);
void winMsg_SetNextWin(tWindow *nxt);
void winReport_SaveSampleWiring(void);
void winAuto_SetNumCh(uint8_t ch);








/** Stringhe temporanee */
extern char c_string[64];






// Supporto C++
#ifdef __cplusplus
	extern "C" {
#endif
		

/** Init gestore finestre. */
void Windows_Init(void);

/** Carica la finestra corrente.
 * 	\param *win
 * 	Puntatore alla finestra corrente
 */
void Windows_Load(tWindow *win);

/** Disegna la finestra corrente. */
void Windows_Draw(void);

/** Esegue la finestra corrente, ricevendo in ingresso lo stato della tastiera */
void Windows_Run(unsigned long cmd);

/** Imposta il nome della finestra successiva. Affinchè la finestra successiva venga caricata utilizzare il metodo Windows_LoadNext
	\param *win
	Puntatore alla finestra da considerare come finestra successiva
*/
void Windows_SetNext(tWindow *win);

/** Imposta il nome della finestra precedente. Affinchè la finestra precedente venga caricata utilizzare il metodo Windows_LoadPrev.
	Ogni volta che viene eseguita una Window_Load questo valore viene sovrascritto.
	\param *win
	Puntatore alla finestra da considerare come finestra precedente
*/
void Windows_SetPrev(tWindow *win);

/** Carica la finestra successiva. Per impostare la finestra successiva utilizzare il metodo Windows_SetNext */
void Windows_LoadNext(void);

/** Carica la finestra precedente. Per impostare la finestra precedente utilizzare il metodo Windows_SetPrev  */
void Windows_LoadPrev(void);

/** Restituisce la finestra corrente */
tWindow *Windows_GetCurrent(void);



/** Visualizza stringa, con la possibilità di attivare/disattivare l'INVERSE mode 
	\param *str
	Puntatore alla stringa da visualizzare.
	\param mode
	Modalità di visualizzazione: NORMAL_MODE - INVERSE_MODE.
*/
void Windows_puts(char *str, char mode);


/** Posiziona il cursore alle coordinate specificate.
	\param x
	Posizione asse x.
	\param y
	Posizione asse y.

*/
void Windows_setpos(char x, char y);

/** Scrivi stringa.
	\param x
	Coordinata X
	\param Y
	Coordinata Y
	\param str
	Stringa da visualizzare.
	\param mode
	Modalità di visualizzazione: NORMAL_MODE - INVERSE_MODE.
*/
void Windows_Write(char x, char y, char *str, char mode);

/** Scrivi stringa centrata orizzontalmente
	\param Y
	Coordinata Y
	\param str
	Stringa da visualizzare.
	\param mode
	Modalità di visualizzazione: NORMAL_MODE - INVERSE_MODE.
*/
void Windows_CenteredWrite(char y, char *str, char mode);


/** Visualizza immagine, con la possibilità di attivare/disattivare l'INVERSE mode 
	\param x
	Coordinata X
	\param Y
	Coordinata Y
	\param psym
	Immagine da visualizzare.
	\param mode
	Modalità di visualizzazione: NORMAL_MODE - INVERSE_MODE.
*/void Windows_PutImg(unsigned char x, unsigned char y, PGSYMBOL psym, char mode );


/** Disegna un rettangolo 
	\param x1
	Coordinata angolo alto-sinistra
	\param y1
	Coordinata angolo alto-sinistra
	\param x2
	Coordinata angolo basso-destra
	\param y2
	Coordinata angolo basso-destra
	\param mode
	Modalità di visualizzazione: NORMAL_MODE - INVERSE_MODE.

*/
void Windows_rectangle(int x1, int y1, int x2, int y2, char mode);




/** Imposta font da utilizzare sul display */
#define Windows_setfont(font)	 gselfont(font)

/** Pulisce videata */
void Windows_Clear(void);


/** Imposta contrasto display */
void Windows_SetContrast(int perc);

/** Imposta retroilluminazione display */
void Windows_SetBacklight(int perc);


/** Restituisce la lunghezza della stringa in pixel */
unsigned char Windows_StrLen(char *s);


/** Avvia test manuale nella videata winMan 
	\param polo: polo richiesto
	\param time: tempo attivazione singolo polo
	\param bt: TRUE se richiesta da dispositivo BT, FALSE se richiesta da encoder scheda
	\param use_timeout: TRUE interrompe il test dopo il tempo previsto. FALSE = il test viene interrotto soltanto dall'utente
*/
void winMan_Start(uint8_t polo, uint8_t time, uint8_t bt, uint8_t use_timeout);


/** Avvia test automatico nella videata winAuto 
	\param time: tempo attivazione singolo polo
	\param bt: TRUE se richiesta da dispositivo BT, FALSE se richiesta da encoder scheda
*/
void winAuto_Start(uint8_t time, uint8_t bt);

/** Interrompe ciclo automatico 
*/
void winAuto_Stop(void);



// Supporto C++
#ifdef __cplusplus
	}
#endif

	
	
#endif 

