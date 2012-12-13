/*
**  ABSDISKC.C - Functions to read and write absolute disk sectors
**               (these will work with all versions of DOS 2-7).
**
**  Public domain code by Bob Stout
**
**  NOTE: These functions work by calling absdisk() from SNIPPETS file,
**        ABSDISK.ASM. Each returns 0 if successful.
*/

#include <stddef.h>
#include <dos.h>
#include "snpdskio.h"

int CDECL absdisk(unsigned char  function,
                   unsigned short drive,              /* 0 = A:, etc.   */
                   size_t         number_of_sectors,
                   size_t         starting_sector,
                   void *         sector_buffer);

int AbsDiskRead(unsigned short drive,
                size_t         num_of_sectors,
                size_t         sector,
                void          *ptr)
{
      return absdisk(0x25, drive, num_of_sectors, (unsigned)sector, ptr);
}

int AbsDiskWrite(unsigned short drive,
                size_t         num_of_sectors,
                size_t         sector,
                void *ptr)
{
      return absdisk(0x26, drive, num_of_sectors, (unsigned)sector, ptr);
}
