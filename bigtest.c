/* BIGTEST.C
**
** Routines to test Big Number Arithmetic functions found in BIGNUM.C
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#if defined(_MSC_VER)
 #include <memory.h>
#elif defined(__TURBOC__)
 #include <mem.h>
#else
 #include <string.h>
#endif

#include "bignum.h"    /* Typedef for BigNum type */

ULong Big2Long(BigNum * b)
{
      /* Converts the big number in b to an unsigned long. This is a support
      ** routine for test purposes only. The length of b should be 3 or less
      ** to avoid overflowing the long.
      */

      int i;
      ULong res = 0L;

      for (i = 0; i < b->nlen; i++)
      {
            res *= (ULong) MODULUS;
            res += (ULong) b->n[i];
      }
      return res;
}

void BigNumRand(BigNum **b)
{
      /* This routine fills the big number pointed to by *b to a random long.
      ** It is a support routine for test purposes only.
      */

      int  i = (*b)->nlen - 1;

      memset((*b)->n, 0, (*b)->nlen * sizeof(UShort));

      if (i < 0)
            return;
      else if (i == 0)
            (*b)->n[0] = rand() % MODULUS;
      else if (i == 1)
      {
            (*b)->n[0] = rand() % MODULUS;
            (*b)->n[1] = rand() % MODULUS;
      }
      else
      {
            (*b)->n[i--] = rand() % MODULUS;
            (*b)->n[i--] = rand() % MODULUS;
            (*b)->n[i] = rand() % 21;
      }
}

int main(void)
{
      /* Test the big number arithmetic routines using random longs. Runs
      ** until an error is encountered or a key pressed.
      ** Gives examples of how to call the routines.
      */

      BigNum *a, *b, *c, *d = NULL, *p, *s;
      ULong al, bl, cl, dl, sl, cnt = 0L;

      /* 'Randomly' seed the rand() generator */

      srand((unsigned int) time(NULL));

      /* Create two big numbers */

      a = BigNumAlloc(3);
      b = BigNumAlloc(3);

      do    /* Loop until a key is pressed... */
      {
            if ((++cnt % 1000L) == 0)     /* Show every 1000 iterations */
                  printf("%ld\n", cnt);
            if (kbhit())            /* If a key pressed--we're outta here! */
            {
                  getch();
                  break;
            }

            /* Make the two big numbers random... */

            BigNumRand(&a);
            BigNumRand(&b);

            /* Get their 'long' values... */

            al = Big2Long(a);
            bl = Big2Long(b);

            c = BigNumDiv(a, b, &d);        /* Divide a by b */
            if (c == NULL)
            {
                  printf("Error: Divide did not return quotient\n");
                  break;
            }
            if (d == NULL)
            {
                  printf("Error: Divide did not return remainder\n");
                  break;
            }
            cl = Big2Long(c);        /* Get quotient as a long */
            dl = Big2Long(d);        /* Get remainder as a long */

            if ((al / bl) != cl)
            {
                  printf("Error: Quotient %lu / %lu != %lu\n", al, bl, cl);
                  printf("a = %d%04d%04d   b = %d%04d\n",
                        a->n[0], a->n[1], a->n[2], b->n[0], b->n[1]);
                  break;
            }
            if ((al % bl) != dl)
            {
                  printf("Error: Remainder %lu %c %lu != %lu\n",
                        al, '%', bl, dl);
                  printf("a = %d%04d%04d   b = %d%04d\n",
                        a->n[0], a->n[1], a->n[2], b->n[0], b->n[1]);
                  break;
            }

            /* Reconstruct BigNum a by multiplying the quotient by b and
            ** adding the remainder.
            */

            p = BigNumMul(c, b);
            s = BigNumAdd(p, d);
            sl = Big2Long(s);
            if (sl != al)
            {
                  printf("Error: Couldn't reconstruct original "
                        "divisor: %lu != %lu\n", al, sl);
                  break;
            }
            BigNumFree(s);    /* To use s again, must free it first! */
            s = BigNumSub(a, b);
            if (al >= bl)     /* For BigNumSub(), a must be > than b */
            {
                  sl = Big2Long(s);
                  if (sl != (al - bl))
                  {
                        printf("Error: Subtraction error %lu - %lu != %lu\n",
                              al, bl, sl);
                        break;
                  }
            }
            else if (s != NULL)
            {
                  printf("Invalid subtraction %lu - %lu\n", al, bl);
                  break;
            }

            /* Free all the memory allocated by the arithmetic calls */

            BigNumFree(c);
            BigNumFree(d);
            BigNumFree(p);
            BigNumFree(s);

      } while (1);

      BigNumFree(a);
      BigNumFree(b);

      return 0;
}
