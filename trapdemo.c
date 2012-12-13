/*
**  Demonstrate TRAPFLAG.ASM
**
**  public domain by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include "extkword.h"

#ifdef __WATCOMC__
 #pragma aux ins09  "_*" parm caller [] modify [ax bx cx dx es]
 #pragma aux undo09 "_*" parm caller [] modify [ax bx cx dx es]
#endif

extern void ins09(void);
extern void undo09(void);

extern volatile int FAR ccrcvd;

static void biosprt(char *p)
{
      union REGS regs;

      while (*p)
      {
            regs.h.ah = 0x0e;             /* Low-level services only!     */
            regs.h.al = *p++;
            regs.x.bx = 0;
            int86(0x10, &regs, &regs);
      }
}

static void FAR my_cc(void)
{
      char *p1 = "Ctrl-";
      char *p2 = "C";
      char *p3 = "Break";
      char *p4 = " received\r\n";

      biosprt(p1);
      if (1 == ccrcvd)
            biosprt(p2);
      else  biosprt(p3);
      biosprt(p4);
}

main()
{
      int ch = 0;

      setbuf(stdout, NULL);
      my_cc();
      ins09();
      atexit(undo09);
      puts("New Ints 09h & 1Bh installed...");
      puts("Hit Esc to quit...");
      do
      {
            if (kbhit())
            {
                  if (0x1b != (ch = getch()))
                  {
                        if (0x20 > ch)
                        {
                              fputc('^', stdout);
                              ch += '@';
                        }
                        fputc(ch, stdout);
                  }
            }
            if (ccrcvd)
            {
                  my_cc();
                  ccrcvd = 0;
            }
      } while (0x1b != ch);
      return 0;
}
