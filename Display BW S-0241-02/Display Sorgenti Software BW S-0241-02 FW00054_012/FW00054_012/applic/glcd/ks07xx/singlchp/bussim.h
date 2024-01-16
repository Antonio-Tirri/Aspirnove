#ifndef BUSSIM_H
#define BUSSIM_H
/***************************** bussim.c ************************************

   Definitions for the LCD bus simulator access functions.

   The definitions below can be modified to reflect a given access function
   implementation.

   RAMTEX Engineering Aps 2001

****************************************************************************/

/* Simulated port adresses */
#define GHWWR  0x1
#define GHWRD  0x1
#define GHWSTA 0x0
#define GHWCMD 0x0

/*
#define CS(x)          if(x==0) FIO1CLR  = 0x00000001; else FIO1SET  =  0x00000001
#define RES(x)         if(x==0) FIO1CLR  = 0x00000002; else FIO1SET  =  0x00000002
#define RS(x)          if(x==0) FIO1CLR  = 0x00000010; else FIO1SET  =  0x00000010
#define RW(x)          if(x==0) FIO1CLR  = 0x00000100; else FIO1SET  =  0x00000100
#define EN(x)          if(x==0) FIO1CLR  = 0x00000200; else FIO1SET  =  0x00000200
#define WR(x)          if(x==0) FIO1CLR  = 0x00000100; else FIO1SET  =  0x00000100
#define RD(x)          if(x==0) FIO1CLR  = 0x00000200; else FIO1SET  =  0x00000200
#define LCD_DATA_D(x)  if(x==1) FIO1DIR |= 0x00FF0000; else FIO1DIR &= ~0x00FF0000
#define LCD_DATA       FIO1PIN2
*/
#define NOP_Operation()   __asm__ ("NOP")
#define DELAY_50n     NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation();NOP_Operation()
#define DELAY_200n    DELAY_50n;DELAY_50n;DELAY_50n;DELAY_50n;
#define DELAY_1u      DELAY_200n;DELAY_200n;DELAY_200n;DELAY_200n;DELAY_200n;                  
#define DELAY_2u      DELAY_1u;DELAY_1u;

/*************** Do not modify the definitions below ********************/
#include <sgtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Single chip mode -> access via user supplied access driver functions */
void simwrby( SGUCHAR adr, SGUCHAR dat);
SGUCHAR simrdby( SGUCHAR adr );
void sim_reset( void );

#ifdef __cplusplus
}
#endif

#endif /* BUSSIM_H */
