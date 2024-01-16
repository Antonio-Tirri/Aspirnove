#include "display.h"	
#include "board.h"
#include "gdisp.h"
#include "maintimer.h"
#include <bussim.h>
#include <stdio.h>



uint8_t Display_Init(void)
{
	ginit();
	ghw_cont_set(0);
	gclrvp();
	
	return 0;
}


/* Write byte */
void simwrby(uint8_t adr, uint8_t dat)
{
	// Imposta i pin Dato come uscite
	GPIO_SetOutput(PORT_DISPLAY_DATA, 0xff);
	
	if (adr == GHWWR)	RS_On();	// GHWWR --> Graphic LCD chip 0 data read 
	else				RS_Off();	// GHWCMD --> Graphic LCD chip 0 status read   
	
	RW_Off();
	CS_Off();
	EN_On();
	
	PORT_DISPLAY_DATA->ODR  = (PORT_DISPLAY_DATA->ODR & 0xff00) + dat;

	EN_Off();
	CS_On();
}


/* Read byte */
uint8_t simrdby(uint8_t adr)
{
	uint8_t dat;

	// Imposta i pin Dato come ingressi 
	GPIO_SetInput(PORT_DISPLAY_DATA, 0xff, GPIO_PULLDOWN);
	
	if (adr == GHWRD)	RS_On();	// GHWRD --> Graphic LCD chip 0 data read 
	else				RS_Off();	// GHWSTA --> Graphic LCD chip 0 status read   
	
	RW_On();
	CS_Off();
	EN_On();

	dat = (uint8_t)PORT_DISPLAY_DATA->IDR ;
	
	EN_Off();
	CS_On(); 

	return dat;
}
 
 
 
/** Reset LCD display. */
void sim_reset( void )
{
	RESET_Off();
	
	DELAY_2u;
	DELAY_2u;
	DELAY_2u;
	DELAY_2u;
	DELAY_2u;
	DELAY_2u; 
	
	RESET_On();
	DELAY_2u;
}







