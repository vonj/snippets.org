/*
**  HEXORINT.C - Detect if a string denotes a hex or decimal
**  number by detecting a leading "0X" or trailing "H" string.
**
**  public domain demo by Bob Stout
*/

#include <stdlib.h>
#include <string.h>
#include "sniptype.h"
#include "numcnvrt.h"

/*
**  Let strtol() do most of the work
*/

long hexorint(const char *string)
{
      int radix = 0;
      char *dummy, valstr[128];

      strcpy(valstr, string);
      if (strchr("Hh", LAST_CHAR(valstr)))
      {
            LAST_CHAR(valstr) = NUL;
            radix = 16;
      }
      return strtol(valstr, &dummy, radix);
}

/*
**  Test code follows - compile with TEST macro defined to test
*/

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      long val;

      while (--argc)
      {
            val = hexorint(*(++argv));
            printf("Value of %s = %ld = %#lx\n", *argv, val, val);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
