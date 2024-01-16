/************************* gsymget.c *******************************

   Creation date: 980223

   Revision date:     03-01-26
   Revision Purpose:  Bit mask on GINVERSE mode

   Revision date:     13-08-04
   Revision Purpose:  Named viewport function _vp added

   Revision date:     7-01-05
   Revision Purpose:  Pointer changed from PGSYMBOL to a GSYMBOL* type
                      (PGSYMBOL is now equal to 'const GSYMBOL *')

   Revision date:
   Revision Purpose:

   Version number: 2.2
   Copyright (c) RAMTEX Engineering Aps 1998-2005

*********************************************************************/
#include <gi_disp.h> /* gLCD prototypes */

#ifdef GSOFT_SYMBOLS

/*******************************************************************
   Segment: Software symbols
   Level: Graphics
   Get box area into symbol buffer.
   The area defined by the input parameters is read from
   the display into sym. The area allocated for symbol must
   be large enough to hold all the data + the GSYMHEAD struct.
   The GSYMHEAD is updated to reflect the actual size
   (which may have been limited by view-port settings or the display size)
   Display information fetched with ggetsym can be written back with
   gfillsym or gputsym.
   See also gsymsize()

   Note: This functions requires a compiler with standard C
   conformant pointer handling. Ex that the same function pointer
   parameter can point both to objects located in ROM and to objects
   located in RAM
*/
void ggetsym(GXT xs, GYT ys, GXT xe, GYT ye, GSYMBOL * psym, GBUFINT size )
   {
   GXT bw;
   GYT h;
   gi_datacheck(); /* check internal data for errors */
   /* normalize to view-port */
   xs = xs + gcurvp->lt.x;
   ys = ys + gcurvp->lt.y;
   xe = xe + gcurvp->lt.x;
   ye = ye + gcurvp->lt.y;

   /* limit values to view-port */
   LIMITTOVP( "ggetsym",xs,ys,xe,ye );
   if( psym == NULL )
      {
      G_WARNING( "ggetsym: parameter, psym == NULL" );
      return;
      }

   glcd_err = 0; /* Reset HW error flag */
   bw = ((xe-xs)+7)/8;
   h = (ye-ys)+1;
   if( size < (GBUFINT)(((GBUFINT)bw * h) + sizeof(GSYMHEAD)/* size of buffer */))
      {
      G_WARNING( "ggetsym: parameter, buffer too small" );
      return;
      }

   /* save w,h */
   psym->sh.cypix = h;
   psym->sh.cxpix = (xe-xs)+1;

   ghw_rdsym(xs, ys, xe, ye, (PGUCHAR)(&(((PGBWSYMBOL)((void*)psym))->b[0])), bw, (SGUCHAR) G_IS_INVERSE() );
   }


/********************************************************************
   Segment: Software symbols
   Level: Graphics
   Returns the minimum size to allocate to hold a symbol.
*/
GBUFINT gsymsize(GXT xs, GYT ys, GXT xe, GYT ye )
   {
   GXT w;
   GYT h;
   GBUFINT bw;

   gi_datacheck(); /* check internal data for errors */
   /* normalize to view-port */
   xs = xs + gcurvp->lt.x;
   ys = ys + gcurvp->lt.y;
   xe = xe + gcurvp->lt.x;
   ye = ye + gcurvp->lt.y;

   /* limit values to view-port */
   LIMITTOVP( "gsymsize",xs,ys,xe,ye );

   w = (xe-xs)+1;
   h = (ye-ys)+1;

   bw = ((GBUFINT)w+7)/8; /* width of char in bytes */

   return ((GBUFINT )bw * h) + sizeof(GSYMHEAD);
   }


#ifdef GFUNC_VP

void ggetsym_vp( SGUCHAR vp, GXT xs, GYT ys, GXT xe, GYT ye, GSYMBOL * psym, GBUFINT size )
   {
   GSETFUNCVP(vp, ggetsym(xs,ys,xe,ye,psym,size) );
   }

GBUFINT  gsymsize_vp( SGUCHAR vp, GXT xs, GYT ys, GXT xe, GYT ye )
   {
   GBUFINT retp;
   GGETFUNCVP(vp, gsymsize(xs,ys,xe,ye) );
   return retp;
   }

#endif /* GFUNC_VP */
#endif /* GSOFT_SYMBOLS */

