/**************************** uni_16x16.c ***********************

   uni_16x16 font table and code page structure definitions.
   This file has been auto-generated with the IconEdit / FontEdit tool.

   Copyright(c) RAMTEX 1998-2006

*****************************************************************/
#include <gdisphw.h>

#if (defined(GMULTIBYTE) || defined(GWIDECHAR))
/* This font contains more than 256 symbols so it must be
   used with either GMULTIBYTE or GWIDECHAR defined */

/* Code page entry (one codepage range element) */
static struct
   {
   GCPHEAD chp;
   GCP_RANGE cpr[40];     /* Adjust this index if more codepage segments are added */
   }
GCODE FCODE uni_16x16cp =
   {
   #include "uni_16x16.cp" /* Codepage table */
   };

/* Symbol table entry with fixed sized symbols */
static struct
   {
   GSYMHEAD sh;         /* Symbol header */
   SGUCHAR b[32];       /* Symbol data, "variable length" */
   }
GCODE FCODE uni_16x16sym[519] =
   {
   #include "uni_16x16.sym" /* Include symbols */
   };

/* Font structure */
GCODE GFONT FCODE uni_16x16 =
   {
   10,       /* width */
   16,       /* height */
   sizeof(uni_16x16sym[0])-sizeof(GSYMHEAD), /* number of bytes in a symbol (including any alignment padding)*/
   (PGSYMBOL)uni_16x16sym, /* pointer to array of SYMBOLS */
   519,      /* num symbols */
   (PGCODEPAGE)&uni_16x16cp
   };

#endif
