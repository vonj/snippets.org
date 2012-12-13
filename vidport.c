/*
**  Portable PC screen functions
**  Public domain by Bob Stout
**  Uses SCROLL.C, also from SNIPPETS
*/
 
#include <stdio.h>
#include <dos.h>
#include "scrnmacs.h"         /* Also in SNIPPETS     */
#include "mk_fp.h"

void GotoXY(int col, int row)
{
      union REGS regs;

      setbuf(stdout, NULL);
      regs.h.dh = (unsigned char)row;
      regs.h.dl = (unsigned char)col;
      regs.h.bh = VIDPAGE;
      regs.h.ah = 2;
      int86(0x10, &regs, &regs);
}

void ClrScrn(int vattrib)
{
      scroll(SCROLL_UP, 0, vattrib, 0, 0, SCREENROWS, SCREENCOLS);
      GotoXY(0, 0);                     /* Home cursor  */
}

void GetCurPos(int *col, int *row)
{
      union REGS regs;

      regs.h.ah = 0x03;
      regs.h.bh = VIDPAGE;
      int86(0x10, &regs, &regs);
      *row = regs.h.dh;
      *col = regs.h.dl;
}

int GetCurAtr(void)
{
      int row, col;
      unsigned short chat;

      GetCurPos(&col, &row);
      chat = *((unsigned short FAR *)MK_FP(SCREENSEG,
            (row * SCREENCOLS + col) << 1));
      return (chat >> 8);
}

void ClrEol(void)
{
      int row, col;

      GetCurPos(&col, &row);
      scroll(0, 0, GetCurAtr(), row, col, row, SCREENCOLS);
}

void ClrEop(void)
{
      int row, col;

      GetCurPos(&col, &row);
      ClrEol();
      if (++row < SCREENROWS)
            scroll(0, 0, GetCurAtr(), row, 0, SCREENROWS, SCREENCOLS);
}

void Repaint(int vattrib)
{
      unsigned short FAR *screen = (unsigned short FAR *)SCRBUFF;
      int row, col;

      for (row = 0; row < SCREENROWS; ++row)
      {
            for (col = 0; col < SCREENCOLS; ++col, ++screen)
                  *screen = (*screen & 0xff) + (vattrib << 8);
      }
}


#ifdef TEST

#include <conio.h>

/*
**  Run this test with a screenful of misc. stuff
*/

main()
{
      int vatr = GetCurAtr();

      GotoXY(1, 1);
      fputs("Testing ClrEol()", stderr);
      ClrEol();
      fputs("\nHit any key to continue...\n", stderr);
      getch();
      fputs("Testing ClrEop()", stderr);
      ClrEop();
      fputs("\nHit any key to continue...\n", stderr);
      getch();
      ClrScrn(vatr);
      GotoXY(0, 0);
      fputs("ClrScrn() tested", stderr);
      fputs("\nHit any key to continue...\n", stderr);
      getch();
      Repaint(BG_(CYAN_) | BLACK_);
      fputs("Repaint() tested", stderr);
      fputs("\nHit any key to continue...\n", stderr);
      getch();
      Repaint(vatr);
      return 0;
}

#endif /* TEST */
