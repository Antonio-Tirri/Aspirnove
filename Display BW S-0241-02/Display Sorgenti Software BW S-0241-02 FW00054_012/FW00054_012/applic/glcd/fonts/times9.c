/***************************** times9.c ************************

   times9 font table structure definitions.
   This file has been auto-generated with the IconEdit tool.

   Copyright RAMTEX 1998-2003

*****************************************************************/
#include <gdisphw.h>

/* Code page entry (one codepage range element) */
static struct
   {
   GCPHEAD chp;
   GCP_RANGE cpr[1];     /* Adjust this index if more codepage segments are added */
   }
 GCODE FCODE times9cp =
   {
   #include "times9.cp" /* Symbol table */
   };

/* Symbol table entry with fixed sized symbols */
struct
   {
   GSYMHEAD sh;        /* Symbol header */
   SGUCHAR b[9];       /* Symbol data, variable length = (cxpix/8+1)*cypix */
   }
GCODE FCODE times9sym[96] =
   {
   #include "times9.sym" /* Include symbols */
   };

/* Font structure */
GCODE GFONT FCODE times9 =
   {
   5,       /* width */
   9,       /* height */
   sizeof(times9sym[0])-sizeof(GSYMHEAD), /* number of bytes in a symbol (including any alignment padding)*/
   (PGSYMBOL)times9sym,/* pointer to array of SYMBOLS */
   96,      /* num symbols */
   (PGCODEPAGE)&times9cp
   };

