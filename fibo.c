/*
    This program demonstrates how to calculate any arbitrary term
    of the Fibonacci sequence using phi (the golden number) and
    eighteenth century mathematician A. de Moivre's formula.

    written on Fri  04-05-1996  by Ed Beroset
        and released to the public domain by the author

    modified for SNIPPETS 01-Sep-1996 by Bob Stout
*/

#include "snipmath.h"                           /* includes math.h      */

double fibo(unsigned short term)
{
      const double k = 1/sqrt(5.0);             /* a handy constant     */
      double x;                                 /* the Fibonacci term   */

      /*
      **  this is de Moivre's formula using phi, the golden number (defined
      **  in snipmath.h), and as simplified by Donald Knuth
      */

      x = k * pow(PHI, term);

      /*
      **  this is to compensate for computer error
      */

      return dround(x);
}

#ifdef TEST

#include <stdio.h>

int main()
{
      unsigned i;                               /* which term?          */

      for (i = 1; i < 84; i++)                  /* limit of precision   */
            printf("%2d: %20.0f\n", i, fibo(i));
      return 0;
}

#endif /* TEST */
