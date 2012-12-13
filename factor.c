/*
** factor.c -- print prime factorization of a number
** Ray Gardner -- 1985 -- public domain
** Modified Feb. 1989 by Thad Smith > public domain
**
** This version takes numbers up to the limits of double precision.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int prevfact = 0;
void factor (double);
void show (double, int);

main (int argc, char *argv[])
{
      while ( --argc )
            factor(atof(*++argv));
      return 0;
}

void factor (double n)
{
      double d;
      int k;

      prevfact = 0;

      d = n+1;     /* test for roundoff error */
      if (n+3 != d+2)
      {
            printf("%0.0f is too large to process.\n", n);
            return;
      }
      if (fmod(n,1.) != 0.0)
      {
            printf("%f is not an integer.\n",n);
            return;
      }
      printf("%0.0f  ",n);
      if ( n < 2. )
      {
            printf("is less than 2.\n");
            return;
      }
      else if ( n > 2. )
      {
            d = 2;
            for ( k = 0; fmod(n,d) == 0.0; k++ )
                  n /= d;
            if ( k )
                  show(d,k);
            for ( d = 3; d * d <= n; d += 2 )
            {
                  for ( k = 0; fmod(n,d) == 0.0; k++ )
                        n /= d;
                  if ( k )
                        show(d,k);
            }
      }
      if ( n > 1 )
      {
            if ( ! prevfact )
                  printf(" is prime");
            else  show(n,1);
      }
      printf("\n");
}

void show (double d, int k)
{
      if ( prevfact )
            printf(" * ");
      else  printf(" = ");
      prevfact++;
      printf("%0.0f",d);
      if ( k > 1 )
            printf("^%d",k);
}
