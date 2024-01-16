/************************* ggrect.c ********************************

   Creation date: 980224

   Revision date:     03-01-26
   Revision Purpose:  Bit mask on GINVERSE mode

   Revision date:     13-08-04
   Revision Purpose:  Named viewport function _vp added

   Revision date:
   Revision Purpose:

   Version number: 2.3
   Copyright (c) RAMTEX Engineering Aps 1998-2004

*********************************************************************/
#include <gi_disp.h> /* gLCD prototypes */

#ifdef GGRAPHICS

/********************************************************************
   Segment: Graphics
   Level: Graphics
   Draw a rectangle, not filling inner.
   grectangle(0,0,2,2) is a border with 1 pixel inside.
*/
void grectangle( GXT xs, GYT ys, GXT xe, GYT ye )
   {
   /* normalize to view-port */
   xs = xs + gcurvp->lt.x;
   ys = ys + gcurvp->lt.y;
   xe = xe + gcurvp->lt.x;
   ye = ye + gcurvp->lt.y;

   /* limit values to view-port */
   LIMITTOVP( "grectangle",xs,ys,xe,ye );

   glcd_err = 0; /* Reset HW error flag */
   ghw_rectangle( xs,ys,xe,ye, (SGBOOL) (G_IS_INVERSE() ? 0 : 1) );
   ghw_updatehw();
   }

#ifdef GFUNC_VP

void grectangle_vp( SGUCHAR vp, GXT xs, GYT ys, GXT xe, GYT ye )
   {
   GSETFUNCVP(vp, grectangle(xs,ys,xe,ye) );
   }

#endif /* GFUNC_VP */

#endif /* GGRAPHICS */

