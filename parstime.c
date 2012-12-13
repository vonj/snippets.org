/*
**  PARSTIME.C - A simple parser to extract times from strings.
**
**  Public domain by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include "datetime.h"


/*
**  parse_time() - Parse a time string into its components
**
**  Arguments: 1 - String to parse
**             2 - Address of hours storage
**             3 - Address of minutes storage
**             4 - Address of seconds storage
**
**  Returns: 0 For success, non-zero for range errors
*/

Boolean_T parse_time(const char *str,
                     unsigned   *hours,
                     unsigned   *mins,
                     unsigned   *secs)
{
      unsigned hh, mm, ss;                /* Local data                 */

      if (3 != sscanf((char *)str, "%u:%u:%u", &hh, &mm, &ss))
            return Error_;
      if (hh > 23 || mm > 59 || ss > 59)
            return Error_;
      *hours = hh;
      *mins  = mm;
      *secs  = ss;
      return Success_;
}

#ifdef TEST

main(int argc, char *argv[])
{
      if (2 > argc)
      {
            puts("Usage: PARSTIME time_string [...time_string]");
            return EXIT_FAILURE;
      }
      while (--argc)
      {
            char *str;
            unsigned hh, mm, ss;
            Boolean_T retval;

            retval = parse_time(str = *++argv, &hh, &mm, &ss);
            printf("parse_time(\"%s\") returned %d\n", str, retval);
            if (Success_ == retval)
                  printf("  time = %02u:%02u:%02u\n\n", hh, mm, ss);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
