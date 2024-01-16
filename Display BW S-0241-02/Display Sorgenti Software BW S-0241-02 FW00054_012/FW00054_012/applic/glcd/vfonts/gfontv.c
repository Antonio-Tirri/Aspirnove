/*
   VIRTUAL FONT DRIVER INTERFACE
   -----------------------------
   Internal library low-level drivers for codepage and symbols stored
   in virtual (external) memory.

   External virtual memory is only accessed via getvmem(..) function
   which must be implemented by the library user. The prototype for
   getvmem(..) are located in the getvmem.h header file.

   Local buffering and load position flags are used in this implementation
   to minimize the number of fetches from external memory.

   All knowledge about details for the font storage layout is isolated in this module.

   NOTE:
   The data storage format used by this module must correspond with the genvf.exe
   generated formats, so do NOT make any code changes unless you are absolutely shure
   what you are doing.

   Revision date:     17-04-2009
   Revision Purpose:  Virtual font structure split in a RAM dependent and a constant
                      data structure
                      (to handle compilers using non-standard C conformant pointers)

   Version number: 1.1
   RAMTEX International ApS 2007-2009
*/
#include <gvfont.h>

#ifdef GVIRTUAL_FONTS

/*
   Define size of temporary buffer (for optimum performance make it large enough
   to hold the symbol data size of a symbol in the most frequently used fonts)
   Minimum allowed value for VFBUFSIZE is 6 bytes (= GCP_RANGE_VSIZE).
   Note: Must be an even number to assure word alignement for binary font copy
*/
#define  VFBUFSIZE 32

static SGUCHAR vfbuf[VFBUFSIZE];

/***************** Symbol load controls ************/
static GFONTBUFIDX vfbufstart;
static GFONTBUFIDX vfbufend;
static GFONTBUFIDX vfsymstart;
static GBUFINT vfsymsize;
static GFONTDEVICE vfdevice;

/*************** Font codepage and symbol lookup *****************/

static PGFONTV vfontp = NULL;
static GFONTBUFIDX vfsym_base;
/* Vsymbol header, variable (RAM) part */
static GSYMHEADV_V vsymbol_v;
/* Vsymbol header, constant (ROM) part */
static GCODE GSYMHEADV FCODE vsymbol =
   {
   0,0,1,               /* Virtual font type identifier */
   (PGSYMHEADV_V) &vsymbol_v /* Pointer to variable part*/
   };

static GCP_RANGE vcp_range;
static GFONTBUFIDX vfcodepage_last = (((GFONTBUFIDX)(-1)) & ~0x1);
static GFONTBUFIDX vfcodepage_base = 0;

/* Data type sizes in virtual memory */
#define  GWCHAR_VSIZE    2
#define  GXT_VSIZE       2
#define  GCP_RANGE_VSIZE (GWCHAR_VSIZE*3)

/*
   Prepare for fast codepage and symbol data lookup (if needed)
   Init and preload a GSYMHEADV structure for the font related parameters
   (Default to first symbol in font)
*/
void gi_fontv_open(PGFONTV fp)
   {
   if (vfontp != fp)
      { /* New font, prepare for load */
      vfontp = fp;
      if (fp->pcodepage != NULL)
         vfcodepage_base = fp->pcodepage->si_codepage;
      else
         vfcodepage_base = 0;

      /* Init local vsymbol structure */
      vfsym_base = fp->si_symbols;

      vsymbol_v.numbits = fp->numbits;
      vsymbol_v.cxpix = fp->symwidth;
      vsymbol_v.cypix = fp->symheight;
      vsymbol_v.device_id = fp->device_id;
      vsymbol_v.bidx = vfsym_base;

      /* Prepare for codepage and symbol reinit */
      vfcodepage_last = (((GFONTBUFIDX)(-1)) & ~0x1);
      }
   }

/*
   Return pointer to codepage range for virtual font
   gi_fontv_open(..) must have been called in advance
*/
PGCP_RANGE_V gi_fontv_cp( GWCHAR index )
   {
   GFONTBUFIDX vfcodepage;
   vfcodepage = vfcodepage_base + ((GFONTBUFIDX)index)*GCP_RANGE_VSIZE;
   if (vfcodepage != vfcodepage_last)
      {
      /* New range, load it */
      getvmem(vsymbol_v.device_id, vfbuf, vfcodepage, GCP_RANGE_VSIZE);
      /* Solve any endian differences between platforms */
      vcp_range.min = ((GWCHAR)vfbuf[1]) * 256 + vfbuf[0];
      vcp_range.max = ((GWCHAR)vfbuf[3]) * 256 + vfbuf[2];
      vcp_range.idx = ((GWCHAR)vfbuf[5]) * 256 + vfbuf[4];
      vfcodepage_last = vfcodepage;
      }
   return (PGCP_RANGE_V)(&vcp_range);
   }

/*
   Return pointer to virtual font symbol header for symbol equal to index
   gi_fontv_open(..) must have been called in advance
*/
PGSYMBOL gi_fontv_sym( GWCHAR index )
   {
   GFONTBUFIDX vfsym;
   vfsym = vfsym_base + ((GFONTBUFIDX)index)*((GFONTBUFIDX)vfontp->symsize+GXT_VSIZE);
   if ( (vfsym+GXT_VSIZE) != vsymbol_v.bidx )
      {
      /* Width not loaded, Get active symbol width (assuming a proportional font) */
      getvmem(vsymbol_v.device_id, vfbuf, vfsym, GXT_VSIZE);
      /* Solve any endian differences between platforms */
      vsymbol_v.cxpix = (GXT)(((SGUINT)vfbuf[1]) * 256 + vfbuf[0]);
      vsymbol_v.bidx = vfsym+GXT_VSIZE;
      }
   return (PGSYMBOL) (&vsymbol);
   }

/********************* Symbol fetch ********************/

/*
   Symbol load preset function
   Preset symbol load information and do first symbol load.
   (called before low-level drivers are activated)
*/
void gi_symv_open( PGSYMBOL psymbol, SGUINT bw, GYT offset)
   {
   PGSYMHEADV_V psymv;
   psymv = ((PGSYMHEADV) psymbol)->psymh_v; /* Pointer to variable part */
   vfsymstart = psymv->bidx+(GFONTBUFIDX)bw*offset;
   vfsymsize  = (GBUFINT)(psymv->cypix - offset)*bw;
   vfbufstart = vfsymstart;
   vfdevice = psymv->device_id;

   /* Optimize read size */
   if (vfsymsize < VFBUFSIZE)
      vfbufend = vfbufstart+vfsymsize-1; /* Read whole symbol in one operation */
   else
      vfbufend = vfbufstart+VFBUFSIZE-1; /* Read first part of symbol */

   /* Prefetch first part of symbol */
   getvmem(vfdevice, vfbuf, vfbufstart, (SGUINT) ((SGULONG)vfbufend-vfbufstart+1));
   }

/*
   Symbol data fetch
   gi_symv_open(..) must have been called first
   This function is called from low-level drivers.
*/
SGUCHAR gi_symv_by(GBUFINT symdatidx)
   {
   GFONTBUFIDX sidx = vfsymstart+symdatidx;
   if ((sidx < vfbufstart) || (sidx > vfbufend))
      {
      /* Symbol data index is outside temp buffer,
         fetch new data from external memory */
      vfbufstart = sidx;
      vfbufend = vfbufstart + (VFBUFSIZE-1);
      /* Optimize read size */
      if (vfbufend >= vfsymstart+vfsymsize)
         vfbufend = vfsymstart+vfsymsize-1;
      /* Fetch new part of symbol */
      getvmem(vfdevice, vfbuf, vfbufstart, (SGUINT) ((SGULONG)vfbufend-vfbufstart+1));
      }
   return vfbuf[sidx - vfbufstart];
   }

#if (defined( GHW_ACCELERATOR ) && defined( GHW_SYMCOPY_ACCELERATOR))
/*
   Return 1 if symbol is loaded in memory directly accessible by
   display controller (accelerator accessible memory) and if so update the
   offset with logical offset of symbol
*/
SGUCHAR gi_symv_isloaded( SGULONG *offset )
   {
   if (getvmem_is_loaded( vfdevice ))
      {
      if (offset != NULL)
         *offset = vfdevice + vfsymstart;
      return 1;
      }
   return 0;
   }
#endif

#endif /* GVIRTUAL_FONTS */

