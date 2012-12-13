/*
**  DRVALID.C - validate disk drives
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Uses ABSDISKC.C and ABSDISK.ASM in SNIPPETS.
*/

#include <dos.h>
#include <stdlib.h>
#include "dosfiles.h"
#include "snpdskio.h"

/*
**  getdrv()
**
**  Just as getcwd() returns the default directory, getdrv() returns
**  the current drive.
**
**  Arguments: None.
**
**  Returns:   Current drive (0 = A:, 1 = B:, etc.)
**
**  Side effects: none
*/

int getdrv(void)
{
      union REGS regs;

      regs.h.ah = 0x19;
      intdos(&regs, &regs);
      return (regs.h.al);
}

/*
**  chdrv()
**
**  Like chdir(), except changes drives rather than directories.
**
**  Arguments: 1 - target drive (0 = A:, 1 = B:, etc.)
**
**  Returns: Success_ or Error_
**
**  Side effects: none
*/

Boolean_T chdrv(int drive)
{
      union REGS regs;

      regs.h.ah = 0x0e;
      regs.h.dl = (char)drive;
      intdos(&regs, &regs);
      if (drive != getdrv())
            return Error_;
      else  return Success_;
}

/*
**  drvalid()
**
**  Verifies whether a logical disk drive is available without
**  triggering the DOS critical error handler.
**
**  Arguments: 1 - target drive (0 = A;, 1 = B:, etc.)
**
**  Returns:   True_  - drive is valid
**             False_ - drive is invalid
**
**  Side effects: none
*/

Boolean_T drvalid(int drive)
{
      int original, result;

      original = getdrv();
      result   = (Success_ == chdrv(drive));
      chdrv(original);
      return result;
}

/*
**  drvrdy()
**
**  Checks whether a drive with removable media is ready.
**
**  Arguments: 1 - target drive (0 = A;, 1 = B:, etc.)
**
**  Returns:   True_  - drive is ready
**             False_ - drive is not ready
**             Error_ - other read error
**
**  Side effects: none
*/

Boolean_T drvrdy(int drive)
{
      int status;
      char buf[2048];         /* nice & roomy   */

      status = AbsDiskRead(drive, 1, 0, buf);
      if (0 == status)
            return True_;
      status &= 0xff;
      if (2 == status)
            return False_;
      else  return Error_;
}

#ifdef TEST

#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
      int drive;

      if (2 > argc)
      {
            puts("Usage: DRVALID drive[:]");
            return EXIT_FAILURE;
      }
      drive = toupper(*argv[1]);
      if (!isalpha(drive))
      {
            puts("Error: Invalid drive name");
            return EXIT_FAILURE;
      }
      printf("Drive %c: is %svalid\n", drive,
            drvalid(drive - 'A') ? "" : "not ");
      if (2 < _osmajor)
      {
            union REGS regs;

            regs.x.ax = 0x4408;
            regs.h.bl = (unsigned char)(drive - '@');
            intdos(&regs, &regs);
            printf("ioctl returned Cflag=%s\n",
                  regs.x.cflag ? "TRUE" : "FALSE");
            printf("ioctl returned AX=0x%X\n", regs.x.ax);
            printf("Drive %c is%s removable\n", drive,
                  regs.x.ax ? " not" : "");
            if (0 == regs.x.ax)
            {
                  printf("Drive %c is %sready\n", drive,
                        drvrdy(drive - 'A') ? "" : "not ");
            }
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
