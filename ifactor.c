/*
** ifactor.c -- print prime factorization of a number
**
** Ray Gardner -- 1985 -- public domain
*/

#include <stdio.h>
#include <stdlib.h>

int prevfact = 0;
void factor (long);
void show (long, int);

main (int argc, char *argv[])
{
      while ( --argc )
            factor(atol(*++argv));
      return 0;
}

void factor (long n)
{
      long d;
      int k;
      long n0 = n;
      prevfact = 0;

      printf("%ld  ",n);
      if ( n < 2 )
      {
            printf("is less than 2.\n");
            return;
      }
      else if ( n > 2 )
      {
            d = 2;
            for ( k = 0; n % d == 0; k++ )
                  n /= d;
            if ( k )
                  show(d,k);
            for ( d = 3; d * d <= n; d += 2 )
            { 
                  for ( k = 0; n % d == 0; k++ )
                        n /= d;
                  if ( k )
                        show(d,k);
            }
      }
      if ( n > 1 )
      {
            if ( n == n0 )
                  printf(" is prime");
            else  show(n,1);
      }
      printf("\n");
}

void show (long d, int k)
{
      if ( prevfact )
            printf(" * ");
      else  printf(" = ");
      prevfact++;
      printf("%ld",d);
      if ( k > 1 )
            printf("^%d",k);
}
