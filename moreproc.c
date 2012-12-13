/*
**  MOREPROC.C - Display lines to the screen using more-style processing
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include "more.h"

static int rows, cols;

/*
**  more_proc() - Display a line using more-style processing
**
**  Parameters: 1 - Line to display
**
**  Returns: Key_ESC ('\x1b') if ESC pressed, else 0
**
**  Notes: Reads screen size from BIOS
**         Handles long line wrapping
**         Space to single step
**         ESC to exit via calling function
**         Any other key to continue until screen full
*/

int more_proc(char *str)
{
      static int linecnt = 0;
      char linebuf[256];

      if (!rows || !cols)
      {
            rows = SCREENROWS;
            cols = SCREENCOLS;
      }

      if (strlen(str) == ((size_t)cols + 1) && LAST_CHAR(str) == '\n')
                  LAST_CHAR(str) = NUL;

      while (strlen(str) > (size_t)cols)
      {
            strn1cpy(linebuf, str, cols);
            linebuf[cols] = NUL;
            more_proc(linebuf);
            strMove(str, str + cols);
      }

      if ((rows - (++linecnt)) < 2)
      {
            int ch;

            fputs("[...more...]", stderr);
            fflush(stderr);

            switch (ch = ext_getch())
            {
            case ' ':
                  --linecnt;
                  break;

            case Key_ESC:
                  fputs("\r[...Aborted...]\n", stderr);
                  return ch;

            default:
                  linecnt = 0;
            }
            fputs("\r            \r", stderr);
            fflush(stderr);
      }

      fputs(str, stderr);
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
                  if (Key_ESC == more_proc(buf))
                        break;
            }
      }
      return 0;
}

#endif /* TEST */
