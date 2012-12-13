/*
**  iscons()
**
**  A function to determine if a specified stream refers to the console.
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <dos.h>
#include "dosfiles.h"

int iscons(FILE *fp)
{
      union REGS regs;

      regs.x.ax = 0x4400;
      regs.x.bx = (unsigned)fileno(fp);
      intdos(&regs, &regs);
      if (0 == (regs.x.ax & 0x80))
            return 0;
      return TOBOOL(regs.x.ax & 0x13);
}

#ifdef TEST

int main(void)
{
      fprintf(stderr, "stdin is%s redirected\n",
            iscons(stdin) ? " not" : "");
      fprintf(stderr, "stdout is%s redirected\n",
            iscons(stdout) ? " not" : "");
      return 0;
}

#endif /* TEST */
