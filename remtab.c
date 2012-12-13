/* remtab.c 12-4-91 Robert Mashlan, Public Domain
   modified 28 mar 93 by Bob Stout

   Filter for removing tabs.  All tabs in the input will be replaced
   with spaces.  This filter takes one optional command line
   parameter, which specifies the number spaces to replace for a tab.
   If no size is specifies, it defaults to 8.

   example usage:

   remtab 6 < tabbed.c > untabbed.c

*/

#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 4096


int main(int argc, char **argv )
{
      int tabsize = 8;

      if (argc > 1)                 /* look for command line parameter */
      {
            if (0 == (tabsize = atoi(argv[1])))
                  tabsize = 8;
      }

      while (1)
      {
            char buf[BUFSIZE];
            int nr, i, j, pos = 0;

            nr = fread(buf, 1, sizeof(buf), stdin);
            for (i = 0; i < nr; i++)
            {
                  switch (buf[i])
                  {
                  case '\t':              /* replace tabs with spaces   */
                        for(j = pos % tabsize; j < tabsize; ++j)
                        {
                              putchar(' ');
                              ++pos;
                        }
                        break;

                  case '\n':              /* start a new line           */
                        pos = -1;         /* this will become 0 when... */

                        /* ...we fall through to...   */

                  default:
                        putchar(buf[i]);/* send character through unchanged */
                        ++pos;
                  }
            }
            if (nr < sizeof(buf))
                  break;
      }
      return 0;
}
