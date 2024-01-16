/*************************** times13.c ************************

   Times font table structure and codepage structure definitions

   Copyright RAMTEX 1998-2003

*****************************************************************/
#include <gdisphw.h>

/* Code page entry (one codepage range element) */
static struct
   {
   GCPHEAD chp;
   GCP_RANGE cpr[1];     /* Adjust this index if more codepage segments are added */
   }
 GCODE FCODE times13cp =
   {
   #include "times13.cp" /* Symbol table */
   };

/* Symbol table entry with fixed sized symbols */
static struct
   {
   GSYMHEAD sh;         /* Symbol header */
   SGUCHAR b[13];       /* Symbol data, variable length = (cxpix/8+1)*cypix */
   }
GCODE FCODE times13sym[96] =  /* Adjust his index if more symbols are added */
   {
   #include "times13.sym" /* Symbol table */
   };

/* Font structure */
GCODE GFONT FCODE times13 =
   {
   8,       /* width  */
   13,      /* height */
   sizeof(times13sym[0])-sizeof(GSYMHEAD), /* number of bytes in a symbol (including any alignment padding)*/
   (PGSYMBOL)times13sym, /* pointer to array of SYMBOLS */
   96,    /* number of symbols in times13sym */
   (PGCODEPAGE)&times13cp
   };

