/**	\file   GMenu.h
 *
 *	\brief  Gestione videata Menù
 *
 * 	\author Walter Giusiano - lab3841 \n
 *  	walter.giusiano@lab3841.com 
 *
 *  \version    v1.0 - 05/05/2015
 * 	-   Versione iniziale
 *
 *
 */
 #ifndef _GMENU_H_
#define _GMENU_H_

#include "dati.h"

enum
{
	T_MONO = 0,
	T_SHORT,
	T_USHORT,
	T_DOUBLE,
	T_FLOAT,
	T_STR,
	T_PSW,
	T_UPDW,
	T_COMBO,
	T_TIME
};



extern struct COMBO_BOX combo;

extern struct EDIT editMenu;

struct GMENU
{ 

   char  			*Msg;          	//numero della readMsg
   unsigned short  	Tipo;         	//Tipo
										// 0: Monoriga
										// 1: Short
										// 2: UShort
										// 3: Double
										// 4: Float
										// 5: Stringa
										// 6: Password
										// 7: UpDwn
										// 8: Combo
   double  			Min;            //Valor Minimo                                
   double  			Max;            //Valor Massimo  
   double  			Incremento;		//Incremento                                
   void 			*Dato;         	//Variabile
   char 			*Frm;          	//Formato
   void 			*Sym;          	//Simbolo 0 - noSimbolo
   char    			Visibile;		// Visibilita'
   char 			Edit;
                        
};




unsigned char GMenu_Init(unsigned char pos, tCursorMode *cursorMode);
	
unsigned char GMenu_Run(void (*Fun)(void), tCursorMode *cursorMode);

void GMenu_Draw(void);


void GMenu_AddMono(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddShort(char *msg,char * frm, short min, short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddUShort(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddDouble(char *msg,char * frm,double min, double max, double incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddFloat(char *msg,char * frm,float min, float max, float incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddString(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddPsw(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddUpDwn(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddCombo(char *msg,char * frm,unsigned short min, unsigned short max, unsigned short incr, void * dato,void * sym,char Edit,unsigned short pos,unsigned char pass);
void GMenu_AddTime(char *msg,char * frm,signed short min, signed short max, signed short incr, void * dato,void * sym,char Edit, unsigned short pos,unsigned char pass);
	
void StrCpyMax(char * dest,const char * orig,unsigned short num);
	
void SetFont9(void);


#endif
