/*
**  FACTORYL.C
**
**  Original Copyright 1992 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Uses DBLROUND.C, also in SNIPPETS
*/

#include <float.h>
#include "snipmath.h"

#define dfrac(x) ((x)-dround(x))

#define SQRT2PI sqrt(2 * PI)
#define ONESIXTH (1.0/6.0)

/*
**  log10factorial()
**
**  Returns the logarithm (base 10) of the factorial of a given number.
**  The logarithm is returned since this allows working with extremely
**  large values which would otherwise overflow the F.P. range.
**
**  Parameters: 1 - Number whose factorial to return.
**
**  Returns: log10() of the passed value, -1.0 if error
**
**  Limitations: Cannot return 0! since log(0) is undefined.
*/

double log10factorial(double N)
{
      double dummy;

      if ((N < 1.0) || (0.0 != modf(N, &dummy)))
            return -1.0;
      if (N < 40)                         /* Small, explicitly compute  */
      {
            int i;
            double f;

            if (0.0 == N)
                  return N;
            for (i = 1, f = 1.0; i <= (int)N; ++i)
                  f *= i;
            return log10(f);
      }
      else                                /* Large, use approximation   */
      {
            return log10(SQRT2PI)+((N + 0.5) *
                  (log10(sqrt(N * N + N + ONESIXTH) / exp(1.0))));
      }
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      double f, lf;
      char *dummy;

      while (--argc)
      {
            f  = strtod((const char *)(*(++argv)), &dummy);
            if (0.0 == f)
            {
                  puts("0! = 0");
                  continue;
            }
            if (-1.0 == (lf = log10factorial(f)))
            {
                  printf(">>> ERROR: %g! is not a valid expression\n", f);
                  continue;
            }
            if (171.0 > f)
                  printf("%.14g! = %.14g\n", f, pow(10.0, lf));
            else
            {
                  printf("%.14g! = %.14ge+%ld\n", f,
                        pow(10.0, dfrac(lf)), (long)dround(lf));
            }
      }
      lf = log10C(1000000L,750000L);
      printf("\nJust to dazzle with you with big numbers:\n"
            "C(1000000,750000) = %.14ge+%ld\n",
            pow(10.0, dfrac(lf)), (long)dround(lf));
      lf = log10P(1000000L,750000L);
      printf("\n...once more:\n"
            "P(1000000,750000) = %.14ge+%ld\n",
            pow(10.0, dfrac(lf)), (long)dround(lf));
      return EXIT_SUCCESS;
}

#endif /* TEST */
