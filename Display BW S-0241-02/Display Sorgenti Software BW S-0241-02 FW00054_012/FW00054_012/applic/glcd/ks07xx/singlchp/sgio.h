/*---------------------------------------------------------------------------*
 * Generic template for sgio.h
 *
 * It is a reduced and simplified version of the SGIO.H file generated with
 * the SGSETUP tool. Modify this file to fit the target C compiler:
 * 
 * This header file contains:
 * -    Definitions of the portable SG I/O function syntax. 
 * -    Includes I/O register definitions for target in SGIO_PC.H or SGIO_TA.H
 * -    Includes fixed integer size definitions from SGTYPES.H 
 * 
 * (SG interrupt and SG I/O buffer declarations have been omitted from this
 * simplified and reduced version of SGIO.H)
 * 
 * Copyright (c) RAMTEX International 2000
 * Version 1.0
 * 
 * A description of the SG syntax can be found at: 
 *     www.ramtex.dk/standard/sgsyntax.htm
 * 
 *--------------------------------------------------------------------------*/
#ifndef SGIO_H
#define SGIO_H

#include <sgtypes.h>

#ifdef  SGPCMODE
   /* This part are used when compiling with PC compilers. 
      Ignore this part if you only use the target C compiler. 
      These prototype definitions should never need modifications. */
   
   #ifdef __cplusplus
   extern "C" {
   #endif

   /* Prototypes for SG I/O functions for compilation with
      PC compilers. The purpose is to be able to do PC compilation
      check of modules doing I/O. Will check for instance the function 
      parameters as this is real function prototypes instead of macros. */

   /* bit access to target CPU : (may not be supported) */
   void sgfnc sgwrbi( enum access_type paddr, SGBOOL pval );
   SGBOOL sgfnc sgrdbi( enum access_type paddr );
   void sgfnc sgandbi( enum access_type paddr, SGBOOL pval );
   void sgfnc sgorbi( enum access_type paddr, SGBOOL pval );

   /* unsigned byte access to target CPU : */
   void sgfnc sgwrby( enum access_type paddr, SGUCHAR pval );
   SGUCHAR sgfnc sgrdby( enum access_type paddr );
   void sgfnc sgandby( enum access_type paddr, SGUCHAR pval );
   void sgfnc sgorby( enum access_type paddr, SGUCHAR pval );

   /* unsigned word access to target CPU : */
   void sgfnc sgwrwo( enum access_type paddr, SGUINT pval );
   SGUINT sgfnc sgrdwo( enum access_type paddr );
   void sgfnc sgandwo( enum access_type paddr, SGUINT pval );
   void sgfnc sgorwo( enum access_type paddr, SGUINT pval );

   /* unsigned double word (long word) access to target CPU : */
   void sgfnc sgwrdw( enum access_type paddr, SGULONG pval );
   SGULONG sgfnc sgrddw( enum access_type paddr );
   void sgfnc sganddw( enum access_type paddr, SGULONG pval );
   void sgfnc sgordw( enum access_type paddr, SGULONG pval );

   /* Make enumerated list of target peripherals (access_type names): 
      from the sgio_pc.h file generated by the SGSETUP tool.

      Note that the SGSETUP tool normally uses the sgio_pc.h files
      as a generic master file for generating the corresponding
      sgio_ta.h file with the I/O definition syntax used by the
      target compiler. The sgio_ta.h file is included below.
      Note that the sgio_pc.h file is not necessary if the PC 
      compilation mode is not used */
   #define SGPORTELEM( rwdir, accessty, dataty, portaddr, subadress, portname ) \
      portname

   /* Create lists of symbolic names for target specific peripherals. */
   enum access_type {
      #include <sgio_pc.h>  
      SGLASTPORT
      };
   #undef SGPORTELEM

   #ifdef __cplusplus
   }
   #endif

#else
   /* This part is used with the target C compiler.

      Macro definitions for SG I/O functions are used when compiling with 
      the target compiler.

      Modify them to fit the syntax for I/O access used by the target C 
      compiler if necessary.

      The purpose of these macro functions is to map between the
      "standardized" SG syntax used by portable libraries and the
      non-standardized syntax used by the target compiler.

      The implementation below just assumes that the target compiler uses a 
      simple assignment syntax for access operations on I/O registers.
      (i.e. just like memory mapped I/O). 
   */

   /* bit access to target CPU : 
     (Delete if not possible with the target compiler) */
   #define sgwrbi( PADDR, PVAL ) ( PADDR = ( PVAL ))
   #define sgrdbi( PADDR ) (( PADDR ))
   #define sgorbi( PADDR, PVAL ) ( PADDR |= ( PVAL ))
   #define sgandbi( PADDR, PVAL ) ( PADDR &= ( PVAL ))

   /* byte access to target CPU : */
   #define sgwrby( PADDR, PVAL ) ( PADDR = ( PVAL ))
   #define sgrdby( PADDR ) (( PADDR ))
   #define sgorby( PADDR, PVAL ) ( PADDR |= ( PVAL ))
   #define sgandby( PADDR, PVAL ) ( PADDR &= ( PVAL ))

   /* word access to target CPU : */
   #define sgwrwo( PADDR, PVAL ) ( PADDR = ( PVAL ))
   #define sgrdwo( PADDR ) (( PADDR ))
   #define sgorwo( PADDR, PVAL ) ( PADDR |= ( PVAL ))
   #define sgandwo( PADDR, PVAL ) ( PADDR &= ( PVAL ))

   /* double word access (long word) to target CPU : */
   #define sgwrdw( PADDR, PVAL ) ( PADDR = ( PVAL ))
   #define sgrddw( PADDR ) (( PADDR ))
   #define sgordw( PADDR, PVAL ) ( PADDR |= ( PVAL ))
   #define sganddw( PADDR, PVAL ) ( PADDR &= ( PVAL ))

   /* Include definitions of the symbolic names and locations 
      for I/O registers in target.
      sgio_ta.h must use the syntax for I/O register definitions 
      required by the target compiler. 
      (If the target compiler have already defined I/O registers
      for the target processor in a header file, it may be enough just
      to copy this file to sgio_ta.h and then add any I/O register 
      definitions used by the portable I/O library.)
   */
   #include <sgio_ta.h>          

#endif

#endif

/* eof sgio.h */
