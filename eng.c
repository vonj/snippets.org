/* ENG.C - Format floating point in engineering notation          */
/* Released to public domain by author, David Harmon, Jan. 1994   */
/* Modifed by Dan Stubbs & Bob Stout, 14-Jul-1997                 */

#include <stdio.h>
#include "numcnvrt.h"

char *eng(double value, int places)
{
      const char * const prefixes[] = {
            "a", "f", "p", "n", "æ", "m", "", "k", "M", "G", "T"
            };
      int p = 6;
      static char result[30];
      char *res = result;

      *res++ = (value >= 0) ? ' ' : '-';
      value  = (value >= 0) ? value : -value;

      for ( ; p > 0 && value != 0 && value < 1.0; --p)
            value *= 1000.0;

      for ( ; p < 10 && value != 0 && value > 1000.0; ++p)
            value /= 1000.0;

      places = (value >= 100.0) ? places-2 :
               (value >=  10.0) ? places-1 : places;

      sprintf(res, "%.*f %s", places-1, value, prefixes[p]);

      return result;
}

#ifdef TEST

#include <stdio.h>

main()
{
      double w;

      for (w = 1e-19; w < 1e16; w *= 42)
            printf(" %g W = %sW\n", w, eng(w, 3));
      return 0;
}
#endif /* TEST */
