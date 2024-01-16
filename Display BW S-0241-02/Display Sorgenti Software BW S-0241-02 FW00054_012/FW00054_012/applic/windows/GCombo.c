#include "windows.h"
#include "GMenu.h"
#include "GCombo.h"
#include "msg.h"
#include "main.h"
#include "maintimer.h"
#include "board.h"
#include "tastiera.h"




static unsigned char refresh = FALSE;
static unsigned short indice;

static unsigned char numParam;


unsigned char GCombo_Init(struct COMBO_BOX *m, unsigned char param, tCursorMode *cursorMode)
{

	numParam = param;
	
	*cursorMode = CURSOR_MODE_COMBO;
	
	refresh = TRUE;
	
	if(*m->val > m->num_voci)
		*m->val = 0;   
	indice = *m->val;

	return 0;
}





unsigned char GCombo_Run(struct COMBO_BOX *m, tCursorMode *cursorMode)
{
	
	switch (Dati.key)
	{
		case (SKEY_DS_INC):
			refresh = TRUE;
			if(++indice >= m->num_voci)
				indice = 0;
		break;
		
		case (SKEY_DS_DEC):
			refresh = TRUE;
			if(indice)
				--indice;
			else
				indice = (m->num_voci - 1);
		break;       
		
		case (DKEY_ESC):
			GMenu_Init(numParam, cursorMode);
		break;
		
		case (DKEY_ENTER):
			GMenu_Init(numParam, cursorMode);
			*m->val = indice;
		break;
	}			


	
	return(indice);
}




void GCombo_Draw(struct COMBO_BOX *m)
{
	char str[25];
	
	if (!refresh)
		return;
	
	refresh = FALSE;
	
	// Cancello la stringa
	gsetmode(GNORMAL);
	gsetpos(m->x, m->y);     
	memset(str, ' ', sizeof(str));
	str[20] = 0;
	gputs((PGCSTR)str); 


	// Aggiorno la voce   
	gsetmode(GINVERSE);
	gsetpos(m->x, m->y);     
	strcpy(str, tabMsg[m->inizio_voci + indice]);
	gputs((PGCSTR)str);      
	gsetmode(GNORMAL);	
}

