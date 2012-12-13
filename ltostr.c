/*
**  LTOSTR.C - An improved, safer, ltoa()
**
**  On call:
**  num       =   number to convert
**  string    =   buffer for output
**  max_chars =   maximum size of buffer
**  base      =   number base for conversion.
**
**  Return value:
**  if illegal base
**    NULL
**  beginning of converted number.
**
**  notes: if number is too large in magnitude to fit in the buffer,
**  the MOST significant digits will be truncated.  If the number is
**  negative, a leading '-' will be placed in the buffer even if this
**  causes the most significant digit to be truncated.
**  The number is right justified in the buffer and the location of the
**  first character in the number is returned so:
**    If you want right justification, use the original string.
**    If you want left justification, use the returned string.
**  If the number doesn't fill the buffer:
**       leading characters will be filled with spaces.
**
**  public domain by Jerry Coffin
*/

#include <stdio.h>
#include <string.h>
#include "numcnvrt.h"

char *ltostr(long num, char *string, size_t max_chars, unsigned base)
{
      char remainder;
      int sign = 0;   /* number of digits occupied by the sign. */

      if (base < 2 || base > 36)
            return NULL;

      if (num < 0)
      {
            sign = 1;
            num = -num;
      }

      string[--max_chars] = '\0';

      for (max_chars--; max_chars > sign && num!=0; max_chars --)
      {
            remainder = (char) (num % base);
            if ( remainder < 9 )
                  string[max_chars] = remainder + '0';
            else  string[max_chars] = remainder - 10 + 'A';
            num /= base;
      }

      if (sign)
            string[--max_chars] = '-';

      if ( max_chars > 0 )
            memset(string, ' ', max_chars+1);

      return string + max_chars;
}

#ifdef TEST

#include <stdlib.h>

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif

#ifdef __TURBOC__
 #pragma argsused
#endif

#define SIZE 50

int main(int argc, char *argv[])
{
      char buffer[SIZE];

      long number   = atoi(argv[1]);
      unsigned base = atoi(argv[2]);

      printf("%ld in base %u is \"%s\"\n", number, base,
            ltostr(number, buffer, SIZE, base));

      return 0;
}

#endif /* TEST */
