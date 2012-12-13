/* +++Date last modified: 05-Jul-1997 */

#include <stdio.h>
#include "xfile.h"

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

int main(int argc, char **argv)
{
      while (*++argv != 0)
      {
            XFILE *f = xopen(*argv);

            if (f == 0)
                  fprintf(stderr, "ERROR: can't open file %s\n", *argv);
            else
            {
#if 0
                  char *s;

                  fprintf(stdout, "--- %s ---\n", *argv);
                  while ((s = xgetline(f)) != NULL)
                        fputs(s, stdout);
                  xclose(f);
#else
                  unsigned int nLines = 0;
                  char *s;

                  while (xgetline(f) != NULL)
                        ++nLines;
                  printf("%5u lines in %s\n", nLines, *argv);
                  xclose(f);
#endif
            }
      }

      return 0;
}
