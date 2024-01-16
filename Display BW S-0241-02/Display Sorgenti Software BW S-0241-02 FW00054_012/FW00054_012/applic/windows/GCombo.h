/**	\file   GCombo.h
 *
 *	\brief  Gestione voce Combo
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
 #ifndef _GCOMBO_H_
#define _GCOMBO_H_

#include "dati.h"


struct COMBO_BOX
{
   unsigned char x;         
   unsigned char y;         	

   unsigned char  *val;       	// Valore di partenza e attuale    
   unsigned short inizio_voci;  // numero di partenza della readMsg
   unsigned short num_voci;		// numero finale della readMsg

};



unsigned char GCombo_Init(struct COMBO_BOX *m, unsigned char param, tCursorMode *cursorMode);

unsigned char GCombo_Run(struct COMBO_BOX *m, tCursorMode *cursorMode);

void GCombo_Draw(struct COMBO_BOX *m);



#endif
