/*
**  Print a line of text, displaying Ctrl characters using leading carets
**  public domain by Bob Stout
*/

#include <stdio.h>
#include <string.h>
#include "ctrlprnt.h"

void ctrl_print(char *line)
{
      while (*line)
      {
            if (' ' > *line)
            {
                  putchar('^');
                  putchar('@' + (*line++));
            }
            else  putchar(*line++);
      }
      if (!strcmp((line - 2), "\x0d\x0a") || !strcmp((line - 2), "\x0a\x0d"))
            putchar('\n');
}

#ifdef TEST

#include <stdlib.h>
#include <ctype.h>

main()
{
      char *p, *test = "This is a test";

      for (p = strupr(test); *p; ++p)
      {
            if (isalpha(*p))
                  *p = *p - 64;
      }
      ctrl_print(test);
      return EXIT_SUCCESS;
}

#endif /* TEST */
