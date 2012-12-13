/*
**  COMMAFLT.C - Format a double using commas as thousands separators
**               and a specified number of significant fractional digits.
**
**  public domain by Bruce Wedding and Kurt Kuzba
*/
#include <stdio.h>

#include "numcnvrt.h"
#include "snip_str.h"

char *comma_float( double num, char *buf, int dec)
{
      char tmp[80];
      int bf = 0, cm = 0, tm = 9 - dec + (!dec);

      sprintf(tmp, "%.9f", num);
      strrev(tmp);
      if(dec)
      {
            while( (buf[bf++] = tmp[tm++]) != '.')
                  ;
            while((buf[bf++] = tmp[tm++]) != 0)
            {
                  if(++cm % 3 == 0 && tmp[tm])
                        buf[bf++] = ',';
            }
      }
      return strrev(buf);
}

#ifdef TEST

#include <stdio.h>

int main( void )
{
      int i;
      char result[80];
      double num[10] = {
            1.98765,       12.98765,       123.98765,
            1234.98765,    12345.98765,    123456.98765,
            1234567.98765, 12345678.98765, 123456789.98765,
            1234567890.98765  };

      for ( i = 0; i < 10; i++ )
            printf("Before: %-24.5f After: %s \n",
                  num[i], comma_float(num[i], result, 6));
      return 0;
}

#endif /* TEST */
