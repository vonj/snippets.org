/* BIGNUM1.C
**
** Routines to do Big Number Arithmetic. These are multi-precision, unsigned
** natural numbers (0, 1, 2, ...). For the storage method, see the BigNum
** typedef in file BIGNUM.H
**
** Released to the public domain by Clifford Rhodes, June 15, 1995, with
** no guarantees of any sort as to accuracy or fitness of use.
*/

#include <stdlib.h>
#include "bignum.h"    /* Typedef for BigNum type */


BigNum * BigNumAdd(const BigNum * a, const BigNum * b)
{
      /* Big Numbers a and b are added. If successful a pointer to the sum is
      ** returned. If unsuccessful, a NULL is returned.
      ** Neither a nor b is changed by the call.
      */

      UShort  carry = 0;
      int     size, la, lb;
      long    tsum;
      BigNum  * sum;

      /* Allocate memory for Big Number sum to be returned... */

      size = max(a->nlen, b->nlen) + 1;
      if ((sum = BigNumAlloc(size)) == NULL)
            return NULL;

      /* Get indexes to last digits in each number (Least significant) */

      la = a->nlen - 1;
      lb = b->nlen - 1;
      size--;

      while (la >= 0 && lb >= 0)    /* While both a and b have data */
      {
            tsum = carry + (long) *(a->n + la) + (long) *(b->n + lb);
            *(sum->n + size) = (UShort) (tsum % (long) MODULUS);
            carry = (tsum / (long) MODULUS) ? 1 : 0;
            la--;
            lb--;
            size--;
      }
      if (lb < 0)                   /* Must see if a still has data */
      {
            while (carry && la >= 0)
            {
                  tsum = carry + (long) *(a->n + la);
                  *(sum->n + size) = (UShort) (tsum % (long) MODULUS);
                  carry = (tsum / (long) MODULUS) ? 1 : 0;
                  la--;
                  size--;
            }
            while (la >= 0)
            {
                  *(sum->n + size) = *(a->n + la);
                  la--;
                  size--;
            }
      }
      else                          /* See if b still has data */
      {
            while (carry && lb >= 0)
            {
                  tsum = carry + (long) *(b->n + lb);
                  *(sum->n + size) = (UShort) (tsum % (long) MODULUS);
                  carry = (tsum / (long) MODULUS) ? 1 : 0;
                  lb--;
                  size--;
            }
            while (lb >= 0)
            {
                  *(sum->n + size) = *(b->n + lb);
                  lb--;
                  size--;
            }
      }
      *(sum->n + size) = carry;

      return sum;
}

BigNum * BigNumSub(const BigNum * a, const BigNum * b)
{
      /* Big Numbers a and b are subtracted. a must be >= to b. A pointer to
      ** the difference (a - b) is returned if successful. If unsuccessful,
      ** a NULL is returned.
      ** Neither a nor b is changed by the call.
      */

      int      size, la, lb, borrow = 0;
      long     tdiff;
      BigNum * diff;

      /* Allocate memory for Big Number difference to be returned... */

      if ((diff = BigNumAlloc(a->nlen)) == NULL)
            return NULL;

      la = a->nlen - 1;
      size = la;
      lb = b->nlen - 1;

      while (lb >= 0)
      {
            tdiff = (long) *(a->n + la) - (long) *(b->n + lb) - borrow;
            if (tdiff < 0)
            {
                  tdiff += (long) (MODULUS - 1);
                  borrow = 1;
            }
            else  borrow = 0;
            *(diff->n + size) = (UShort) tdiff + borrow;
            la--;
            lb--;
            size--;
      }
      while (la >= 0)          /* Must get rest of a->n */
      {
            tdiff = (long) *(a->n + la) - borrow;
            if (tdiff < 0)
            {
                  tdiff += (long) (MODULUS - 1);
                  borrow = 1;
            }
            else  borrow = 0;
            *(diff->n + size) = (UShort) tdiff + borrow;
            la--;
            size--;
      }
      if (borrow)   /* We've got an underflow here! */
      {
            BigNumFree(diff);
            return NULL;
      }
      return diff;
}

BigNum * BigNumMul(const BigNum * a, const BigNum * b)
{
      /* Big Numbers a and b are multiplied. A pointer to the product
      ** is returned if successful. If unsuccessful, a NULL is returned.
      ** Neither a nor b is changed by the call.
      */

      int      size, la, lb, apos, bpos, ppos;
      UShort   carry;
      BigNum * product;

      /* Allocate memory for Big Number product to be returned... */

      size = a->nlen + b->nlen;
      if ((product = BigNumAlloc(size)) == NULL)
            return NULL;

      la = a->nlen - 1;
      lb = b->nlen - 1;
      size--;

      bpos = lb;

      while (bpos >= 0)             /* We'll multiply by each digit in b */
      {
            ppos = size + (bpos - lb);    /* Position in product */

            if (*(b->n + bpos) == 0) /* If zero multiplier, skip this pass */
                  ppos = ppos - la - 1;
            else                    /* Multiply a by next b digit */
            {
                  apos = la;
                  carry = 0;
                  while (apos >= 0) /* Go a digit at a time through a */
                  {
                        ULong temp;

                        temp = (ULong) *(a->n + apos) *
                              (ULong) *(b->n + bpos) + carry;

                        /*
                        ** We must add any previous product term in
                        ** this 'column' too
                        */

                        temp += (ULong) *(product->n + ppos);

                        /* Now update product term, remembering any carry */

                        *(product->n + ppos) =
                              (UShort) (temp % (ULong) MODULUS);
                        carry = (UShort) (temp / (ULong) MODULUS);

                        apos--;
                        ppos--;
                  }
                  *(product->n + ppos) = carry;
            }
            bpos--;
      }
      return product;
}
