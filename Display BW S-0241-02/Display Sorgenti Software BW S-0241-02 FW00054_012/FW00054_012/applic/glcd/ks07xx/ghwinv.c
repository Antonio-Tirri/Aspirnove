/************************** ghwinv.c *****************************

   Invert box area

   The box area may have any pixel boundary.

   ---------

   The KS07XX controller is assumed to be used with a LCD module.

   The following LCD module characteristics MUST be correctly
   defined in GDISPCFG.H:

      GDISPW  Display width in pixels
      GDISPH  Display height in pixels
      GBUFFER If defined most of the functions operates on
              a memory buffer instead of the LCD hardware.
              The memory buffer content is copied to the LCD
              display with ghw_updatehw().
              (Equal to an implementation of delayed write)


   Creation date:
   Revision date:     170804
   Revision Purpose:  Modified to use ghw_set_xy(..)
   Revision date:
   Revision Purpose:

   Version number: 1.0
   Copyright (c) RAMTEX Engineering Aps 2002

*********************************************************************/
#include <gdisphw.h>   /* HW driver prototypes and types */
#include <ks07xx.h>    /* KS07XX controller specific definements */

#if (!defined( GNOCURSOR ) && defined (GSOFT_FONTS )) || defined (GGRAPHICS)

void ghw_invert(GXT ltx, GYT lty, GXT rbx, GYT rby)
   {
   GYT y;
   GXT GFAST x;
   SGUCHAR GFAST msk, mske;
   #ifdef GBUFFER
   GBUFINT gbufidx;
   GBUF_CHECK();
   #endif

   glcd_err = 0;

   /* Force reasonable values */
   GLIMITU(ltx,GDISPW-1);
   GLIMITU(lty,GDISPH-1);
   GLIMITD(rby,lty);
   GLIMITU(rby,GDISPH-1);
   GLIMITD(rbx,ltx);
   GLIMITU(rbx,GDISPW-1);

   #ifdef GBUFFER
   invalrect( ltx, lty );
   invalrect( rbx, rby );
   #endif

   /* Prepare masks */
   msk = startmask[lty & 0x7];
   mske  = stopmask[rby & 0x7];
   rby /= GDISPCH;
   lty /= GDISPCH;

   /* Loop byte rows */
   for (y = lty; y <= rby; y++)
      {
      if (y == rby)
         msk &= mske;  /* Use stop mask on last row */

      #ifdef GBUFFER
      gbufidx = GINDEX(ltx,y);
      #else
      ghw_set_xypos(ltx,(GYT)(y*GDISPCH));
      #endif
      /* Loop bit collums */
      for (x = ltx; x <= rbx; x++ )
         {
         #ifdef GBUFFER
         gbuf[gbufidx++] ^= (0xff & msk) ;
         #else
         ghw_auto_wr((SGUCHAR)(ghw_rd() ^ (0xff & msk)));
         #endif
         }
      msk = 0xff;
      }
   #if (defined(GHW_PCSIM) && !defined(GHW_FAST_SIM_UPDATE))
   GSimFlush();
   #endif
   }

#endif /* GBASIC_TEXT */

