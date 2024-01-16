
#include "GMenu.h"
#include "GCombo.h"
#include "msg.h"
#include "main.h"
#include "application.h"
#include "dati.h"
#include "GEdit.h"

#include "windows.h"
#include "maintimer.h"
#include "board.h"
#include "tastiera.h"
#include "img.h"




#define POSX_START     7  // 19
#define MAX_CHR        20

#define X_START			0	// 9
#define X_SIZE			110	// 120




struct EDIT editMenu;
static struct GMENU menu[30];                   
static unsigned char menu_table[30];            

static char LevelPass = 1;

static unsigned short num_voci = 0;
static unsigned short num_vis = 0;

static unsigned short posizione;  
	
static unsigned char refresh = FALSE;

// Oggetto combo
struct COMBO_BOX combo;






unsigned char GMenu_Init(unsigned char pos, tCursorMode *cursorMode)
{
    unsigned char a = 0, b = 0;
    
    posizione = pos;
    num_voci = 0;
    num_vis = 0;   
	
    // Calcolo del numero massimo di voci
    while(menu[a].Msg != 0)
    {
        if(menu[a].Visibile == 1)
        {
            // Ricalcolo la posizione aggiornata
            if(a >= pos)
            {
                if(posizione == 0)
                    posizione = b;
            }
            menu_table[b++] = a;
            num_vis++;
        }
        a++;
        num_voci++;
    }
    num_voci--;      
    num_vis--;
	
	// Se non ho elementi visibili esco dal menu'
    if(num_vis == 0)
		return (0);  
          
    if(posizione == 0)
    {
        posizione = 1;
    }

	*cursorMode = CURSOR_MODE_NORMAL;
	
	refresh = TRUE;

	return 0;
}




unsigned char GMenu_Run(void (*Fun)(void), tCursorMode *cursorMode)
{
	int enc = 0;


	
	if (Dati.key == 0)
	{
		if (enc > 0)
		{
			if (++posizione > num_vis)
				posizione = 1;
				
			refresh = TRUE; 
		}		    
		else if (enc < 0)
		{
			if(--posizione == 0)
				posizione = num_vis;

			refresh = TRUE;		
		}
	}
			
	switch (Dati.key)
	{

		case (SKEY_DS_INC):
			if(--posizione == 0)
				posizione = num_vis;

			refresh = TRUE;
		break;

		case (SKEY_DS_DEC):
			if (++posizione > num_vis)
				posizione = 1;

			refresh = TRUE;
		break;
		
		case (DKEY_ENTER):
			if ((menu[menu_table[posizione]].Tipo == T_MONO) || (menu[menu_table[posizione]].Tipo == T_STR) )
				return(menu_table[posizione]);
			else
			{
				// Controllo se il campo è editabile
				if(menu[menu_table[posizione]].Edit == 1)
				{
					if(menu[menu_table[posizione]].Tipo == T_COMBO)
					{
						combo.x = POSX_START;
						combo.y = 34 + X_START;
						combo.val = (unsigned char *)menu[menu_table[posizione]].Dato;
						combo.inizio_voci = menu[menu_table[posizione]].Min;
						combo.num_voci = menu[menu_table[posizione]].Max;
					
						GCombo_Init(&combo, posizione, cursorMode);
					}
					else 
					{          
						editMenu.x=POSX_START;                              
						editMenu.y=34 + X_START;                                      
						editMenu.tipo= menu[menu_table[posizione]].Tipo; 
						if (editMenu.tipo == T_TIME)
							editMenu.len = 7;
						else
							editMenu.len = NumeroCaratteri(menu[menu_table[posizione]].Frm);
						editMenu.min = menu[menu_table[posizione]].Min;
						editMenu.max = menu[menu_table[posizione]].Max; 
						editMenu.Incremento = menu[menu_table[posizione]].Incremento;   
						editMenu.dato = menu[menu_table[posizione]].Dato;               
						editMenu.fmt = menu[menu_table[posizione]].Frm;
						editMenu.fun = 0;

						GEdit_Init(&editMenu, posizione, cursorMode);

						if(Fun != NULL)
						{	Fun();	}
						
					}
					
				}
			}

						
		break;
			
		case (DKEY_ESC):
			return (0);

	}
	
	return (255);
   
}


void GMenu_Draw(void)
{
    unsigned short a, b, c[5], d;
    char str1[6][22];
    float x,y,z;

	if (!refresh)
		return;
	
	refresh = FALSE;
	
	
	b =  posizione;
    c[4] = menu_table[b]; 

    if(++b > num_vis)
		b = 1;
    c[1] = menu_table[b];      

    if(++b > num_vis)
		b = 1;
    c[0] = menu_table[b];  
    b = posizione;      

    if(--b == 0)
		b = num_vis;  
    c[2] = menu_table[b];   

    if(--b == 0)
		b = num_vis;
    c[3] = menu_table[b];
    
    for(a=0;a<4;++a)
		StrCpyMax(&str1[a][0],menu[c[a]].Msg,MAX_CHR);     
   
	StrCpyMax(&str1[4][0],menu[c[4]].Msg,MAX_CHR); 
	
	switch (menu[c[4]].Tipo)
	{
		case (T_STR):	// Stringa
		case (T_PSW):
			StrCpyMax(&str1[5][0],(char *)menu[c[4]].Dato,MAX_CHR);
		break;

		case (T_SHORT):
			sprintf(&str1[5][0],menu[c[a]].Frm,*(short *)menu[c[4]].Dato);
		break;
		
		case (T_USHORT): 
			sprintf(&str1[5][0],menu[c[a]].Frm,*(unsigned short *)menu[c[4]].Dato);
		break;
		
		case (T_DOUBLE):
			sprintf(&str1[5][0],menu[c[a]].Frm,*(double *)menu[c[4]].Dato);
		break;

		case (T_FLOAT):
			sprintf(&str1[5][0],menu[c[a]].Frm,*(float *)menu[c[4]].Dato);
		break;
		
		case (T_UPDW):
			sprintf(&str1[5][0],menu[c[a]].Frm,*(int *)menu[c[4]].Dato); 
		break;
		
		case (T_COMBO):
			d = *(unsigned char*)menu[c[4]].Dato;
			StrCpyMax(&str1[5][0], tabMsg[(int)(menu[c[4]].Min + d)],18);
		break;

		case (T_TIME):
			d = *(int *)menu[c[4]].Dato;
			sprintf(&str1[5][0], menu[c[a]].Frm, (d / 60), (d % 60));
		break;		
	}
	
	gsetmode(GNORMAL);
	gclrvp();
	
	// Titolo videata
	if (strlen(menu[0].Msg))
	{
		gsetmode(INVERSE_MODE);
		gselfont(&footnote); 
		gsetpos(115 - Windows_StrLen(menu[0].Msg), 63);
		gputs(menu[0].Msg);
	}
	
	gsetmode(NORMAL_MODE);
	SetFont9();

	a = strlen(menu[0].Msg);
	a = a/2;
	if(a > ((GDISPW/2)/ggetfw()) ) 
		a=((GDISPW/2)/ggetfw());

	// Titolo Menù
	//gsetcpos(((GDISPW/2)/ggetfw())-a,0);
	//gputs(menu[0].Msg);

	// Disegna la finestra del menu
	//gmoveto(0, 8);
	//glineto(X_SIZE + 7, 8);
	
	gmoveto(2, 18 + X_START);
	glineto(X_SIZE + 1, 18+ X_START);
	gmoveto(0, 20+ X_START);
	glineto(0, 34+ X_START);
	gmoveto(1, 35+ X_START);
	glineto(X_SIZE + 1, 35 + X_START);
	gmoveto(2, 36+ X_START);
	glineto(X_SIZE, 36 + X_START);
	gmoveto(X_SIZE + 2, 19 + X_START);
	glineto(X_SIZE + 2, 34 + X_START);
	gmoveto(X_SIZE + 3, 20 + X_START);
	glineto(X_SIZE + 3, 33 + X_START);
	
	gsetpixel(1, 19 + X_START, 1);
	gsetpixel(X_SIZE + 1, 34 + X_START, 1);
	gsetpixel(X_SIZE + 1, 19 + X_START, 1);
   
 	gmoveto(0, 54);
	glineto(X_SIZE + 7, 54);  
   
	// MenuBar
	b=X_SIZE + 6;
	for(a=X_START; a<=54 + X_START; ++a)
	{
		gsetpixel(b,a,1);
		if(b == X_SIZE + 6)
			b=X_SIZE + 7;
		else
			b=X_SIZE + 6;
	}     

	z = num_vis;   
	x = 52/z;
	z = posizione;
	y = x * z - x;
	b = y + X_START;

   
    for(a=0;a<5;++a)
	{
        gsetpixel(X_SIZE + 5,a+b,1);
        gsetpixel(X_SIZE + 6,a+b,1);
        gsetpixel(X_SIZE + 7,a+b,1);   
    }
    
    SetFont9(); 

    if(menu[c[4]].Tipo == T_MONO){//MonoRiga

        gsetpos(POSX_START, 30 + X_START);
        gputs(&str1[4][0]);
    }
    else{
        gsetpos(POSX_START, 26 + X_START);
        gputs(&str1[4][0]);
        gsetpos(POSX_START, 34 + X_START);
        gputs(&str1[5][0]);
    }
	
    switch(num_vis)
    {
    	default:
    		gsetpos(POSX_START, 53 + X_START);
      		gputs(&str1[0][0]);		// 1 riga
    	case 4:
       		gsetpos(POSX_START, 9 + X_START);
      		gputs(&str1[3][0]);		// 5 riga
    	case 3:
       		gsetpos(POSX_START, 45 + X_START);
      		gputs(&str1[1][0]);		// 2 riga
    	case 2:
    		gsetpos(POSX_START, 17 + X_START);
      		gputs(&str1[2][0]);		// 4 riga
    	case 1:
    	
    	break;
    }   
	
    if(menu[c[2]].Sym != 0)
        gputsym(2,28,(PGSYMBOL) (menu[c[2]].Sym));
}



   
void GMenu_AddMono(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{                       
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_MONO;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm;
	menu[pos].Edit = 1;
	
	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0;                     
}         

void GMenu_AddShort(char *msg,char * frm, short min, short max, unsigned short incr, void * dato,void * sym,char Edit, unsigned short pos,unsigned char pass)
{                                                        
	menu[pos].Dato =dato;
	menu[pos].Max = (short)max;
	menu[pos].Min = (short)min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_SHORT;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm;  
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	
}      

void GMenu_AddUShort(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit, unsigned short pos,unsigned char pass)
{                                                   
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_USHORT;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm;
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	
                                 
}       

void GMenu_AddDouble(char *msg,char * frm,double min, double max, double incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{                                                       
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_DOUBLE;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm;
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	                           
}    

void GMenu_AddFloat(char *msg,char * frm,float min, float max, float incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{                       
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_FLOAT;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm;
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	
}    

void GMenu_AddString(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{    
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_STR;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm;
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	
}     

void GMenu_AddPsw(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{  
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_PSW;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm; 
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	
}     

void GMenu_AddUpDwn(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{   
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_UPDW;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm; 
	menu[pos].Edit = Edit;
	
	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0;                   
}       

void GMenu_AddCombo(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass)
{     	
	menu[pos].Dato =dato;
	menu[pos].Max =max;
	menu[pos].Min =min;
	menu[pos].Incremento = incr;
	menu[pos].Msg =msg;
	menu[pos].Tipo = T_COMBO;
	menu[pos].Sym =sym;
	menu[pos].Frm =frm; 
	menu[pos].Edit = Edit;
	
	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0;             
}         

void GMenu_AddTime(char *msg,char * frm,signed short min, signed short max, signed short incr, void * dato,void * sym,char Edit, unsigned short pos,unsigned char pass)
{                                                   
	menu[pos].Dato = dato;
	menu[pos].Max = max;
	menu[pos].Min = min;
	menu[pos].Incremento = incr;
	menu[pos].Msg = msg;
	menu[pos].Tipo = T_TIME;
	menu[pos].Sym = sym;
	menu[pos].Frm = frm;
	menu[pos].Edit = Edit;

	// Visibile se livello password sufficiente a quello richiesto
	menu[pos].Visibile  = (pass >= LevelPass)? 1 : 0; 	
                                 
}       


void StrCpyMax(char * dest,const char * orig,unsigned short num)
{
	unsigned short a=0;

	while((orig[a] != 0) && (a != (num - 1)))
	{
		dest[a] = orig[a];
		++a;
	}

	dest[a] = 0;

}

void SetFont9(void)
{
#if 0	// Utilizzato per russo, bulgaro...
	gselfont(&Cirillico9);
	return;
#endif
	gselfont(&SMALL_FONT);
}
