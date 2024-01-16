/************************** ghwblkrw.c *****************************

   Graphic block copy functions for LCD display

   Read graphic area from the display to a GLCD buffer.
   Write graphic buffer to LCD display.

   Information about the size of the graphic area is stored in the buffer.
   The buffer can be written back to the display with ghw_wrblk(),
   optionally with another start origin.

   All coordinates are absolute pixel coordinate.

   ---------

   The KS07XX controller is assumed to be used with a LCD module.

   The following LCD module characteristics MUST be correctly
   defined in GDISPCFG.H:

      GDISPW  Display width in pixels
      GDISPH  Display height in pixels
      GBUFFER If defined most of the functions operates on
              a memory buffer instead of the LCD hardware.
              The memory buffer content is complied to the LCD
              display with ghw_updatehw().
              (Equal to an implementation of delayed write)


   Creation date:
   Revision date:     030702
   Revision Purpose:  GHW_BLK_HEADER introduced
   Revision date:     041002
   Revision Purpose:  Made more generic by removing use of sg_union16
   Revision date:     070703
   Revision Purpose:  ghw_restoreblk(..) added
   Revision date:     171103
   Revision Purpose:  Use of GHW_BLK_SIZE macro added
   Revision date:     170804
   Revision Purpose:  Modified to use ghw_set_xy(..)
   Revision date:     240206
   Revision Purpose:  SGUCHAR changed to GXT

   Version number: 2.05
   Copyright (c) RAMTEX Engineering Aps 2000-2006

*********************************************************************/
/* <stdlib.h> is included via gdisphw.h */
#include <gdisphw.h>   /* HW driver prototypes and types */
#include <ks07XX.h>   /* KS07XX controller specific definements */

#ifdef GBASIC_INIT_ERR
typedef struct
   {
   GXT lx;
   GYT ly;
   GXT rx;
   GYT ry;
   SGUCHAR dat[1];
   } GHW_BLK_HEADER, * PGHW_BLK_HEADER;

/****************************************************************
 ** block functions
****************************************************************/

/*
   Calculate the needed size for the buffer used by ghw_rdblk()
   Return value can be used as parameter for buffer allocation with
   malloc.
   The coordinates to this function may be absolute or view-port relative
*/
GBUFINT ghw_blksize(GXT ltx, GYT lty, GXT rbx, GYT rby)
   {
   /* Force resonable values (assure that unsigned is positive) */
   GLIMITD(rby,lty);
   GLIMITD(rbx,ltx);

   return GHW_BLK_SIZE(ltx,lty,rbx,rby);
   }

/*
   Copy a graphic area from the display to a GLCD buffer
   Information about the size of the graphic area is saved in the buffer.
   The buffer can be written back to the display with ghw_wrblk(),
   optionally with another start origin.

   All coordinates are absolute pixel coordinate.

   The first part of the buffer will be a dynamic header defining
   the block rectangle:
      GXT left_top_x,
      GYT left_top_y,
      GXT right_bottom_x,
      GYT right_bottom_y,
     followed by th block data
*/
void ghw_rdblk(GXT ltx, GYT lty, GXT rbx, GYT rby, SGUCHAR *dest, GBUFINT bufsize )
   {
   GYT y;
   GXT x;
   PGHW_BLK_HEADER desthdr;
   #ifdef GBUFFER
   GBUFINT gbufidx;
   GBUF_CHECK();
   #endif

   glcd_err = 0;
   if (dest == NULL)
      return;

   /* Force reasonable values */
   GLIMITU(ltx,GDISPW-1);
   GLIMITU(lty,GDISPH-1);
   GLIMITD(rby,lty);
   GLIMITU(rby,GDISPH-1);
   GLIMITD(rbx,ltx);
   GLIMITU(rbx,GDISPW-1);

   if (ghw_blksize(ltx, lty, rbx, rby) > bufsize)
      {
      G_ERROR( "ghw_rdblk: dest buffer too small" );
      return;
      }

   /* Save header info */
   desthdr = (PGHW_BLK_HEADER) dest;
   dest = &(desthdr->dat[0]);
   desthdr->lx = ltx;
   desthdr->ly = lty;
   desthdr->rx = rbx;
   desthdr->ry = rby;

   lty &= ~(0x7);
   rby &= ~(0x7);
   for (y = lty; y <= rby; y+=GDISPCH)
      {
      #ifdef GBUFFER
      gbufidx = GINDEX(ltx, y/8);
      for (x = ltx; x <= rbx; x++)
         *dest++ = gbuf[gbufidx++];

      #else
      ghw_set_xypos(ltx,y);
      ghw_auto_rd_start();     /* init for read with auto increment */
      for (x = ltx; x <= rbx; x++)
         {
         *dest++ = ghw_auto_rd();     /* read with auto increment */
         }
      #endif
      }
   *dest = 0;   /* tail = 0 (needed for simple wrblk shift) */
   }

/*
   Copy a graphic area from a GLCD buffer to the display
   The GLCD buffer must have been read with ghw_rdblk

   If the destination range is larger than the buffered range
   then the destination range is limited to fit the size of
   the buffered range.

   If the destination range is smaller than the buffered range
   then only the upper-left part of the buffer is written to
   the display.

   All coordinates are absolute pixel coordinate.

   The first part of the buffer will be a dynamic header defining
   the block rectangle:
      GXT left_top_x,
      GYT left_top_y,
      GXT right_bottom_x,
      GYT right_bottom_y,
     followed by th block data
*/
void ghw_wrblk(GXT ltx, GYT lty, GXT rbx, GYT rby, SGUCHAR *src )
   {
   GXT w,we,x,xe;
   GYT h,he;
   SGUCHAR msk;
   SGINT shift;
   SGUINT y;
   SGUINT  dat;
   SGUCHAR datb;
   PGHW_BLK_HEADER srchdr;
   #ifdef GBUFFER
   GBUFINT gbufidx;
   GBUF_CHECK();
   #endif

   glcd_err = 0;
   if (src == NULL)
      return;

   /* Force reasonable values */
   GLIMITU(ltx,GDISPW-1);
   GLIMITU(lty,GDISPH-1);
   GLIMITD(rby,lty);
   GLIMITD(rbx,ltx);
   GLIMITU(rby,GDISPH-1);
   GLIMITU(rbx,GDISPW-1);

   #ifdef GBUFFER
   invalrect( ltx, lty );
   invalrect( rbx, rby );
   #endif

   /* Get header info about stored buffer */
   srchdr = (PGHW_BLK_HEADER) src;
   src = &(srchdr->dat[0]);
   w = srchdr->lx;
   h = srchdr->ly;
   we = srchdr->rx;
   he = srchdr->ry;

   /* Set shift value (negative = shift left) */
   shift = (SGINT)(h % GDISPCH) - (SGINT)(lty % GDISPCH);

   /* Limit destination range against source window size in buffer */
   if (rbx-ltx > (we-w))
      rbx = ltx + (we-w);
   if (rby-lty > (he-h))
      rby = lty + (he-h);

   xe = rbx - ltx;
   w = we - w + 1;   /* Stored line width in bytes */

   /* Select stop and start mask */
   msk = startmask[lty & 0x7];
   he  = stopmask[rby & 0x7];
   rby /= GDISPCH;
   lty /= GDISPCH;

   for (y = lty; y <= rby; y++)
      {
      if (y == rby)
         msk &= he;  /* Use stop mask */

      #ifdef GBUFFER
      gbufidx = GINDEX(ltx,y);
      #else
      ghw_set_xypos(ltx,(GYT)(y*GDISPCH));
      #endif

      for (x = 0; x <= xe; x++ )
         {
         if (shift != 0)
            {
            if (shift > 0)
               {
               dat =  (SGUINT) src[x];
               dat |= ((SGUINT) src[(SGUINT)x+w])*256;
               dat >>= shift;
               datb = (SGUCHAR) dat;
               }
            else
               {
               dat = (y == lty) ? 0 : (SGUINT) src[x-w];
               dat |= ((SGUINT) src[x])*256;
               dat <<= abs(shift);
               datb = (SGUCHAR) (dat / 256);
               }
            }
         else
            datb = src[x];

         /* Fetch byte and mask */
         #ifdef GBUFFER
         if (msk != 0xff)
            datb = (gbuf[gbufidx] & ~msk) | (datb & msk);
         gbuf[gbufidx++] = datb;
         #else
         if (msk != 0xff)
            datb = (ghw_rd() & ~msk) | (datb & msk);
         ghw_auto_wr( datb );
         #endif
         }
      src = &src[w]; /* Set to next buffer line */
      msk = 0xff;
      }
   #if (defined(GBUFFER) || defined(GHW_FAST_SIM_UPDATE))
   ghw_updatehw();
   #endif
   }

/*
   Retore a block buffer in the same position as it was read
   The position information is saved in the header
*/
void ghw_restoreblk(SGUCHAR *src)
   {
   PGHW_BLK_HEADER srchdr;
   if ((srchdr = (PGHW_BLK_HEADER) src) != NULL)
      ghw_wrblk(srchdr->lx,srchdr->ly, srchdr->rx, srchdr->ry, src );
   }

#endif

