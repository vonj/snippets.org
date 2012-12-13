/*
**  ISXKBRD.C - public domain by Ed Kowalski.
**
**  isxkeybrd() - detects enhanced kbd
*/

#include <dos.h>
#include "snipkbio.h"

/*
**  Check for enhanced keyboard support.
*/

int isxkeybrd(void)
{
      union REGS rg;
      unsigned kbdflags;

      rg.h.ah = 0x02;               /* check BIOS supports enhanced kbd */
      int86(0x16, &rg, &rg);        /* get kbd flags                    */
      kbdflags = rg.h.al;

      /* mess 'em up, get enhanced flags */

      rg.x.ax = 0x1200 + kbdflags ^ 0xff;
      int86(0x16, &rg, &rg);
      if (rg.h.al == kbdflags)      /* BIOS supports enhanced keyboard  */
      {
            /* if bit 4 at 40:96h is set machine has an enhanced kbd */

            if ((*(( char far *) 0x400096L) & 0x10))
                  return 1;         /* enhanced keyboard present        */
      }
      return 0;                     /* don't use enhanced keyboard calls */
}

#ifdef TEST

#include <stdio.h>
main()
{
      if (isxkeybrd())
            puts( "Enhanced Keyboard supported" );
      else  puts( "Enhanced Keyboard NOT supported ");
      return 0;
}

#endif /* TEST */
