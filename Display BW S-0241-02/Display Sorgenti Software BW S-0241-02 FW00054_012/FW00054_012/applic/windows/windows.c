#include "windows.h"
#include "display.h"
#include "pwm.h"
#include "dati.h" 
#include "msg.h"

// Lunghezza stringa in pixel
extern SGUINT gi_strlen( PGCSTR sp, SGUCHAR mode );


// Videate
extern tWindow winInit, winMain, winCharger, winStandby, winFiera;


/** Posizone cursore */
int cursor;


/** Livello di password utilizzato */
unsigned char pswLevel;

/** Stringhe temporanee */
char c_string[64];

/** Flag che indica che non è ancora stato eseguito il metodo Run */
static char win_run = 0;



/** Puntatore alla finestra corrente */
static tWindow *current;

/** Puntatore alla finestra precedente */
static tWindow *prev;

/** Puntatore alla finestra successiva */
static tWindow *next;





void Windows_puts(char *str, char mode)
{
	if (mode == INVERSE_MODE)  	gsetmode(GINVERSE);
	else						gsetmode(GNORMAL);
	gputs(str);
}


void Windows_setpos(char x, char y)
{
	gsetpos(x, y);
}


void Windows_Write(char x, char y, char *str, char mode)
{
	gsetpos(x, y);
	Windows_puts(str, mode);
}

void Windows_CenteredWrite(char y, char *str, char mode)
{
	char x;
	
	// Rimuove caratteri in eccesso
	while (Windows_StrLen(str) > 128)
		str[strlen(str)- 1] = 0;

	x = (128 - Windows_StrLen(str)) / 2;
	gsetpos(x, y);
	Windows_puts(str, mode);
}


void Windows_PutImg(unsigned char x, unsigned char y, PGSYMBOL psym, char mode )
{
	if (mode == INVERSE_MODE)  	gsetmode(GINVERSE);
	else						gsetmode(GNORMAL);
	gputsym(x, y, psym);
}

void Windows_rectangle(int x1, int y1, int x2, int y2, char mode)
{
	if (mode == INVERSE_MODE)  	gsetmode(GINVERSE);
	else						gsetmode(GNORMAL);
	grectangle(x1, y1, x2, y2);
}


void Windows_InitBacklight(void)
{
}


void Windows_SetBacklight(int perc)
{
	if (perc < 0)
		perc = 0;
	
	if (perc > 100)
		perc = 100;

	PWM_SetPercent(PWM_BACKLIGHT, perc);
}


void Windows_SetContrast(int perc)
{
	ghw_cont_set(perc);
}


void Windows_Clear(void)
{
	gsetmode(GNORMAL);			  
	gclrvp();
}


unsigned char Windows_StrLen(char *s)
{
	return gi_strlen(s, 0);
}

void Windows_Init(void)
{
	
	Display_Init();	
	
	prev 		= NULL;
	current 	= NULL;
	next 		= NULL;	
	
	SetLanguage(Parameters.Language);

	// Init PWM Backlight (100Hz)
	Windows_InitBacklight();

	// Imposta contrasto
	Windows_SetContrast(Parameters.DisplayContrast);	
	
	// Carica videata iniziale
	Windows_Load(&winInit);	

}


void Windows_Load(tWindow *win)
{
	prev = current;
	current = win;

	if (current->Load != NULL)
		current->Load();			    

	// Metodo Run non ancora eseguito
	win_run = 0;
}


			    
void Windows_Draw(void)
{
	if (current == NULL)
		return;

	// Non visualizza se non ho ancora eseguito almeno una volta il metodo Run
	if (!win_run)
		return;

	if (current->Draw != NULL)
	{
		current->Draw();
	}

}


void Windows_Run(unsigned long cmd)
{
	if (current == NULL)
		return;


		
	// Metodo Run eseguito
	win_run = 1;

	if (current->Run != NULL)
		current->Run(cmd);
}


void Windows_SetNext(tWindow *win)
{
	next = win;
}


void Windows_SetPrev(tWindow *win)
{
	prev = win;
}


void Windows_LoadNext(void)
{
	Windows_Load(next);
}


void Windows_LoadPrev(void)
{
	Windows_Load(prev);
}


tWindow *Windows_GetCurrent(void)
{
	return current;
}










