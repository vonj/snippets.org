/*
$Header: c:/bnxl/rcs/dtotp6.h 1.1 1995/05/01 00:04:08 bnelson Exp $

$Log: dtotp6.h $
Revision 1.1  1995/05/01 00:04:08  bnelson

- Include file companion to dtotp6.c, see notes contained there.
*/


#include "dirport.h"


#ifndef D2TOTP6_H_
#define     D2TOTP6_H_

#ifdef __TURBOC__
#pragma     option -a-       /* Force byte alignment in struct */
#endif

#ifdef __GNUC__
#define PAK        __attribute__((packed))
#else
#define PAK
#endif

#ifdef MONOSPACE_6           /* Just to be safe... */
 #define     double_to_tp6    DBL2TP
 #define     tp6_to_double    TP2DBL
#endif

typedef struct {
    unsigned char be   PAK;     /* biased exponent */
    unsigned int  v1   PAK;     /* lower 16 bits of mantissa */
    unsigned int  v2   PAK;     /* next  16 bits of mantissa */
    unsigned int  v3:7 PAK;     /* upper  7 bits of mantissa */
    unsigned int  s :1 PAK;     /* sign bit */
} tp_real_t;

extern tp_real_t  double_to_tp6(double x);
extern double     tp6_to_double(tp_real_t r);

#endif    /* D2TOTP6_H_ */
