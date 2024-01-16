/***************************** bussimbi.c ************************************

   Generic template file for external bus simulator drivers.

   The functions in this module is called by single-chip-processor version
   of the ghwinit.c module.

   This implementation assumes that the processor port architecture supports
   BIT addressing capabilities and that BIT addressing is also supported by
   the target compiler.

   The compiler independent SG syntax notation is used in this example for
   clarification.
   (Alternatively these functions could be implemented using the compiler
    specific syntax for I/O port operations.)

   The following symbolic names for bit and byte ports must be declared for
   the actual processor architecture and target system:

   Bit ports:
      CD     (LCD address line)
      NWR    (LCD /WR line)
      NRD    (LCD /RD line)
      NCE    (LCD /CE line)
      NRST   (LCD /RST line, if connected to a port)

   Byte ports:
      DATAWR (LCD data write)
      DATARD (LCD data read. May be equal to DATAWR, depends on the processor)
      DATADIR (Processor data port direction setup, depends on the processor).
      CTRLDIR (Processor ctrl port direction setup, depends on the processor).

   Copyright (c) RAMTEX Engineering Aps 1998

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
#define DATABUS_IS_INP  0x00  /* 8 signal lines is used as input */
#define DATABUS_IS_OUTP 0xFF     /* 8 signal lines is used as outputs */
#define CTRLBUS_IS_OUTP 0x3F  /* 4-6 signal lines is used as output */

/*
   Simulate a bus write operation for a LCD controller with an Intel
   like bus interface (i.e. use of separate /RD and /WR strobes).

   The address parameter adr is assumed to be either 0 or 1.
*/
void simwrby(SGUCHAR adr, SGUCHAR dat)
   {
   /* A: Set C/D line according to adr, Set /CE line active low */
   sgwrbi(CD, (adr != 0));
   sgwrbi(NCE, 0);
   /* B1: Make data port an output (if required by port architecture) */
   sgwrby(DATADIR,DATABUS_IS_OUTP);
   /* B2: Write data to data port */
   sgwrby(DATAWR,dat);
   /* C: Set /WR active low, (Delay min 80 ns), */
   sgwrbi(NWR, 0);     /* repeat this instruction if more delay is needed*/
   /* D: Set /WR passive high */
   sgwrbi(NWR, 1);
   /* E: Set /CE passive high */
   sgwrbi(NCE, 1);
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
   sgwrbi(CD, (adr != 0));
   sgwrbi(NCE, 0);
   /* b: Make data port an input (if required by port architecture) */
   sgwrby(DATADIR,DATABUS_IS_INP);
   /* c: Set /RD active low, (Delay min 150ns), */
   sgwrbi(NRD, 0);     /* repeat this instruction if more delay is needed*/
   /* d: Read data from data port */
   dat = sgrdby(DATARD);
   /* e1: Set /RD passive high */
   sgwrbi(NRD, 1);
   /*, Set /CE passive high (could be ignored) */
   sgwrbi(NCE, 1);
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
   sgwrbi(NCD, 1);
   sgwrbi(NWR, 1);
   sgwrbi(NRD, 1);
   sgwrbi(NCE, 1);
   /* 3. Set LCD reset line /RST active low   (if /RST is connected to a port bit) */
   sgwrbi(NRST, 0);
   /* 4. Set LCD reset line /RST passive high (if /RST is connected to a port bit) */
   sgwrbi(NRST, 1);
   }


#endif /* GHW_SINGLE_CHIP */
#endif /* GHW_NOHDW */

