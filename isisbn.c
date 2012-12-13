/*
**  ISISBN.C - Validate International Standard Book Numbers (ISBNs)
**
**  public domain by Maynard Hogg
*/

#include <ctype.h>
#include "isisbn.h"

int isbn2(char *str)
{
      int i = 0;
      int test = 0;
      int c;

      while ('\0' != (c = *str++))
      {
            if (isdigit(c))
                  c -= '0';
            else if (i == 9 && 'X' == c)
                  c = 10;
            else continue;
            test += c * ++i;
      }
      return (i == 10 && test % 11 == 0);
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      while (--argc)
      {
            int r = isbn2(*++argv);

            printf("%s is%s a valid ISBN number\n", *argv, r ? "" : " not");
      }
      return 0;
}

#endif /* TEST */
