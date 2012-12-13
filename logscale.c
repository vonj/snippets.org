/*
**  L O G S C A L E
**
**  Logarithmically scale from long integer to long integer. The
**  function domain (input) & range (output) both start from 0
**  and the range MUST be smaller than the domain.
**
**  This scaling function was especially designed to colour fractals.
**  In order to preserve detail & to ensure that no colours are wasted,
**  the function is almost linear with a slope near 1 for early values.
**
**  Written by M. Stapleton of Graphic Bits
**
**  Public domain
**
**  May 18 1997
*/

#include "snipmath.h"


/* "Shifted" logarithm = log(u+v) - log(v) */

#define shlog(u,v) (log(((double)(u) + (double)(v)) / (double)(v)))

static double alpha;            /* Scaling parameter */

/*
**  Calculate scaling parameter (alpha) using Newton's method,
**  where rmax = alpha * shlog(dmax, alpha)
**  Boolean return: 0 on failure
*/

int initlogscale(long dmax, long rmax)
{
      double dm = (double) dmax;
      double rm = (double) rmax;
      double x = 1.0, dx, y, yy, t;

      if (dm <= rm)
            return 0;

      do
      {
            t = shlog(dm, x);
            y = x * t - rm;
            yy = t - dm / (x + dm);
            dx = y / yy;
            x -= dx;
            /* printf("Alpha = %f\n", x); */
      } while (abs(dx) > 1E-4);

      alpha = x;

      return 1;
}

/*
**  Logarithmically scale d, offset by alpha
*/

long logscale(long d)
{
      double r = alpha * shlog(d, alpha);

      return (long) floor(0.5 + r);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

/*
**  Default domain & range maxima.
*/

#define DMAX 1000
#define RMAX 255

/*
**  Test logscale
*/

int main(int argc, char *argv[])
{
      long d, dmax = 0, r0, r1, rmax = 0;

      if (argc == 2 && *argv[1] == '?')
      {
            printf("Test logscale()\n Usage:\n");
            printf("%s [domain] [range]\n", argv[0]);
            return EXIT_FAILURE;
      }

      /*
      **  Get domain & range maxima
      */

      if (argc > 1)
            dmax = atol(argv[1]);
      if (dmax == 0)
            dmax = DMAX;

      if (argc > 2)
            rmax = atol(argv[2]);
      if (rmax == 0)
            rmax = RMAX;

      if (rmax >= dmax)
      {
            printf("Warning: range must be smaller than domain!\n");

            /*
            **  A real program we would exit here, but we go on to test
            **  initlogscale's error handling...
            */
      }

      /*
      ** Initialise scaling parameter
      */

      if (!initlogscale(dmax, rmax))
      {
            printf("Error: cannot initialise logscale!\n");
            return EXIT_FAILURE;
      }

      /*
      **  Calculate function for every domain value.
      ** Only print where the range value changes.
      */

      for (r0 = -1, d = 0; d < dmax; d++)
      {
            r1 = logscale(d);
            if (r1 != r0)
            {
                  printf("%ld -> %ld\n", d, r1);
                  r0 = r1;
            }
      }
      printf("%ld -> %ld\n", d, r1);

      return EXIT_SUCCESS;
}

#endif /* TEST */
