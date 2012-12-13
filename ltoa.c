/*
**  LTOA.C -- routine and example program to convert a long int to
**  the specified numeric base, from 2 to 36.
**
**  Written by Thad Smith III, Boulder, CO. USA  9/06/91 
**  and contributed to the Public Domain.
**
**  Parameters: 1 - number to be converted
**              2 - buffer in which to build the converted string
**              3 - number base to use for conversion (2-36)
**
**  Returns:  A character pointer to the converted string if
**            successful, a NULL pointer if the number base specified
**            is out of range.
*/

#include <stdlib.h>
#include "numcnvrt.h"

#if defined(__ZTC__) && !defined(__SC__)

char *ltoa(
      long val,                                 /* value to be converted */
      char *buf,                                /* output string         */
      int base)                                 /* conversion base       */
{
      ldiv_t r;                                 /* result of val / base  */

      if (base > 36 || base < 2)          /* no conversion if wrong base */
      {
            *buf = '\0';
            return buf;
      }
      if (val < 0)
            *buf++ = '-';
      r = ldiv (labs(val), base);

      /* output digits of val/base first */

      if (r.quot > 0)
            buf = ltoa ( r.quot, buf, base);

      /* output last digit */

      *buf++ = "0123456789abcdefghijklmnopqrstuvwxyz"[(int)r.rem];
      *buf   = '\0';
      return buf;
}

#endif

#ifdef TEST

#include <stdio.h>

main()
{
      char buf[100], line[100], *tail;
      long v;
      int inbase, outbase;

      for (;;)
      {
            printf ("inbase, value, outbase (or hit Ctrl-C to exit)? ");
            fgets (line, sizeof line, stdin);
            sscanf (line, " %d%*[, ]%[^, ]%*[, ]%d", &inbase, buf, &outbase);
            if (inbase == 0)
                  break;                        /* exit if first number 0 */
            v = strtol (buf, &tail, inbase);
            printf("%s(%d) = %ld(10)\n", buf, inbase, v);
            ltoa ( v, buf, outbase);
            printf("ltoa(%ld, buf, %d) = %s\n\n", v, outbase, buf);
            printf ("=%ld (10) = %s (%d).\n", v, buf, outbase);
      };
      return 0;
}

#endif /* TEST */
