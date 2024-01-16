/***************************** times16.c ************************

  times16 font table structure definitions.
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
 GCODE FCODE times16cp =
   {
   #include "times16.cp" /* Symbol table */
   };

/* Symbol table entry with fixed sized symbols */
static struct
   {
   GSYMHEAD sh;        /* Symbol header */
   SGUCHAR b[16];       /* Symbol data, "variable length" */
   }
GCODE FCODE times16sym[96] =
   {
   #include "times16.sym" /* Include symbols */
   };

/* Font structure */
GCODE GFONT FCODE times16 =
   {
   8,       /* width */
   16,       /* height */
   sizeof(times16sym[0])-sizeof(GSYMHEAD), /* number of bytes in a symbol (including any alignment padding)*/
   (PGSYMBOL)times16sym,/* pointer to array of SYMBOLS */
   96,      /* num symbols */
   (PGCODEPAGE)&times16cp
   };
