/*
**  isRamDsk() - Determine if a drive is a RAM disk
**
**  Call with drive letter ('a' - 'z', 'A' - 'Z')
**
**  Returns True_, False_, or Error_
**
**  Uses ABSDISKC.C, ABSDISK.ASM, and DOS5BOOT.H from SNIPPETS
**  (Note: The relevant parts of the structure in DOS5BOOT.H are
**   also applicable to lower version numbers of DOS)
**
**  Public domain by Bob Stout
*/

#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include "dos5boot.h"
#include "dosfiles.h"
#include "snpdskio.h"

Boolean_T isRamDsk(unsigned char drive)
{
      union REGS regs;
      B_REC buffer;

      regs.x.ax = 0x4408;           /* Not if removable     */
      regs.h.bl = (unsigned)toupper(drive) - (unsigned char)'@';
      intdos(&regs, &regs);
      if (0 == regs.x.ax)
            return False_;
      if (AbsDiskRead(toupper(drive) - 'A', 1, 0, &buffer))
            return Error_;
      return (1 == buffer.bsFATs);
} 

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
      if (2 > argc)
      {
            puts("Syntax: ISRAMDSK drive_letter");
            return EXIT_FAILURE;
      }
      printf("Drive %c: is%s a RAM drive\n", toupper(*argv[1]),
            isRamDsk(*argv[1]) ? "" : " not");
      return EXIT_SUCCESS;
}

#endif /* TEST */
