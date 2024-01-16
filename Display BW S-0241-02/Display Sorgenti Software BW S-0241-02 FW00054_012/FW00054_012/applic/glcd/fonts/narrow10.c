/***************************** suFont.c ***********************

  suFont font table structure definitions.
  This file has been auto-generated with the IconEdit tool.

   Copyright RAMTEX 1998-2007

*****************************************************************/
#include <gdisphw.h>

/* Code page entry (one codepage range element) */
static struct
   {
   GCPHEAD chp;
   GCP_RANGE cpr[1];     /* Adjust this index if more codepage segments are added */
   }
 GCODE FCODE narrow10cp =
   {
   #include "narrow10.cp" /* Symbol table */
   };

/* Symbol table entry with fixed sized symbols */
static struct
   {
   GSYMHEAD sh;        /* Symbol header */
   SGUCHAR b[10];       /* Symbol data, "variable length" */
   }
GCODE FCODE narrow10sym[112] =
   {
   #include "narrow10.sym" /* Include symbols */
   };

/* Font structure */
GCODE GFONT FCODE narrow10 =
   {
   5,       /* width */
   10,       /* height */
   sizeof(narrow10sym[0])-sizeof(GSYMHEAD), /* number of bytes in a symbol (including any alignment padding)*/
   (PGSYMBOL)narrow10sym,/* pointer to array of SYMBOLS */
   112,      /* num symbols */
   (PGCODEPAGE)&narrow10cp
   };
