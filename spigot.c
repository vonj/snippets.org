/*
 This program was based on a Pascal program posted in the FIDO 80XXX
 assembly conference.  That Pascal program had the following comment:

                    ------------------------
 This program, which produces the first 1000 digits of PI, using
 only integer arithmetic, comes from an article in the "American
 Mathematical Monthly", Volume 102, Number 3, March 1995, page
 195, by Stanley Rabinowitz and Stan Wagon.
                    ------------------------

 My C implementation is placed into the public domain, by its author
 Carey Bloodworth, on August 22, 1996.

 I have not seen the original article, only that Pascal implementation,
 but based on a discussion in the 80XXX conference, I believe the
 program should be accurate to at least 32 million digits when using
 long unsigned integers.  Using only 16 bit integers causes the
 variables to overflow after a few hundred digits.
*/

#include <stdio.h>

long unsigned int i, j, k, nines, predigit;
long unsigned int q, x, numdig, len;
long unsigned int *pi;

int printed = 0, line = 1;

void OutDig(int dig)
{
      putchar(dig);
      printed++;
      if ((printed%50) == 0)
      {
            printed = 0;
            printf("\nL%04d:   ", line++);
      }
      else if ((printed%10) == 0)putchar(' ');
}

int main(int argc, char *argv[])
{
      if (argc < 2)
      {
            printf("I need to know how many digits to compute\n");
            return 1;
      }

      numdig = atol(argv[1]);
      len = (numdig*10)/3;

      pi = (long unsigned int *)malloc((len+1)*sizeof(long unsigned int) );
      if (pi == NULL)
      {
            printf("Unable to allocate memory\n");
            return 1;
      }

      for (x = len; x > 0; x--)
            pi[x] = 2;

      printf("L0001: 3.");

      nines = 0;
      predigit = 0;
      for (j = 0; j <= numdig; j++)
      {
            q = 0;
            for (i = len; i > 0; i--)
            {
                  x = 10 * pi[i]+ q * i;
                  pi[i] = x % (2 * i - 1);
                  q = x / (2 * i - 1);
            }
            pi[1] = q % 10; q = q / 10;
            if (q == 9)
                  nines++;
            else if (q == 10)
            {
                  OutDig('1' + predigit);
                  while (nines)
                  {
                        OutDig('0');
                        nines--;
                  }
                  predigit = 0;
                  fflush(stdout);
            }
            else
            {
                  if (j > 1)
                        OutDig('0' + predigit);
                  while (nines)
                  {
                        OutDig('9');
                        nines--;
                  }
                  predigit = q;
                  fflush(stdout);
            }
      }
      OutDig(predigit + '0');

      free(pi);
      return 0;
}
