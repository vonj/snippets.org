/*
**  FRAND.C - Public domain by Larry Hudson
*/

#include <math.h>
#include <time.h>
#include "snipmath.h"

#define TEN_PI 31.41592653589793
#define E      2.718281828459045

/*--------------------------------------------------------------+
|           Return random double between 0.0 and 1.0            |
|                                                               |
|    If n is negative it will randomize the seed, based on the  |
|        current MSDOS time.                                    |
|    If n is zero it will return the next random number.        |
|    If n is positive it will set the seed to a value based on  |
|        the value of n.                                        |
+--------------------------------------------------------------*/

double frandom(int n)
{
      static double seed = E;
      double dummy;
      time_t tim;

      if (n < 0)
      {
            time(&tim);
            seed = (double)tim;
      }
      else if (n > 0)
            seed = (double)n * E;

      seed = modf(seed * TEN_PI + E, &dummy);
      return seed;
}
