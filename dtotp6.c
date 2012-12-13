/*
$Header: c:/bnxl/rcs/dtotp6.c 1.2 1995/05/01 00:30:58 bnelson Exp $

$Log: dtotp6.c $
Revision 1.2  1995/05/01 00:30:58  bnelson

- Added test driver and Thad Smith's original function
  to convert a TP real to double

- Checks out OK with lint.

- Tested on GNU C 2.6.3 (little endian Intel) after adding PAK to
  real struct.

Revision 1.1  1995/04/30 23:54:56  bnelson

Written by Bob Nelson of Dallas, TX, USA (bnelson@netcom.com)

Original tp6_to_double() written by Thad Smith III of Boulder, CO, and
  released to the public domain in SNIPPETS

- Initial release -- converts C double value into the bit pattern used
  by a Turbo Pascal 6-byte real. Uses the "real" struct written by Thad
  Smith for ease of assignment to members.

- Tested on BC++ 3.1. 

- This source and associated include are contributed to the Public Domain.
*/


#include <math.h>
#include "dtotp6.h"


#define DBL_BIAS            0x3FE
#define REAL_BIAS           0x80
#define TP_REAL_BIAS        (DBL_BIAS - REAL_BIAS)    /* 0x37E */


tp_real_t double_to_tp6(double x)
{
      unsigned int *wp;
      tp_real_t r;

      if(x == 0.0)
      {
            r.v3 = r.v2 = r.v1 = r.be = r.s = 0;
            return r;
      }

      wp = (void *)&x;            /* Break down double into words */

      r.s  = wp[3] >> 15;         /* High bit set for sign */

      /* -------------------------------------------------------------------
      ** Grab biased exponent -- exclude sign and shift out the MSB
      ** mantissa bits.
      */

      r.be = (unsigned char)(((wp[3] & 0x7FFF) >> 4) - TP_REAL_BIAS);

      /* ------------------------------------------------------------------
      ** Now...just assign the mantissa after shifting the bits to conform
      ** with the layout for the TP 6-byte real.
      */

      r.v3 = ((wp[3] & 0x0F) << 3) | (wp[2] >> 13);
      r.v2 = (wp[2] << 3) | (wp[1] >> 13);
      r.v1 = (wp[1] << 3) | (wp[0] >> 13);

      return r;
}


/* -----------------------------------------------------------------
** Slightly adapted version of Thad Smith's function from TP6TOD.C
** from Snippets. (Uses TP real struct parameter and no memcpy).
*/


double tp6_to_double(tp_real_t r)
{
      if (r.be == 0)
            return 0.0;

      return ((((128 + r.v3) * 65536.0) + r.v2) * 65536.0 + r.v1) *
            ldexp((r.s ? -1.0 : 1.0), r.be - (129 + 39));
}

#if defined (TEST)

#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
      double x, y;
      tp_real_t r;

      if(argc > 1)
            x = strtod(argv[1], NULL);
      else  x = 19999.99;

      r = double_to_tp6(x);
      y = tp6_to_double(r);

      printf("input double value: %.2f, converted double value: %.2f\n",
            x, y);

      return 0;
}

#endif    /* TEST */
