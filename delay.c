/*
**  DELAY.C - A portable time delay compatible with Borland's and Watcom's
**            delay() function.
**
**  public domain demo by Bob Stout
*/

#if (!defined(__WATCOMC__) && !defined(__TURBOC__)) || (defined(__TURBOC__) \
      && (defined(_Windows) && !defined(__DPMI16) && !defined(__DPMI32__)))

#include <time.h>
#include "delay.h"

#ifndef  CLOCKS_PER_SEC                   /* CLOCKS_PER_SEC is ANSI/ISO */
 #define CLOCKS_PER_SEC CLK_TCK
#endif

void delay(unsigned short msec)
{
      clock_t t0;
      unsigned long diff = 0L;

      for (t0 = clock(); diff < (unsigned long)msec; )
      {
            diff  = (unsigned long)(clock() - t0);
            diff *= 1000L;
            diff /= CLOCKS_PER_SEC;
      }
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      int msec;

      if (2 > argc)
      {
            puts("Usage: DELAY milliseconds");
            return EXIT_FAILURE;
      }
      msec = atoi(argv[1]);
      printf("Delaying %d milliseconds\n", msec);
      delay(msec);
      return EXIT_SUCCESS;
}

#endif /* TEST */

#endif /* Not Watcom or Borland */
