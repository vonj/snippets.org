/*
**  TABTRICKs.C - Demonstrates how to use printf() for columnar formatting
*/

#include <stdio.h>
#include <string.h>

#define putnum(i) putchar(i+'0')

main()
{
      char *firstname[] = { "Aloysius",   "Bob",   "Dennis",  NULL },
           *lastname[]  = { "Fuddrucker", "Stout", "Ritchie", NULL };
      int   score[]     = { -10,          70,      200,       0    },
            i, tabwidth;

      printf("%15sStudent Name%30s\n\n", "", "Test Score");
      for (i = 0; NULL != lastname[i]; ++i)
      {
            tabwidth = 36                             /* spaces to tab  */
                       -2                             /* allow for ", " */
                       -strlen(lastname[i]);          /* lastname space */
            printf("%15s%s, %-*s%3d\n",
                  "", lastname[i], tabwidth, firstname[i], score[i]);
      }

      /* print a ruler to make things clearer   */

      puts("");
      for (i = 0; i < 71; ++i)
      {
            if (i == 10 * (i / 10))
                  putnum(i / 10);
            else  putchar(' ');
      }
      puts("");
      for (i = 0; i < 71; ++i)
            putnum(i % 10);
      return 0;
}

/*

RESULTS:

               Student Name                    Test Score

               Fuddrucker, Aloysius                -10
               Stout, Bob                           70
               Ritchie, Dennis                     200

0         1         2         3         4         5         6         7
01234567890123456789012345678901234567890123456789012345678901234567890

*/
