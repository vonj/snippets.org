/*
**  PARITY.C - Computes even or odd parity for various integral types
**
**  public domain demo by Bob Stout
*/

#include "parity.h"

unsigned parity32(unsigned long x, Parity_T even)
{
      x = x ^ (x >> 16);
      x = x ^ (x >> 8);
      x = x ^ (x >> 4);
      x = x ^ (x >> 2);
      x = x ^ (x >> 1);

      return ((unsigned)(x & 1)) ^ even;
}

unsigned parity16(unsigned short x, Parity_T even)
{
      x = x ^ (x >> 8);
      x = x ^ (x >> 4);
      x = x ^ (x >> 2);
      x = x ^ (x >> 1);

      return ((unsigned)(x & 1)) ^ even;
}

unsigned parity8(unsigned char x, Parity_T even)
{
      x = x ^ (x >> 4);
      x = x ^ (x >> 2);
      x = x ^ (x >> 1);

      return ((unsigned)(x & 1)) ^ even;
}

unsigned parity64(void *x, Parity_T even)
{
      union longlong *val64 = (union longlong *)x;

      return (parity32(val64->lo, even) ^ parity32(val64->hi, even));
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      while (--argc)
      {
            unsigned long n = strtoul(*(++argv), NULL, 10);

            printf("Even parity of %ld = %d\n", n, parity32(n, Even_));
            printf("Odd parity of  %ld = %d\n\n", n, parity32(n, Odd_));
      }
}

#endif
