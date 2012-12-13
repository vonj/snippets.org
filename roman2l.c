/*
**  ROMAN2L.C Covert roman numerals to long integers.
**
**  public domain by Bob Stout
*/

#include <ctype.h>
#include "sniptype.h"

struct numeral {
      long val;
      int  ch;
};

static struct numeral numerals[] = {
      {    1L, 'I' },
      {    5L, 'V' },
      {   10L, 'X' },
      {   50L, 'L' },
      {  100L, 'C' },
      {  500L, 'D' },
      { 1000L, 'M' }
};

/*
**  roman2long() - Converts a roman numeral string into a long integer
**
**  Arguments: 1 - Roman numeral string
**
**  Returns: Long value if valid, else -1L
*/

long roman2long(const char *str)
{
      int i, j, k;
      long retval = 0L;

      if (!str || NUL == *str)
            return -1L;
      for (i = 0, k = -1; str[i]; ++i)
      {
            for (j = 0; j < 7; ++j)
            {
                  if (numerals[j].ch == toupper(str[i]))
                        break;
            }
            if (7 == j)
                  return -1L;
            if (k >= 0 && k < j)
            {
                  retval -= numerals[k].val * 2;
                  retval += numerals[j].val;
            }
            else  retval += numerals[j].val;
            k = j;
      }
      return retval;
}

#ifdef TEST

#include <stdio.h>

int main(int argc, char *argv[])
{
      while (--argc)
      {
            ++argv;
            printf("roman2long(%s) returned %ld\n", *argv, roman2long(*argv));
      }
}

#endif /* TEST */
