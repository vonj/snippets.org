/*
**  ADJSCROL.C - Display lines to the screen, adjusting the scroll rate
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include "more.h"

static int cols;
static long pause = 500;                  /* Default to 1/2 second      */

/*
**  adj_scroll() - Display a line, dynamically adjusting the scroll rate
**
**  Parameters: 1 - Line to display
**
**  Returns: Key_ESC ('\x1b') if ESC pressed, else 0
**
**  Notes: Reads screen size from BIOS
**         Handles long line wrapping
**         Key_ESC to exit via calling function
**         Key_PGUP to speed scrolling
**         Key_PGDN to slow scrolling
*/

int adj_scroll(char *str)
{
      char linebuf[256];
      int ch;

      if (!cols)
            cols = SCREENCOLS;

      if (strlen(str) == ((size_t)cols + 1) && LAST_CHAR(str) == '\n')
                  LAST_CHAR(str) = NUL;

      while (strlen(str) > (size_t)cols)
      {
            strn1cpy(linebuf, str, cols);
            linebuf[cols] = NUL;
            adj_scroll(linebuf);
            strMove(str, str + cols);
      }

      fputs(str, stderr);
      delay((int)pause);

      if (EOF != (ch = ext_inkey()))
      {

            switch (ch)
            {
            case Key_ESC:
                  return ch;

            case Key_PGUP:
                  pause *= 3L;            /* Reduce pause by 25%        */
                  pause /= 4L;
                  break;

            case Key_PGDN:
                  pause *= 125L;          /* Increase pause by 25%      */
                  pause /= 100L;
                  break;
            }
      }
      return 0;
}

#ifdef TEST

#include "errors.h"                             /* For cant()           */

main(int argc, char *argv[])
{
      FILE *fp;
      char buf[512];

      while (--argc)
      {
            fp = cant(*++argv, "r");

            while (!feof(fp))
            {
                  if (NULL == fgets(buf,  512, fp))
                        break;
                  if (Key_ESC == adj_scroll(buf))
                        break;
            }
      }
      return 0;
}

#endif /* TEST */
