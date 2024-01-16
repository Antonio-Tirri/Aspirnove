

typedef struct _Setting_Menu_Assegna_Funzioni
{
	union
	{
		struct {
					unsigned int 	L1_En:                            1;	    /**Abilitazione voce di  1° livello */
					unsigned int 	L2_En_V0:                         1;		/**Abilitazione voce di  2° livello N° 0*/	                
		            unsigned int 	L2_En_V1:                         1;		/**Abilitazione voce di  2° livello N° 1*/	
		            unsigned int 	L2_En_V2:                         1;		/**Abilitazione voce di  2° livello N° 2*/	
		            unsigned int 	L2_En_V3:                         1;		/**Abilitazione voce di  2° livello N° 3*/	
		            unsigned int 	L2_En_V4:                         1;		/**Abilitazione voce di  2° livello N° 4*/	
		            unsigned int 	L2_En_V5:                         1;		/**Abilitazione voce di  2° livello N° 5*/	
		            unsigned int 	L2_En_V6:                         1;		/**Abilitazione voce di  2° livello N° 6*/	
		            unsigned int 	L2_En_V7:                         1;		/**Abilitazione voce di  2° livello N° 7*/	
		            unsigned int 	L2_En_V8:                         1;		/**Abilitazione voce di  2° livello N° 8*/	
					unsigned int 	L2_En_V9:                         1;		/**Abilitazione voce di  2° livello N° 9*/	
		            unsigned int 	L2_En_V10:                        1;		/**Abilitazione voce di  2° livello N° 10*/	
		            unsigned int 	L2_En_V11:                        1;		/**Abilitazione voce di  2° livello N° 11*/	
		            unsigned int 	L2_En_V12:                        1;		/**Abilitazione voce di  2° livello N° 12*/	
		            unsigned int 	L2_En_V13:                        1;		/**Abilitazione voce di  2° livello N° 13*/	
		            unsigned int 	L2_En_V14:                        1;		/**Abilitazione voce di  2° livello N° 14*/	
					
				   unsigned int 	L2_En_V15:                        1;		/**Abilitazione voce di  2° livello N° 15*/	
				   unsigned int 	L2_En_Riserva_0:                 15;		/**Abilitazione voce di  2° livello RISERVA*/	

					
					
					};
		struct {
				unsigned int Enable_W0;
	            unsigned int Enable_W1;
			   };
	};	
}T_Setting_Menu_Assegna_Funzioni;

typedef struct _Setting_Menu_Alleggerimento
{
	union
	{
		struct {
					unsigned int 	L1_En:                            1;	    /**Abilitazione voce di  1° livello */
					unsigned int 	L2_En_V0:                         1;		/**Abilitazione voce di  2° livello N° 0*/	                
		            unsigned int 	L2_En_V1:                         1;		/**Abilitazione voce di  2° livello N° 1*/	
		            unsigned int 	L2_En_V2:                         1;		/**Abilitazione voce di  2° livello N° 2*/	
		            unsigned int 	L2_En_V3:                         1;		/**Abilitazione voce di  2° livello N° 3*/	
		            unsigned int 	L2_En_V4:                         1;		/**Abilitazione voce di  2° livello N° 4*/	
		            unsigned int 	L2_En_V5:                         1;		/**Abilitazione voce di  2° livello N° 5*/	
		            unsigned int 	L2_En_V6:                         1;		/**Abilitazione voce di  2° livello N° 6*/	
		            unsigned int 	L2_En_V7:                         1;		/**Abilitazione voce di  2° livello N° 7*/	
		            unsigned int 	L2_En_V8:                         1;		/**Abilitazione voce di  2° livello N° 8*/	
					unsigned int 	L2_En_V9:                         1;		/**Abilitazione voce di  2° livello N° 9*/	
		            unsigned int 	L2_En_V10:                        1;		/**Abilitazione voce di  2° livello N° 10*/	
		            unsigned int 	L2_En_V11:                        1;		/**Abilitazione voce di  2° livello N° 11*/	
		            unsigned int 	L2_En_V12:                        1;		/**Abilitazione voce di  2° livello N° 12*/	
		            unsigned int 	L2_En_V13:                        1;		/**Abilitazione voce di  2° livello N° 13*/	
		            unsigned int 	L2_En_V14:                        1;		/**Abilitazione voce di  2° livello N° 14*/	
					
				   unsigned int 	L2_En_V15:                        1;		/**Abilitazione voce di  2° livello N° 15*/	
				   unsigned int 	L2_En_Riserva_0:                 15;		/**Abilitazione voce di  2° livello RISERVA*/	

					
					
					};
		struct {
				unsigned int Enable_W0;
	            unsigned int Enable_W1;
			   };
	};	
}T_Setting_Menu_Alleggerimento;


typedef struct _Menu_Setting
{
	union
	{
		struct {
					unsigned int 	L1_En_Menu:                         1;			/**Abilitazione di menu livello 1 */
	                unsigned int 	L1_En_Lingua:                       1;			/**Abilitazione di menu livello 1 */
					unsigned int 	L1_En_Tipo_Macchina:                1;			/**Abilitazione di menu livello 1 */
					unsigned int    L1_En_Riserva_0:                    13;          /**Abilitazioni riserva 0 */
					T_Setting_Menu_Assegna_Funzioni      Assegna_Funzioni;          /**Abilitazioni menu e sottomenu assegna funzioni  */
					unsigned int 	L1_En_Carica_Default:               1;			/**Abilitazione di menu livello 1 */
				    unsigned int 	L1_En_Feedback_Tasti:               1;			/**Abilitazione di menu livello 1 */	
	                T_Setting_Menu_Alleggerimento	       Alleggerimento;			/**Abilitazione di menu livello 1 */
					unsigned int 	L1_En_Discesa_Pressione:            1;			/**Abilitazione di menu livello 1 */
					unsigned int 	L1_En_Abilitazioni:                 1;			/**Abilitazione di menu livello 1 */
					unsigned int 	L1_En_Totalizzatori:                1;			/**Abilitazione di menu livello 1 */
					unsigned int 	L1_En_Debug:                        1;		    /**Abilitazione di menu livello 1 */
	                unsigned int 	L1_En_Riserva_1:                    12;			/**Abilitazione  riserva 1*/

		
			   };
		struct {
				unsigned int L1_Enable_W0;
	
			   };
	};	
}T_Menu_Setting;

/*Struttura usata dai diversi tipi macchina per creare il menu relativo ad esse*/
extern T_Menu_Setting  Menu_Setting;

void Set_PSW(char livello);
/*Funzione che crea le voci di primo livello del menu*/ 
 void Menu_Principale(void);
 void Menu_EEPROM(void);

