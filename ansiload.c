/*
**  ANSILOAD.C - tries to detect if an ANSI-style driver is loaded
**
**  public domain by Bob Jarvis
*/

#include <stdio.h>
#include <dos.h>
#include "sniptype.h"
#include "ansiload.h"

static void goto_rc(int row, int col)
{
      union REGS regs;

      regs.h.ah = 2;
      regs.h.bh = 0;                /* assumes we're using video page 0 */
      regs.h.dh = (unsigned char)row;
      regs.h.dl = (unsigned char)col;

      int86(0x10, &regs, &regs);
}

static void get_rc(int *row, int *col)
{
      union REGS regs;

      regs.h.ah = 3;
      regs.h.bh = 0;                /* again, assume video page 0       */

      int86(0x10, &regs, &regs);

      *row = regs.h.dh;
      *col = regs.h.dl;
}

int is_ansi_loaded(void)
{
      int save_r, save_c;
      int new_r, new_c;
      int isloaded;

      get_rc(&save_r, &save_c);
      goto_rc(15,15);
      fputs("\x1B[0;0H", stderr);

      get_rc(&new_r, &new_c);

      if(new_r == 0 && new_c == 0)
            isloaded = True_;
      else
      {
            isloaded = False_;
            fputs("\b\b\b\b\b\b      \b\b\b\b\b\b", stderr);
      }

      goto_rc(save_r, save_c);
      return isloaded;
}

#ifdef TEST

main()
{
      if(is_ansi_loaded())
            puts("ANSI.SYS is loaded");
      else  puts("ANSI.SYS is NOT loaded");
      return 0;
}

#endif /* TEST */
