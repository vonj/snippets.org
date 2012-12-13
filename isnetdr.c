/*----------------------------------------------------------------------*/
/*  determine_drive_type -- Public Domain code from Bob Dolan           */
/*                                                                      */
/*  INPUT:  the drive number ( 0=current, 1=A:, 2=B:, etc. )            */
/*  OUTPUT: drive type ( 0=physical drive, 1=Network drive, 2=RamDisk ) */
/*----------------------------------------------------------------------*/
 
#include <dos.h>
#include "dosfiles.h"
 
int drive_type(int dr)
{
      union REGS regs;
 
      regs.x.ax = 0x4409;     /* IOCTL func 9 */
      regs.h.bl = (unsigned char)dr;
      int86(0x21, &regs, &regs);
      if (!regs.x.cflag)
      {
            if (regs.x.dx & 0x1000)
                  return 1;   /* Network drive */

            else if (regs.x.dx == 0x0800)
                  return 2;   /* RAMdisk */
      }
 
      return 0;   /* physical drive */
}
 
#ifdef TEST
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
 
int main(int argc, char *argv[])
{
      int dr = 0;
 
      if (1 < argc)
            dr = toupper(*argv[1]) - '@';
      printf ("drive_type(%d) = %d\n", dr, drive_type(dr));
      return 0;
}
 
#endif /* TEST */
