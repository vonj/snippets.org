/*
**  MDASORT.C - Test program for mdalloc()/amalloc() demonstrating sorting
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "snparray.h"
#include "bc_rand.h"

int compare(const void *elem1, const void *elem2);
int posn;

void usage(void)
{
      puts("Usage: MDATEST {A | M}");
      puts("where: A means to use amalloc()");
      puts("       M means to use mdalloc()");
      exit(EXIT_FAILURE);
}

main(int argc, char *argv[])
{
      int i, arg;
      char **ary;

      if (argc < 2)
            usage();
      arg = toupper(*argv[1]);
      if (arg != 'M' && arg != 'A')
            usage();
      printf("MDATEST: Using %s\n\n",
             ('M' == arg) ? "mdalloc()" : "amalloc()");

      /*
      **  Create a 5 x 40 array of char to hold 5 strings
      */

      if ('M' == arg)
      {
            if (NULL == (ary = mdalloc(sizeof(char), 5, 40)))
            {
                  puts("mdalloc() failed");
                  return EXIT_FAILURE;
            }
      }
      else
      {
            if (NULL == (ary = amalloc(sizeof(char), NULL, 2, 5, 40)))
            {
                  puts("amalloc() failed");
                  return EXIT_FAILURE;
            }
      }

      /*
      **  Fill the array with recognizable random strings
      */

      randomize();
      for (i = 0; i < 5; ++i)
      {
            posn = sprintf(ary[i], "String #%d val = ", i + 1);
            sprintf(ary[i] + posn, "%04X", random(UINT_MAX));
      }

      /*
      **  Display the unsorted array
      */

      for (i = 0; i < 5; ++i)
            printf("%d - %s\n", i, ary[i]);
      puts("");

      /*
      **  Sort the array and display it again
      */

      qsort(ary, 5, sizeof(char *), compare);

      for (i = 0; i < 5; ++i)
            printf("%d - %s\n", i, ary[i]);

      return EXIT_SUCCESS;
}

int compare(const void *elem1, const void *elem2)
{
      char *e1 = *((char **)elem1), *e2 = *((char **)elem2);
      long l1, l2;

      l1 = (unsigned)strtol(&e1[posn], NULL, 16);
      l2 = (unsigned)strtol(&e2[posn], NULL, 16);
      return (l2 < l1) ? 1 : ((l1 == l2) ? 0 : -1);
}
