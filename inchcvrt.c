/*
**  Convert English measurement units
**
**  Takes command line arguments in inches and converts to:
**
**  1. feet and inches (expressed as floating point)
**  2. feet and inches (expressed as fraction)
**
**  public domain demo by Bob Stout
**  uses ROUND.H from SNIPPETS
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "round.h"

#define BASE 64.0

void cnvrt_inches(double  input,
                  double *feet,
                  double *inches,
                  double *dec_inches,
                  double *num_inches,
                  double *den_inches)
{
      /*
      **  Split feet and inches
      */

      *feet   = floor(input / 12.0);
      *inches = fmod(input, 12.0);

      /*
      **  Get integer inches and fractions
      */

      *num_inches = modf(*inches, dec_inches) * BASE;

      *num_inches = fround(*num_inches, 0);
      if (0.0 == *num_inches)
            return;

      /*
      **  Reduce fractions to lowest common denominator
      */

      for (*den_inches = BASE;
            0.0 == fmod(*num_inches, 2.0);
            *den_inches /= 2.0, *num_inches /= 2.0)
      {
            ;
      }
}

main(int argc, char *argv[])
{
      double arg, feet, inches, dec, num, den, dummy;

      while (--argc)
      {
            arg = atof(*(++argv));
            cnvrt_inches(arg, &feet, &inches, &dec, &num, &den);
            printf("%f Inches = %d' %.5f\" or %d' %d",
                  arg, (int)feet, inches, (int)feet, (int)dec);
            if (0.0 == num)
                  puts("\"");
            else
            {
                  printf("-%d/%d\"", (int)num, (int)den);
                  if (modf(num, &dummy))
                        puts(" (approx.)");
                  else  puts("");
            }
      }
      return EXIT_SUCCESS;
}
