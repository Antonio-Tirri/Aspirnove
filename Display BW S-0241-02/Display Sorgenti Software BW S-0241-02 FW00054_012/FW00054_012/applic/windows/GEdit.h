/**	\file   geDIT.h
 *
 *	\brief  Gestione voce campo Edit
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *
 *  \version    v1.0 - 05/05/2015
 * 	-   Versione iniziale
 *
 *
 */
 #ifndef _GEDIT_H_
#define _GEDIT_H_

#include "dati.h"


enum
{
	E_STR_NUM = 0,
	E_SHORT,
	E_USHORT,
	E_DOUBLE,
	E_FLOAT,
	E_STR,
	E_PSW,
	E_UPDW,
	E_COMBO,
	E_TIME
};




struct EDIT
{
   unsigned char x;           //
   unsigned char y;           //
   unsigned char tipo;        // 0=    
   unsigned char len;
   float min;
   float max;
   float Incremento;
   void *dato; 
   char *fmt;                      //Puntatore al formato dei dati es."%3.0f"
   void (*fun)(void *,unsigned short);//Puntatore a una funzione generica
};



unsigned char GEdit_Init(struct EDIT *e, unsigned char param, tCursorMode *cursorMode);

unsigned char GEdit_Run(struct EDIT *e, tCursorMode *cursorMode);

void GEdit_Draw(struct EDIT *e);

unsigned char GEditRefresh(char *str,char *x, char *y,struct EDIT *eu);
short NumeroCaratteri(char *fmt);

#endif

