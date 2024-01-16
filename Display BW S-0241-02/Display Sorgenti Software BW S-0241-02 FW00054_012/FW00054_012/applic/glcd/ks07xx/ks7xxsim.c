/************************* KS07xxsim.c *********************************

   KS07XX LCD controller simulator for the RAMTEX LCD driver library

   The KS07XX is simulated by the use of one buffer:
     KS07XX_buf    simulates the RAM on the KS07XX module.
     KS07XX_onoff  simulates the display on/off state.

   Simulation of the internal x,y pointer handling in KS07XX is
   omitted in this simulator as this information is tracked anyway
   by the g_xpos, g_ypos parameters in ghw_init()

   Revision date:     070306
   Revision Purpose:  Simulator extended to support 4 chips

   Revision date:    041207
   Revision Purpose: TURBOC simulator mode discontinued.

   Revision date:    100108
   Revision Purpose: Simulator buffer allocated by malloc

   Version number: 1.02
   Copyright (c) RAMTEX Engineering Aps 2003-2008

*********************************************************************/

#ifndef GHW_PCSIM
#error Only include this file in Gdisp simulation mode
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <gdisphw.h>  /* swprintf */ /* GHW_FAST_SIM_UPDATE */
#include <ks07xx.h>   /* KS07XX controller specific definements */

static int graph_init = 0;           /* don't init graphic twice */

#ifdef _WIN32
   #include <gsimintf.h>
   void simputs( SGINT sgstream, const char *chp )
      {
      sgstream = sgstream; /* remove warning */
      GSimPuts(chp);
      }
   #define far /* nothing */
#else
   #error no simulation for this compiler
#endif

void simprintf( SGINT sgstream, const char far *fmt, ...)
   {
   va_list argptr;
   int form_len;
   static char printf_buf[100];

   va_start(argptr, fmt);
   form_len = vsprintf(printf_buf, fmt, argptr);
   va_end(argptr);

   if (form_len >= 100)
      simputs(-1,"\nERROR: Buffer overrun in simsprintf()");

   simputs( sgstream, printf_buf );
   }

/* Internal controller data simulation */
SGUCHAR *KS07XX_buf = NULL;     /* KS07XX simulator module buffer (max for 4 chips) */
static SGBOOL  KS07XX_onoff;    /* Display on off */

/* Copy of user LCD size variables (allows the simulator to be a lib) */
static SGUINT   gdisph;               /* Initiated with user gdisph */
static SGUINT   gdispw;               /* Initiated with user gdispw */
static SGUINT   gctrlw;               /* Initiated with user gctrlw */

/*
  Write to PC video here
*/
#ifdef _WIN32
   #define ghw_wr_PCSim( x, y, val ) \
         { \
         SGUINT x1,y1; \
         SGUCHAR msk; \
         msk = 0x01; \
         y1 = ((y) & ~(0x7)); \
         x1 = (x); \
         do \
            { \
            GSimWrBit(x1,y1++,(unsigned char)(((val) & msk) != 0));  \
            } \
         while ((msk = (msk << 1) & 0xff) != 0); \
         }
#else

static void ghw_wr_PCSim( SGUINT x, SGUINT y,  SGUCHAR val )
      {
      register SGUCHAR msk;

      /*assure byte alignement */
      y = (y & ~(0x7));

      msk = 0x01;
      #ifdef _WIN32
      do
         {
         GSimWrBit(x,y++,((val&msk)!=0 ));
         }
      while ((msk = (msk << 1) & 0xff) != 0);
      #else
      #error no simulation for this compiler
      #endif
      }
#endif


/*
   Simulate KS07XX commands
   Only ON / OFF is relevant
*/
void ghw_cmd_KS07XXsim( SGUCHAR cmd )
   {
   register SGUINT xp,yp;

   switch (cmd)
      {
      case GCTRL_OFF:  /* Display off */
         if (KS07XX_onoff != 0)
            {
            /* Blank PC video display (leave buffers intact) */
            for( yp=0; yp < gdisph; yp+=8 )
               {
               for( xp=0; xp < gctrlw; xp++ )
                  ghw_wr_PCSim( xp, yp, 0 );
               }
            KS07XX_onoff = 0;
            #ifdef _WIN32
            GSimFlush();
            #endif
            }
         break;
      case GCTRL_ON:
         if (KS07XX_onoff == 0)
            {
            /* Restore whole buffer content on screen */
            SGUCHAR *p = &(KS07XX_buf[0]);
            for( yp=0; yp < gdisph; yp += 8)
               {
               for( xp=0; xp < gctrlw; xp++, p++ )
                  ghw_wr_PCSim( xp, yp, *p );
               }
            KS07XX_onoff = 1;
            #ifdef _WIN32
            GSimFlush();
            #endif
            }
         break;
      default:
         {
         }
      }
   }

/*
   Double command debug service
*/
void ghw_dcmd_KS07XXsim( SGUCHAR cmd1, SGUCHAR cmd2 )
   {
   switch(cmd1)
      {
      #if (!defined( GHW_NJU6677 ) && !defined(GHW_S1D15719))
      case GCTRL_REF_VOLT:
         {
         char str[30];
         sprintf(str,"Set contrast: 0x%02X",cmd2);
         simputs(0,str);
         break;
         }
      #endif
      default:
         {
         }
      }
   }

/*
   Write to Update PC screen
*/
void ghw_wrupdate_KS07XXsim( SGUINT x, SGUINT y, SGUCHAR val )
   {
   if (graph_init == 0)
      return;   /* Display have not been initiated yet */

   KS07XX_buf[x + (y/8)*gctrlw] = val; /* Update buffer */
   if (KS07XX_onoff != 0)
      ghw_wr_PCSim( x, y, val );      /* Update display if not turned off */
   }

/*
   Init PC screen here according to internal simulator parameters
*/
static void ghw_init_PCSim(void)
   {
   #ifdef _WIN32
/*   if( !graph_init ) */
      {
      if (!GSimInit((unsigned short) gdispw, (unsigned short) gdisph)) /* Init connection to LCD simulator */
         {
         graph_init = 1;
         }
      GSimFlush();
      }
   #else
   #error no simulation for this compiler
   #endif
   }

/*
   This function is called last in ghw_init after all KS07XX display
   module parameters has been initiated.

   NOTE: User code may cause that this function is in-worked more
   than once
*/
void ghw_init_KS07XXsim( SGUINT dispw, SGUINT disph, SGUINT ctrlw)
   {
   gdisph =  disph;
   gdispw =  dispw;
   gctrlw =  ctrlw;
   if (KS07XX_buf != NULL)
      free(KS07XX_buf);
   graph_init = 0;
   KS07XX_onoff = 0;          /* Turn display off*/
   if ((KS07XX_buf = (SGUCHAR *) calloc(((disph+7)/8)*ctrlw,1))==NULL)
      return; /* Memory allocation error */
   /* Initialize PC screen here according to internal simulator parameters */
   ghw_init_PCSim();
   }

/*
   This function is activated via ghw_exit() when gexit() is called
*/
void ghw_exit_KS07XXsim( void )
   {
   #if defined( _WIN32 )
   GSimClose();
   #endif
   graph_init = 0;
   if (KS07XX_buf != NULL)
      free(KS07XX_buf);
   KS07XX_buf = NULL;
   }

