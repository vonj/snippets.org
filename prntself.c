/*
**  PRNTSELF.C - A program which prints its own source
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <string.h>

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif

#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      FILE *in;
      char fname[13], *ptr;
      char line[1024];        /* Nice & roomy   */

      /*
      ** Get the source name by replacing the executable's COM or EXE with C
      */

      strcpy(fname, argv[0]);
      ptr = strrchr(fname, '.');
      strcpy(++ptr, "C");

      /*
      ** Print its own source
      */

      if (NULL != (in = fopen(fname, "r")))
      {
            while (NULL != fgets(line, 1024, in))
                  fputs(line, stdprn);
            return 0;
      }
      else  return -1;
}
