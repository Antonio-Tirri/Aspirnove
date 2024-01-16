/*
   Interface function for virtual (external) memory

   This module contains access functions for virtual memory.

   The getvmem(..) function must be implemented by the library
   user for the specific type of storage device.

*/
#include <getvmem.h>

/*
   Get data from virtual memory

   buf      = destination pointer to RAM storage location
   index    = source index for data in virtual font to be loaded to RAM
   numbytes = number of bytes to load to buffer

*/
void getvmem(GFONTDEVICE vf_device, SGUCHAR *buf, GFONTBUFIDX index, SGUINT numbytes)
   {
   /* Add code here which copy data from external memory (font)
      storage to RAM buffer */

   /* If multiple different storage locations then vf_device can be used for
      storage location selection, and if different storage types are used, for
      storage device driver selection*/

   /*
     Set buffer array
         from buf[0] to buf[numbytes-1]
     equal to content in virtual storage memory
         from ext_memory[index] to ext_memory[index + numbytes-1]
   */
   buf++;    /* dummy operations to silence compiler, ex when module is used as a test stup. */
   vf_device++;
   index++;
   numbytes++;
   }

/*
   Returns != 0 if font (device type) is loaded / located in display controller memory
   (Is only called if bothe GHW_ACCELERATOR and  GHW_SYMCOPY_ACCELERATOR are defined in gdispcfg.h)
*/
SGUCHAR getvmem_is_loaded( GFONTDEVICE vf_device )
   {
   vf_device++; /* dummy operation to silence compiler, ex when module is used as a test stup. */
   return 0;
   }
/*
   Called by application before first use of virtual font
   Return == 0 if no errors
   Return != 0 if some errors

   Leave an empty function if operation is not relevant for memory hardware
*/
int getvmem_open( GFONTDEVICE vf_device )
   {
   vf_device++; /* dummy operation to silence compiler, ex when module is used as a test stup. */
   return 0;
   }

/*
   Optionally called by application after last use of virtual font

   Leave an empty function if operation is not relevant for memory hardware
*/
void getvmem_close( GFONTDEVICE vf_device )
   {
   vf_device++; /* dummy operation to silence compiler, ex when module is used as a test stup. */
   }


