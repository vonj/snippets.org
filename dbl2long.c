/*
**  DBL2LONG.C - Functions to round doubles to longs
**  Public domain by Ross G. Cottrell, June 1992
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

/* Assume IEEE doubles, little-endian CPU, 32-bit 2's complement longs. */
/* (Actually, the assumptions made here aren't quite that gross.)       */

unsigned long dbl2ulong(double t)
{
      SAVEROUND();
      t += 1.0 / DBL_EPSILON;
      RESTOREROUND();
      return *(unsigned long *)&t;
}

long dbl2long(double t)
{
      SAVEROUND();
      t += 1.0 / DBL_EPSILON + 2.0 * (LONG_MAX + 1.0);
      RESTOREROUND();
      return *(long *)&t;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

int main(int argc, char **argv)
{
      while (*++argv)
      {
            printf("'%s', as a long: %ld, as an unsigned long: %lu\n",
                  *argv, dbl2long(atof(*argv)), dbl2ulong(atof(*argv)));
      }
      return 0;
}

#endif /* TEST */

/*

EXPLANATION:

The offset of 1.0/DBL_EPSILON forces the least significant bit of the
mantissa to represent the integer 1.  This may not work on all formats of
doubles, but I think it's a safe bet for IEEE compliant doubles, and any
other floating point format with a radix of 2.  When this offset is added,
the number should be rounded to the nearest representable value.  If the
compiler does not support extended math funtionality, a compile-time error is
generated if FLT_ROUNDS isn't set to 1, i.e. if not rounding to the nearest
representable value.  The addition of 2.0*(LONG_MAX+1.0) for the signed long
is to prevent the MSB of the mantissa being borrowed for negative inputs - if
this happened, the exponent would change and the LSB of the mantissa would no
longer be worth 1.  This offset would be perfectly okay to use with the
unsigned longs too but it's unnecessary for them, unless you want to get the
answer correct modulo 2^^32 for negatives.

*/
