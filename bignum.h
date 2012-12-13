/* BIGNUM.H
**
** Header file with definition of BigNum type for Big Number Arithmetic.
**
** Released to the public domain by Clifford Rhodes on June 15, 1995 with
** no guarantees of any sort as to accuracy or fitness of use.
*/

/* Each int in the Big Number array will hold a digit up to MODULUS - 1.
** Choosing 10000 makes testing easy because each digit contains 4 decimal
** places.
*/

#ifndef BIGNUM__H
#define BIGNUM__H

#include "minmax.h"

#define MODULUS  10000    /* Warning: Must be <= USHRT_MAX! */

typedef unsigned short UShort;
typedef unsigned long  ULong;

/* The Big Number is contained in a structure that has a length, nlen, and
** an array, n[], of unsigned shorts to hold the 'digits'. The most
** significant digit of the big number is at n[0]. The least significant
** digit is at n[nlen - 1];
*/

typedef struct {
      int nlen;      /* Number of int's in n */
      UShort *n;     /* Array of unsigned shorts to hold the number */
} BigNum;

/* Arithmetic function prototypes */
BigNum * BigNumAdd(const BigNum *a, const BigNum *b);
BigNum * BigNumSub(const BigNum *a, const BigNum *b);
BigNum * BigNumMul(const BigNum *a, const BigNum *b);
BigNum * BigNumDiv(const BigNum *a, const BigNum *b, BigNum **c);
BigNum * BigNumAlloc(int nlen);
void     BigNumFree(BigNum *b);

#endif /* BIGNUM__H */
