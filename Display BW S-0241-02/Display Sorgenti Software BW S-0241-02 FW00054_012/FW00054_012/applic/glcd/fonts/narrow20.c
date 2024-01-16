/**************************** narrow20.c ***********************

   narrow20 font table and code page structure definitions.
   This file has been auto-generated with the IconEdit / FontEdit tool.

   Copyright(c) RAMTEX 1998-2006

*****************************************************************/
#include <gdisphw.h>

/* Code page entry (one codepage range element) */
static struct
   {
   GCPHEAD chp;
   GCP_RANGE cpr[1];     /* Adjust this index if more codepage segments are added */
   }
GCODE FCODE narrow20cp =
   {
   #include "narrow20.cp" /* Codepage table */
   };

/* Symbol table entry with fixed sized symbols */
static struct
   {
   GSYMHEAD sh;         /* Symbol header */
   SGUCHAR b[40];       /* Symbol data, "variable length" */
   }
GCODE FCODE narrow20sym[112] =
   {
   #include "narrow20.sym" /* Include symbols */
   };

/* Font structure */
GCODE GFONT FCODE narrow20 =
   {
   10,       /* width */
   20,       /* height */
   sizeof(narrow20sym[0])-sizeof(GSYMHEAD), /* number of bytes in a symbol (including any alignment padding)*/
   (PGSYMBOL)narrow20sym, /* pointer to array of SYMBOLS */
   112,      /* num symbols */
   (PGCODEPAGE)&narrow20cp
   };

