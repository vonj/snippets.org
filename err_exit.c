/*
**  ERR_EXIT.C - A generic fatal error-handler by Dave Schaumann
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

void ErrExit(char *fmt, ...)
{
      va_list ap;

      va_start(ap, fmt) ;
      vfprintf(stderr, fmt, ap);
      fputc('\n', stderr);
      exit(EXIT_FAILURE);
}

#ifdef TEST

main()
{
      int x = 1, y = -1;

      if (x != y)
            ErrExit("Found x = %d, y = %d; Expected them to be equal!", x, y);
      return EXIT_SUCCESS;
}

#endif /* TEST */
