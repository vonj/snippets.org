/*
**  IPOW.C - Raise a number to an integer power
**
**  public domain by Mark Stephen with suggestions by Keiichi Nakasato
*/

#include "snipmath.h"

double ipow(double x, int n)        /* return x^n */
{
      double t = 1.0;

      if (!n)
            return 1.0;    /* At the top. 0**0 = 1 */
      if (n < 0)
      {
            n = -n;
            x = 1.0/x;  /* error if x == 0. Good                        */
      }                 /* ZTC/SC returns inf, which is even better     */
      if (x == 0.0)
            return 0.0;
      do
      {
            if (n & 1)
                  t *= x;
            n /= 2;     /* KN prefers if (n/=2) x*=x; This avoids an    */
            x *= x;     /* unnecessary but benign multiplication on     */
      } while (n);      /* the last pass, but the comparison is always  */
      return t;         /* true _except_ on the last pass.              */
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      double d;
      int n;

      d = atof(argv[1]);
      n = atoi(argv[2]);
      printf("%f^%d = %f\n", d, n, ipow(d, n) );
      return 0;
}

#endif /* TEST */
