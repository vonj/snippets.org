/*
**  FRACTION.C - Compute continued fraction series
**
**  cfrac() donated to the public domain by the author, Thad Smith
**  original Fraction.C, public domain by Bob Stout, modified to use cfrac()
*/

#include <math.h>
#include <float.h>

#define MAX_LENGTH 100

long double cfrac(long double x, long double *p, long double *q, int bits)
{
      double v;                                 /* integer in series    */
      long double del;                          /* x - v                */
      long double z;    /* approximated value from truncated series     */
      long double t;                            /* temp                 */
      long double p0 = 0.0, q0 = 0.0;           /* last p, q            */
      long double imax;                         /* max for p, q         */
      static long double cf[MAX_LENGTH]; /* continued fraction integers */
      int i, j, ntimes = MAX_LENGTH;;

      if (x < 0)
            x = -x;
      imax = floor(pow(2.0, bits)) - 1.0;
      for (i = 0; i < ntimes; i++)
      {
            v = floor((double)x);
            cf[i] = v;
            z = cf[i];
            *p = z; *q = 1;
            for (j = i; j--; )
            {
                  z = cf[j] + 1.0/z;
                  t = *p;
                  *p = cf[j] * (*p) + (*q);
                  *q = t;
            }
            del = x-v;
            if (del < DBL_EPSILON)
                  break;
            if ((*p > imax) || (*q > imax))
            {
                  *p = p0;
                  *q = q0;
                  break;
            }
            else
            {
                  p0 = *p;
                  q0 = *q;
            }
            x = 1.0 / del;
      }
      return (*p)/(*q);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main (int argc, char *argv[])
{
      long double x;      /* value to be approximated */
      long double r,p,q;  /* approx ratio r = p/q     */
      int bits;           /* bits of precision        */

      if (argc < 2 || argc > 3)
      {
            puts ("Use: FRACTION value [precision]");
            puts ("where value = floating point value to generate "
                  "continued fraction");
            puts ("      precision (optional) = bits in "
                  "numerator/denominator");
            return 1;
      }
      sscanf (argv[1], "%Lf", &x);
      if (argc == 3)
            bits = atoi(argv[2]);
      else  bits = 32;

      cfrac(x, &p, &q, bits);
      printf("\n[%.20Lf]\n%.0Lf/%.0Lf = %lXh/%lXh = %.20Lf\n",
            x, p, q, (long)p, (long)q, r = p/q);
      printf("Error = %.10Lg, (%.10Lf%%)\n", r - x, 100. * (r - x) / x);
      return EXIT_SUCCESS;
}

#endif /* TEST */
