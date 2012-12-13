/*
**  RND_DIV.C - Rounded integer division
**
**  Public domain - suggested by Dave Hansen in comp.lang.c
*/

#include <stdlib.h>

int round_div(int n, int d)
{
      div_t res = div(n,d);
      div_t rnd = div(res.rem, (abs(d)+1)/2 );

      return res.quot + rnd.quot;
}

long round_ldiv(long n, long d)
{
      ldiv_t res = ldiv(n,d);
      ldiv_t rnd = ldiv(res.rem, (abs(d)+1)/2 );

      return res.quot + rnd.quot;
}

#ifdef TEST

#include <stdio.h>
#include <limits.h>

main(int argc, char *argv[])
{
      long n, d, q;

      if (argc != 3)
      {
            puts("Usage: RND_DIV n d\n");
            puts("Returns n/d rounded to nearest integer");
            return -1;
      }

      n = atol(argv[1]);
      d = atol(argv[2]);

      if (n > INT_MAX || d > INT_MAX)
      {
            q = round_ldiv(n, d);
            printf("round_ldiv(%ld, %ld) = %ld\n", n, d, q);
      }
      else
      {
            q = (long)round_div((int)n, (int)d);
            printf("round_div(%ld, %ld) = %ld\n", n, d, q);
      }

      return 0;
}

#endif /* TEST */
