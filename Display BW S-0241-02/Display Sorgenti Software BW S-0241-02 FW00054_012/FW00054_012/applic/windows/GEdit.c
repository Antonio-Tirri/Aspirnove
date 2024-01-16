#include "windows.h"
#include "GMenu.h"
#include "GCombo.h"
#include <ctype.h>
#include <math.h>
#include "GEdit.h"
#include "main.h"
#include "application.h"
#include "maintimer.h"
#include "board.h"
#include "tastiera.h"



static unsigned char refresh = FALSE;

static unsigned char numParam;

static char *xp;
static char *yp;
static char *str2;
	
static unsigned int contPress = 0;	
	
	
	
/** Metodo interno. Gestione incremento su pressione mantenuta del tasto */
static float gstRepeat(float val)
{
	if (contPress % (50 / TIME_MAIN_CYCLE) == 0)
	{
		if (contPress > (10000 / TIME_MAIN_CYCLE))
			return (val * 1000);
		else if (contPress > (6000 / TIME_MAIN_CYCLE))
			return (val * 100);
		else if (contPress > (3000 / TIME_MAIN_CYCLE))
			return (val * 10);
		else if (contPress > (1000 / TIME_MAIN_CYCLE))
			return (val);
	}

	if (contPress == 1)
		return val;

	return 0;
}	





unsigned char GEdit_Init(struct EDIT *e, unsigned char param, tCursorMode *cursorMode)
{
	unsigned short d;
	
	numParam = param;

	*cursorMode = CURSOR_MODE_EDIT;

	refresh = TRUE;
	
	gsetpos(e->x,e->y);  //Posiziona il cursore
              
	str2 = (char*)malloc(e->len);
	xp = (char*)malloc(e->len);
	yp = (char*)malloc(e->len);
      

	switch(e->tipo)
	{
		case E_STR:
			StrCpyMax(&str2[0], (char *)e->dato, e->len);
		break;
		
		case E_STR_NUM :
			StrCpyMax(&str2[0], (char *)e->dato, e->len);
		break;
		
		case E_SHORT:
			sprintf(str2, e->fmt, *(short *)e->dato);
		break;

		case E_USHORT:
			sprintf(str2, e->fmt, *(unsigned short *)e->dato);
		break;

		case E_FLOAT:
			sprintf(str2, e->fmt, *(float *)e->dato);
		break;
		
		case E_DOUBLE:
			sprintf(str2, e->fmt, *(double *)e->dato);
		break;
		
#if 0
		case E_LONG:
			sprintf(str2, e->fmt, *(long *)e->dato);
		break;
#endif		
		case E_TIME:
			d = *(int *)e->dato;
			sprintf(str2, e->fmt, (d / 60), (d % 60));
		break;

	}
		
	return 0;
}



unsigned char GEdit_Run(struct EDIT *e, tCursorMode *cursorMode)
{
	short updwShort;
	unsigned short updwUShort;
	unsigned short d;

	
	switch (Dati.key)
	{
		
		/////////////////////////////////////
		default:
			contPress = 0;
		break;

		case (SKEY_DS_INC):
			contPress = 0;	 // Azzera alla prima pressione
		case (SKEY_DS_INC_PRESS):

			++contPress;

			switch(e->tipo)
			{
				case E_SHORT:
					updwShort = atoi(&str2[0]);
					if ( ((short)updwShort + gstRepeat(e->Incremento) ) > e->max)
						updwShort = e->max;
					else
						updwShort += gstRepeat(e->Incremento);

					sprintf(&str2[0],e->fmt,updwShort);

					refresh = TRUE;
                break;

				case E_USHORT:
					updwUShort = atoi(&str2[0]);
					if ( ((unsigned long)updwUShort + gstRepeat(e->Incremento) ) > e->max)
						updwUShort = e->max;
					else
						updwUShort += gstRepeat(e->Incremento);

					sprintf(&str2[0],e->fmt, updwUShort);

					refresh = TRUE;
                break;

                case E_FLOAT:
                case E_DOUBLE:
					*(float *)e->dato = *(float *)e->dato + (float)(gstRepeat(e->Incremento));
					if(*(float *)e->dato > e->max)
						*(float *)e->dato = e->max;
					sprintf(&str2[0],e->fmt,*(float *)e->dato);

					refresh = TRUE;
                break;

#if 0
				case E_LONG:
					udwLong = atol(&str2[0]);
					udwLong += gstRepeat(e->Incremento);
					if(udwLong > e->max)
						udwLong = e->max;
					sprintf(&str2[0],e->fmt,udwLong);

					refresh = TRUE;
                break;
#endif
				case E_TIME:
					*(short *)e->dato = *(short *)e->dato + (short)(gstRepeat(e->Incremento));
					if ( (*(short *)e->dato > e->max) || (*(short *)e->dato < 0) )
						*(short *)e->dato = e->max;

					d = *(short *)e->dato;
					sprintf(&str2[0], e->fmt, (d / 60), (d % 60));

					refresh = TRUE;
				break;
            }

		break;

		case (SKEY_DS_DEC):
			contPress = 0;	 // Azzera alla prima pressione
		case (SKEY_DS_DEC_PRESS):

			++contPress;

			switch(e->tipo)
			{
				case E_SHORT:
					updwShort = atoi(&str2[0]);
					updwShort -= gstRepeat(e->Incremento);
					if(updwShort <= e->min)
						updwShort = e->min;
					sprintf(&str2[0],e->fmt,updwShort);

					refresh = TRUE;
				break;

				case E_USHORT:
					updwUShort = atoi(&str2[0]);

					if (gstRepeat(e->Incremento) > updwUShort)
						updwUShort = 0;
					else
						updwUShort -= gstRepeat(e->Incremento);

					if(updwUShort <= e->min)
						updwUShort = e->min;
					sprintf(&str2[0],e->fmt,updwUShort);

					refresh = TRUE;
				break;

				case E_FLOAT:
				case E_DOUBLE:
					//updwDouble = atof(&str2[0]);
					*(float *)e->dato = *(float *)e->dato - (float)(gstRepeat(e->Incremento));
					if(*(float *)e->dato <= e->min)
						*(float *)e->dato = e->min;
					sprintf(&str2[0],e->fmt,*(float *)e->dato);

					refresh = TRUE;
				break;

#if 0
				case E_LONG:
					udwLong = atol(&str2[0]);
					udwLong -= gstRepeat(e->Incremento);
					if(udwLong <= e->min)
						udwLong = e->min;
					sprintf(&str2[0],e->fmt,udwLong);

					refresh = TRUE;
				break;
#endif

				case E_TIME:
					*(short *)e->dato = *(short *)e->dato - (short)(gstRepeat(e->Incremento));
					if(*(short *)e->dato <= e->min)
						*(short *)e->dato = e->min;
					d = *(short *)e->dato;
					sprintf(&str2[0], e->fmt, (d / 60), (d % 60));

					refresh = TRUE;
				break;
			}

		break;
		 
		
		case (DKEY_ESC):
			GMenu_Init(numParam, cursorMode);
		break;
			
		case (DKEY_ENTER):
			switch(e->tipo)
			{
				
				case E_STR:
					StrCpyMax((char *)e->dato,&str2[0],e->len);           
                break;
				
				case E_STR_NUM:
					StrCpyMax((char *)e->dato,&str2[0],e->len);
				break;
               
				case E_USHORT:    
					*(unsigned short *)e->dato = atoi(str2);
					if(*(unsigned short *)e->dato > e->max)
					*(unsigned short *)e->dato = e->max;
					if(*(unsigned short *)e->dato < e->min)
					*(unsigned short *)e->dato = e->min;
				break;

				case E_SHORT:
					*(short *)e->dato = atoi(str2);
					if(*(short *)e->dato > e->max)
					*(short *)e->dato = e->max;
					if(*(short *)e->dato < e->min)
					*(short *)e->dato = e->min;
				break;
					
				case E_FLOAT:
					*(float *)e->dato = atof(str2);
					if(*(float *)e->dato > e->max)
					*(float *)e->dato = e->max;
					if(*(float *)e->dato < e->min)
					*(float *)e->dato = e->min;
				break;
					
				case E_DOUBLE:
					*(double *)e->dato = atof(str2);
					if(*(double *)e->dato > e->max)
					*(double *)e->dato = e->max;
					if(*(double *)e->dato < e->min)
					*(double *)e->dato = e->min;
				break;

#if 0					
				case E_LONG:
					*(long *)e->dato = atol(str2);
					if(*(long *)e->dato > e->max)
					*(long *)e->dato = e->max;
					if(*(long *)e->dato < e->min)
					*(long *)e->dato = e->min;
				break;
#endif
					
				case E_TIME:    
					if(*(short *)e->dato > e->max)
					*(short *)e->dato = e->max;
					if(*(short *)e->dato < e->min)
					*(short *)e->dato = e->min;
				break;
			}       
			
			GMenu_Init(numParam, cursorMode);
		break;
			
	}			

	return(0);
}




void GEdit_Draw(struct EDIT *e)
{
	
	if (!refresh)
		return;
	
	refresh = FALSE;
	

	GEditRefresh(&str2[0], xp, yp, e);
          
         
}






unsigned char GEditRefresh(char *str,char *x, char *y,struct EDIT *eu)
{
	uint8_t i;
	unsigned char PntCar=0;
	short    s,len;
	unsigned short us;
	float    f;
	double   d;

	gsetpos(eu->x,eu->y);  //Posiziona il cursore
	len = NumeroCaratteri(eu->fmt);

	for(i = 0; i < len;++i)
		gputch(' ');   

	gsetpos(eu->x,eu->y);  //Posiziona il cursore

	switch(eu->tipo)
	{

		case E_SHORT:
			s = atoi(str);      
			if(s > eu->max)
			{
				s = eu->max;
				sprintf(str,eu->fmt,s);
			}
			if(s < eu->min)
			{
				s = eu->min;
				sprintf(str,eu->fmt,s);
			}

		break;
			
		case E_USHORT:
			us = atoi(str);      
			if(us > eu->max)
			{
				us = eu->max;
				sprintf(str, eu->fmt, us);
			}
			if(us < eu->min)
			{
				us = eu->min;
				sprintf(str, eu->fmt, us);
			}

		break;
		
		case E_FLOAT:
			f = atof(str);
			if(f > eu->max)
			{
				f = eu->max;
				sprintf(str,eu->fmt,f);
			}
			else
			{
				if(f < eu->min)
				{
					f = eu->min;
					sprintf(str,eu->fmt,f);
				}
			}
		break;
		
		case E_DOUBLE:
			d = atof(str);
			if(d > eu->max)
			{
				d = eu->max;
				sprintf(str,eu->fmt,d);
			}
			if(d < eu->min)
			{
				d = eu->min;
				sprintf(str,eu->fmt,d);
			}

		break;

#if 0
		case E_LONG:
			l = atol(str);
			if(l > eu->max)
			{
				l = eu->max;
				sprintf(str,eu->fmt,l);
			}
			if(l < eu->min)
			{
				l = eu->min;
				sprintf(str,eu->fmt,l);
			}
		break;
#endif
			
		case E_TIME:
			s = *(short *)eu->dato;
			sprintf(str, eu->fmt, (s / 60), (s % 60));
		break;
	}                           
   

	gsetmode(GINVERSE);
	for(i=0;str[i] != 0;++i)
	{//Visualizza la stringa sul display e memorizza le coordinate
		if(i >= eu->len)
		{//Troppo lunga
			str[i] = 0;
			gsetmode(GNORMAL);
			return(PntCar);
		}

		x[i]=ggetxpos();
		y[i]=ggetypos();
		gputch(str[i]);      
		++PntCar;//ritorno

		if(str[i+1] == 0)
		{
			x[i+1]=ggetxpos();
			y[i+1]=ggetypos();
		}
	}
	
	gsetmode(GNORMAL);

	return(PntCar);
}



/* -------------------------------------------------------------------------- */
/* Name: NumeroCaratteri                                                      */
/*                                                                            */
/* Input parameter:                                                           */
/*      char        *fmt;       - Formato della stringa                       */
/*                                                                            */
/* Output parameter:                                                          */
/*                                                                            */
/* Decritpion: Restituisce la lunghezza di un campo analizzando il formato    */
/*                                                                            */
/*                                                                            */
/* Return :                                                                   */
/* -------------------------------------------------------------------------- */
short NumeroCaratteri(char *fmt)
{
    short lun, i;
    char str[20];

    fmt++;
    i = 0;
    while(isdigit(*fmt) != FALSE)
        str[i++] = *fmt++;
	
    str[i] = 0;
    lun = atoi(str);
    if(lun == 0)
        lun = 5;
	else
		lun += 2;	// Aggiunge lunghezza 2 spazi al fondo
	
    return(lun);
}


