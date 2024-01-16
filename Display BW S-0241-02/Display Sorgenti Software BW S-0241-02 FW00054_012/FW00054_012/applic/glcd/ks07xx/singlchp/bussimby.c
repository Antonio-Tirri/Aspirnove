/***************************** bussimby.c ************************************

   Generic template file for external bus simulator drivers.

   The functions in this module is called by single-chip-processor version
   of the ghwinit.c module.

   This implementation assumes that the processor port architecture only
   supports BYTE wide port access. Unused bits in the bus CTRL port is left
   unchanged.

   The compiler independent SG syntax notation is used in this example for
   clarification.
   (Alternatively these functions could be implemented using the compiler
    specific syntax for I/O port operations.)

   The following symbolic names for bit and byte ports must be declared for
   the actual processor architecture and target system:

   Byte ports:
      DATAWR (LCD data write)
      DATARD (LCD data read. May be equal to DATAWR, depends on the processor)
      CTRLWR (LCD ctrl port write)
      CTRLRD (LCD ctrl port read, May be equal to CTRLWR, depends on the processor)

      DATADIR (Processor data port direction setup, depends on the processor).
      CTRLDIR (Processor ctrl port direction setup, depends on the processor).

   Copyright (c) RAMTEX Engineering Aps 1998-2001

****************************************************************************/
#ifndef GHW_NOHDW
#ifdef  GHW_SINGLE_CHIP

#include <bussim.h>
#include <gdispcfg.h> /* Include GDISPCW declaration */

#include <sgio.h>   /* the above port names must be defined in sgio_ta.h
                       using the appropriate compiler syntax. Use the SGSETUP
                       tool when ever possible */

/*#include < Port declaration file for your compiler >
/* Is included instead of sgio.h if the compiler specific syntax for port
   access is used */

/*
   Update these defined values according to processor port architecture
   (if port direction control is needed)
*/
#define  DATABUS_IS_INP  0x00  /* 8 signal lines is used as input */
#define  DATABUS_IS_OUTP 0xFF  /* 8 signal lines is used as outputs */
#define  CTRLBUS_IS_OUTP 0x3F  /* 4-6 signal lines is used as output */

/* Define bits for control port (modify these definements to fit target) */
#define CD     0x01   /* LCD address line) */
#define NWR    0x02   /* LCD /WR line) */
#define NRD    0x04   /* LCD /RD line) */
#define NCE    0x08   /* LCD /CE line) */
#define NRST   0x10   /* LCD /RST line, if connected to a port) */
#define CTRLMSK  (CD|NWR|NRD|NCE)  /* Mask of port bits used dynamically */


/*
   Simulate a bus write operation for a LCD controller with an Intel
   like bus interface (i.e. use of separate /RD and /WR strobes).

   The address parameter adr is assumed to be either 0 or 1.
*/
void simwrby(SGUCHAR adr, SGUCHAR dat)
   {
   /* A: Set C/D line according to adr, Set /CE line active low */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) | CTRLMSK) & ((adr == 0) ? ~(NCE|CD) : ~NCE));
   /* B1: Make data port an output (if required by port architecture) */
   sgwrby(DATADIR,DATABUS_IS_OUTP);
   /* B2: Write data to data port */
   sgwrby(DATAWR,dat);
   /* C: Set /WR active low, (Delay min 80 ns), */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) & ~NWR)); /* repeat this instruction if more delay is needed*/
   /* D: Set /WR passive high */
   /* E: Set /CE passive high */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) | NWR | NCE));
   }

/*
   Simulate a bus read operation for a LCD controller with an Intel
   like bus interface (i.e. use of separate /RD and /WR strobes).

   The address parameter adr is assumed to be either 0 or 1.
*/
SGUCHAR simrdby(SGUCHAR adr)
   {
   SGUCHAR dat;
   /* a: Set C/D line according to adr. Set /CE line active low */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) | CTRLMSK) & ((adr == 0) ? ~(NCE|CD) : ~NCE));
   /* b: Make data port an input (if required by port architecture) */
   sgwrby(DATADIR,DATABUS_IS_INP);
   /* c: Set /RD active low, (Delay min 150ns), */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) & ~NRD)); /* repeat this line if more delay is needed*/
   /* d: Read data from data port */
   dat = sgrdby(DATARD);
   /* e: Set /RD passive high, Set /CE passive high */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) | NRD | NCE));
   return dat;
   }

/*
  Initialize and reset LCD display.
  Is called before simwrby() and simrdby() is invoked for the first time

  The SED1335 reset line is toggled here if it connected to a bus port.
  (it may alternatively be hard-wired to the reset signal to the processors
  in the target system).

  The sim_reset() function is invoked automatically via the ginit() function.
*/
void sim_reset( void )
   {
   /* 1. Init data port setup (if required by port architecture) */
   sgwrby(DATADIR,DATABUS_IS_INP);
   /* 2. Make C/D, /RD, /WR, /CE (+ FS and /RST) to outputs (if required by port architecture) */
   sgwrby(CTRLDIR,CTRLBUS_IS_OUTP);
   sgwrby(CTRLWR, (sgrdby(CTRLRD) | CTRLMSK));
   /* 3. Set LCD reset line /RST active low   (if /RST is connected to a port bit) */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) & ~NRST));
   /* 4. Set LCD reset line /RST passive high (if /RST is connected to a port bit) */
   sgwrby(CTRLWR, (sgrdby(CTRLRD) | NRST));
   }

#endif /* GHW_SINGLE_CHIP */
#endif /* GHW_NOHDW */


