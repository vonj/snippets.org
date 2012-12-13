/* BIGNUM2.C
**
** Routines to do Big Number Arithmetic. These are multi-precision, unsigned
** natural numbers (0, 1, 2, ...). For the storage method, see the BigNum
** typedef in file BIGNUM.H
**
** Released to the public domain by Clifford Rhodes, June 15, 1995, with
** no guarantees of any sort as to accuracy or fitness of use.
*/

#include <stdlib.h>
#if defined(_MSC_VER)
 #include <memory.h>
#elif defined(__TURBOC__)
 #include <mem.h>
#else
 #include <string.h>
#endif

#include "bignum.h"    /* Typedef for BigNum type */

BigNum * BigNumAlloc(int nlen)
{
      /* Allocates memory for a BigNum object with nlen entries. Returns a
      ** pointer to the memory with the data array initialized to zero if
      ** successful. If not successful, NULL is returned.
      */

      BigNum * big;

      big = (BigNum *) malloc(sizeof(BigNum));
      if (big != NULL)
      {
            big->nlen = nlen;
            if (nlen < 1)
                  big->n = NULL;
            else if ((big->n =
                  (UShort *) calloc(nlen, sizeof(UShort))) == NULL)
            {
                  free(big);
                  return NULL;
            }
      }
      else  return NULL;

      return big;
}

void BigNumFree(BigNum * b)
{
      /* Frees memory used by BigNum object b. */

      if (b != NULL)
      {
            if (b->n != NULL)
                  free(b->n);
            free(b);
      }
}

BigNum * BigNumDiv(const BigNum * a, const BigNum * b, BigNum ** c)
{
      /* Big Number a is divided by Big Number b. If successful a pointer to
      ** the quotient is returned. The user must supply a pointer to a Big
      ** Number pointer, c, to receive the remainder.
      ** If unsuccessful, NULL is returned.
      ** Neither a nor b is changed by the call.
      */

      int      i, j, d, bpos;
      UShort   carry, quo;
      long     m1, m2, m3;
      BigNum * quotient, * atemp, * btemp;

      /* Find non-zero MSB of b, make sure b is not 0 */

      for (bpos = 0; bpos < b->nlen && b->n[bpos] == 0; bpos++)
            ;
      if (bpos == b->nlen)  /* Attempt to divide by zero! */
            return NULL;

      /* Create a copy of b, making sure btemp->n[0] != 0 */

      if ((btemp = BigNumAlloc(b->nlen - bpos)) == NULL)
            return NULL;
      memcpy(btemp->n, b->n + bpos, btemp->nlen * sizeof(UShort));

      /* Create a copy of a, making sure atemp->n[0] == 0 */

      carry = (a->n[0] == 0) ? 0 : 1;
      if ((atemp = BigNumAlloc(a->nlen + carry)) == NULL)
      {
            BigNumFree(btemp);
            return NULL;
      }
      memcpy(atemp->n + carry, a->n, a->nlen * sizeof(UShort));

      /* Allocate memory for quotient and remainder */

      if ((quotient = BigNumAlloc(max(1, atemp->nlen - btemp->nlen))) == NULL)
      {
            BigNumFree(atemp);
            BigNumFree(btemp);
            return NULL;
      }
      if ((*c = BigNumAlloc(btemp->nlen)) == NULL)
      {
            BigNumFree(atemp);
            BigNumFree(btemp);
            BigNumFree(quotient);
            return NULL;
      }
      d = MODULUS / (btemp->n[0] + 1);
      for (carry = 0, j = atemp->nlen - 1; j >= 0; j--)   
      {
            m1 = ((long) d * (long) *(atemp->n + j)) + (long) carry;
            *(atemp->n + j) = (UShort) (m1 % (long) MODULUS);
            carry = (UShort) (m1 / (long) MODULUS);
      }
      for (carry = 0, j = btemp->nlen - 1; j >= 0; j--)
      {
            m1 = ((long) d * (long) *(btemp->n + j)) + (long) carry;
            *(btemp->n + j) = (UShort) (m1 % (long) MODULUS);
            carry = (UShort) (m1 / (long) MODULUS);
      }
      for (j = 0; j < (atemp->nlen - btemp->nlen); j++)   
      {
            if (*btemp->n == *(atemp->n + j))
                  quo = MODULUS - 1;
            else
            {
                  m1 = (long) *(atemp->n + j) * (long) MODULUS;
                  m1 = (m1 + (long) *(atemp->n + j + 1)) / (long) *btemp->n;
                  quo = (UShort) m1;
            }
            m3 = (long) *(atemp->n + j) * (long) MODULUS +
                  (long) *(atemp->n + j + 1);
            do
            {
                  if (btemp->nlen > 1)
                        m1 = (long) quo * (long) *(btemp->n + 1);
                  else  m1 = 0l;
                  m2 = (long) quo * (long) *btemp->n;
                  m2 = (m3 - m2) * (long) MODULUS +
                        (long) *(atemp->n + j + 2);
                  if (m1 > m2)
                        quo--;
            } while (m1 > m2);

            bpos = btemp->nlen - 1;
            i = j + btemp->nlen;
            m2 = 0l;
            while (i >= j)
            {
                  if (bpos >= 0)
                        m1 = (long) quo * (long) *(btemp->n + bpos);
                  else  m1 = 0l;
                  m3 = (long) *(atemp->n + i) - m1 + m2;
                  if (m3 < 0l)
                  {
                        m2 = m3 / (long) MODULUS;
                        m3 %= (long) MODULUS;
                        if (m3 < 0l)
                        {
                              m3 += (long) MODULUS;
                              m2--;
                        }
                  }
                  else  m2 = 0l;
                  *(atemp->n + i) = (UShort) (m3);
                  bpos--;
                  i--;
            }
            if (m2 == 0l)
                  *(quotient->n + j) = quo;
            else
            {
                  *(quotient->n + j) = quo - 1;
                  bpos = btemp->nlen - 1;
                  i = j + btemp->nlen;
                  carry = 0;
                  while (i >= j)
                  {
                        long tmp;

                        if (bpos >= 0)
                              carry += *(btemp->n + bpos);
                        tmp = carry + (long) *(atemp->n + i);
                        if (tmp >= (long) MODULUS)
                        {
                              tmp -= (long) MODULUS;
                              carry = 1;
                        }
                        else  carry = 0;
                        *(atemp->n + i) = (UShort) tmp;
                        bpos--;
                        i--;
                  }
            }
      }
      j = atemp->nlen - btemp->nlen;    
      bpos = 0;
      carry = 0;
      while (j < atemp->nlen)            
      {
            m3 = (long) carry * (long) MODULUS + (long) *(atemp->n + j);
            *((*c)->n + bpos) = (UShort) (m3 / d);
            carry = (UShort) (m3 % d);
            j++;
            bpos++;
      }
      BigNumFree(atemp);   /* Free temporary memory */
      BigNumFree(btemp);

      return quotient;
}
