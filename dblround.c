/*
**  DBLROUND.C - Rounds a double to the nearest whole number
**  public domain by Ross Cottrell
*/

#include <float.h>
#include <limits.h>
#include "snipmath.h"

#if defined(__ZTC__)
 #include <fltenv.h>
 #define SAVEROUND() \
       int fersave = fegetround(); \
       fesetround(FE_TONEAREST)
 #define RESTOREROUND() \
       fesetround(fersave)
#else
 #if !defined(FLT_ROUNDS) || (FLT_ROUNDS != 1)
  #error FLT_ROUNDS needs to be defined to be 1
 #endif
 #define SAVEROUND()
 #define RESTOREROUND()
#endif

double dround(double x)
{
      Boolean_T flag;
      static volatile double X;

      SAVEROUND();
      if (True_ == (flag = (x < 0.0)))
            X = -x;
      else  X = x;
      X += 1.0 / DBL_EPSILON;
      X -= 1.0 / DBL_EPSILON;
      RESTOREROUND();
      return ((flag) ? -X : X);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      double val;
      char *dummy;

      while (--argc)
      {
            val = strtod((const char *)(*(++argv)), &dummy);
            printf("dround(%g) = ", val);
            printf("%.12g\n", dround(val));
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
