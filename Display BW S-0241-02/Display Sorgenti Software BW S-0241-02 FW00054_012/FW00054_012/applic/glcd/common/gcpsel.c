/************************* gcpsel.c ******************************

   Creation date: 11-06-03

   Revision date:     13-08-04
   Revision Purpose:  Named viewport function _vp added

   Revision date:     16-11-07
   Revision Purpose:  Vfont adaption

   Version number: 1.2
   Copyright (c) RAMTEX Engineering Aps 2004-2007

******************************************************************/

#include <gi_disp.h> /* glcd prototypes */

#ifdef GSOFT_FONTS
/*******************************************************
   Segment: SoftFonts
   Level: Fonts
   Set current codepage, passing codepage pointer
   Returns previous codepage
********************************************************/
PGCODEPAGE gfgetcp( PGFONT fp )
   {
   if (fp == NULL)
      return (PGCODEPAGE)(NULL);
   return gi_fpcodepage(fp);
   }

PGCODEPAGE gselcp( PGCODEPAGE pcp )
   {
   PGCODEPAGE cp;

   glcd_err = 0;   /* Reset HW error flag */
   gi_datacheck(); /* check internal data for errors */
   cp = gcurvp->codepagep;
   if (pcp == NULL)
      {
      if (gcurvp->pfont != NULL)
         /* Use code page for font */
         gcurvp->codepagep = gfgetcp(gcurvp->pfont);
      else
         /* its legal to select a NULL codepage */
         gcurvp->codepagep = pcp;
      }
   else
      gcurvp->codepagep = pcp;

   gi_calcdatacheck(); /* correct VP to new settings */
   return cp;
   }

PGCODEPAGE ggetcp( void )
   {
   gi_datacheck(); /* check internal data for errors */
   return gcurvp->codepagep;
   }

#ifdef GFUNC_VP

PGCODEPAGE gselcp_vp( SGUCHAR vp, PGCODEPAGE pcp )
   {
   PGCODEPAGE retp;
   GGETFUNCVP(vp, gselcp( pcp ) );
   return retp;
   }

PGCODEPAGE ggetcp_vp( SGUCHAR vp )
   {
   PGCODEPAGE retp;
   GGETFUNCVP(vp, ggetcp() );
   return retp;
   }

#endif /* GFUNC_VP */

#endif /* GSOFT_FONTS */

