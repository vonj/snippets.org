/*
  pi8.c  Sept 9, 1996.

  This program is placed into the public domain by its author,
  Carey Bloodworth.

  This pi program can calculate pi with either integers, or doubles.
  It can also use a variety of formulas.

  This code isn't really optimized because it has to work with both the
  long integer version and the double version, and several formulas.
  Compromises had to be made in several places.

  When compiling, you can chose to use the FPU or the integer version.
  By default, it will chose to work only in integers.  If you want to
  use the FPU, define:

#define USEFPU     1

  You have a choice of formulas.  By default, it will use the Machin
  formula of:  pi=16arctan(1/5)-4arctan(1/239)

  You could chose to use one of the other formulas.

  for pi=8arctan(1/3)+4arctan(1/7)
#define ARC3  1
  for pi=12arctan(1/4)+4arctan(1/20)+4arctan(1/1985)
#define ARC4  1
  for pi=16arctan(1/5)-4arctan(1/70)+4arctan(1/99)
#define ARC5  1
  for pi=32arctan(1/10)-4arctan(1/239)-16arctan(1/515)
#define ARC10 1

  Or, of course, you could define it on the compile command line with
  the -D switch.

  Timings were done on a Cyrix 486/66, with the slow Turbo C++ v3.0
          1,000 2,000 3,000 4,000  1,000F 2,000F 3,000F 4,000F
  Machin      4    18    45    86      1      5     11     20
  Arc3        6    29    74   140      2      9     20     35
  Arc4        5    24    64   116      2      7     16     29
  Arc5        6    26    65   123      1      6     15     26
  Arc10       4    19    46    86      1      5     11     19

  All of the combinations above were verified to their indicated
  precision and in each case, only the last few digits were wrong,
  which is a normal round off / truncation error.

  Better compilers will of course result in faster computations,
  but the ratios should be the same.  When I used GCC 2.6.3 for
  DOS, I computed 4,000 digits with with the Machin formula and
  the FPU in 8 seconds.  The integer version took 17 seconds.

  I also used the FPU GCC version to compute 65,536 digits of
  pi and verified them against the Gutenberg PIMIL10.TXT, and
  only the last 4 digits were incorrect.  The computations took
  33 minutes and 54 seconds.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHOWTIME
#define USEFPU

#if defined USEFPU

#define BASE   1000000000L
#define BASEDIGITS 9

typedef long int SHORT;
typedef double   LONG;
#else

#define BASE   100
#define BASEDIGITS 2

typedef unsigned char SHORT;
typedef long int      LONG;
#endif

typedef long int INDEXER;


SHORT *pi, *powers, *term;
INDEXER size;


void OutDig(int dig)
{
      static int printed = 0;

      putchar(dig + '0');
      printed++;
      if ((printed%1000) == 0)
      {
            printed = 0;
            printf("\n\n\n");
      }
      if ((printed%50) == 0)
            printf("\n");
      else if ((printed%10) == 0)
            putchar(' ');
}


void PrintShort(SHORT num)
{
      int x;
      int digits[BASEDIGITS + 1];

      for (x = 0; x < BASEDIGITS; x++)
      {
            digits[x] = num % 10;
            num /= 10;
      }
      for (x = BASEDIGITS - 1; x >= 0; x--)
            OutDig(digits[x]);
}

void Print(SHORT *num)
{
      INDEXER x;

      printf("\nPI = 3.\n");
      for (x = 1; x < size; x++)
            PrintShort(num[x]);
      printf("\n");
}

void arctan(int multiplier, int denom, int sign)
{
      INDEXER x;
      LONG remain, temp, divisor, denom2;
      SHORT NotZero = 1;
      INDEXER adv;

      for (x = 0; x < size; x++)
            powers[x] = 0;

      divisor = 1;
      denom2 = (LONG)denom;denom2 *= denom2;
      adv = 0;

      remain = (LONG)multiplier * denom;
      while (NotZero)
      {
            for (x = adv; x < size; x++)
            {
                  temp = (LONG)powers[x] + remain;
                  powers[x] = (SHORT)(temp / denom2);
                  remain = (temp - (denom2 * (LONG)powers[x])) * BASE;
            }

            remain = 0;
            for (x = adv; x < size; x++)
            {
                  temp = (LONG)powers[x] + remain;
                  term[x] = (SHORT)(temp / divisor);
                  remain = (temp - (divisor * (LONG)term[x])) * BASE;
            }
            remain = 0;

            if (sign > 0)
            {
                  LONG carry, sum;

                  carry = 0;
                  for (x = size - 1; x >=0; x--)
                  {
                        sum = (LONG)pi[x] + (LONG)term[x] + carry;
                        carry = 0;
                        if (sum >= BASE)
                        {
                              carry = 1;
                              sum -= BASE;
                        }
                        pi[x] = (SHORT)sum;
                  }
            }
            else
            {
                  LONG borrow, sum;

                  borrow = 0;
                  for (x = size - 1; x >= 0; x--)
                  {
                        sum = (LONG)pi[x] - (LONG)term[x] - borrow;
                        borrow = 0;
                        if (sum < 0)
                        {
                              borrow = 1;
                              sum += BASE;
                        }
                        pi[x] = (SHORT)sum;
                  }
            }

            sign = -sign;
            divisor += 2;
            NotZero = 0;
            for (x = adv; x < size; x++)
            {
                  if (powers[x])
                  {
                        NotZero = 1;
                        break;
                  }
            }

            if (NotZero)
            {
                  while (powers[adv] == 0)
                        adv++;
            }
            /* We can skip ones that are already 0 */
      }
}

int main(int argc, char *argv[])
{
      INDEXER x;
      time_t T1, T2;

      if (argc != 2)
      {
            printf("I need to know how many digits to compute.\n");
            exit(EXIT_FAILURE);
      }

      size = atol(argv[1]);
      if (size <= 0L)
      {
            printf("Invalid argument.\n");
            exit(EXIT_FAILURE);
      }

      size = ((size + BASEDIGITS - 1) / BASEDIGITS) + 1;

      pi = malloc(sizeof(SHORT) * size);
      powers = malloc(sizeof(SHORT) * size);
      term = malloc(sizeof(SHORT) * size);

      if ((pi == NULL) || (powers == NULL) || (term == NULL))
      {
            printf("Unable to allocate enough memory.\n");
            exit(EXIT_FAILURE);
      }

      for (x = 0; x < size; x++)
            pi[x] = 0;

      T1 = time(NULL);

#if defined ARC3
      arctan(8, 3, 1);
      arctan(4, 7, 1);
#elif defined ARC5
      arctan(16, 5, 1);
      arctan(4, 70, -1);
      arctan(4, 99, 1);
#elif defined  ARC4
      arctan(12, 4, 1);
      arctan(4, 20, 1);
      arctan(4, 1985, 1);
#elif defined  ARC10
      arctan(32, 10, 1);
      arctan(4, 239, -1);
      arctan(16, 515, -1);
#else
      /* Machin formula */
      arctan(16, 5, 1);
      arctan(4, 239, -1);
#endif

      T2 = time(NULL);

      Print(pi);

#ifdef SHOWTIME
      printf("\nCalculation time %0.0lf\n", difftime(T2, T1));
#endif

      return EXIT_SUCCESS;
}
