/*
**  ETPHI.c
**
**  Compute Euler's Totient function, phi().
**  phi(n) = the number of positive integers less than n
**  which have no common factors with n
**
**  Written by M. Stapleton of Graphic Bits
**  Public Domain
**
**  Sep  7 1996
**
**  Uses ISQRT.C, also in SNIPPETS
*/

#include "snipmath.h"

#undef ULONG
typedef unsigned long ULONG;

ULONG intsqrt(ULONG n);

static void dofact(ULONG i);
ULONG etphi(ULONG n);

static ULONG num, ph;

void dofact(ULONG i)
{
      long p;

      for (p = 0; (num % i) == 0; p++)
            num /= i;

      if (p)
      {
            ph *= i - 1;
            while(--p)
                  ph *= i;
      }
}

ULONG etphi(ULONG n)
{
      ULONG i;
      struct int_sqrt mi;

      if (n<2)
            return 0;

      num = n;
      ph = 1;
      usqrt(n, &mi);

      dofact(2);
      dofact(3);
      for (i = 5; (i <= mi.sqrt) && (num >= i); i += 6)
      {
            dofact(i);
            if (num < i + 2)
                  break;
            dofact(i + 2);
      }

      if (num > 1)
            ph *= num - 1;

      return ph;
}

#ifdef TEST

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
      long n = 0;

      if (argc > 1)
            n = atol(argv[1]);

      printf("etphi(%ld) = %ld\n", n, etphi(n));

      return 0;
}

#endif
