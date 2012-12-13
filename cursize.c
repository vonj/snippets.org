/*
**  Program to set the size of the cursor
**
**  Public domain demonstration by Bob Jarvis
*/

#include <stdio.h>      /* puts()               */
#include <dos.h>        /* int86(), union REGS  */
#include <stdlib.h>     /* exit(), atoi()       */

char *help =  "CURSIZE - sets the cursor size.\n"
              "Usage:\n"
              "   CURSIZE <top-line> <bottom-line>\n"
              "where\n"
              "   top-line = top line of cursor within character cell\n"
              "   bottom-line = bottom line\n"
              "Example:\n"
              "   CURSIZE 7 8     <set cursor to bottom 2 lines of VGA>\n"
              "   CURSIZE 32 32   <turns cursor off>";

void cursor_size(int top_line, int bottom_line)
{
      union REGS regs;

      regs.h.ah = 1;
      regs.h.ch = (unsigned char)top_line;
      regs.h.cl = (unsigned char)bottom_line;

      int86(0x10,&regs,&regs);
}

void get_cursor_size(int *top_line, int *bottom_line)
{
      union REGS regs;

      regs.h.ah = 3;
      regs.h.bh = 0;
      int86(0x10, &regs, &regs);

      *top_line = regs.h.ch;
      *bottom_line = regs.h.cl;

      return;
}

main(int argc, char *argv[])
{
      int top, bottom;

      if(argc < 3)
      {
            puts(help);
            exit(1);
      }

      top = atoi(argv[1]);
      bottom = atoi(argv[2]);

      cursor_size(top,bottom);

      top = bottom = -1;

      get_cursor_size(&top, &bottom);

      printf("top = %d  bottom = %d\n", top, bottom);
      return 0;
}
