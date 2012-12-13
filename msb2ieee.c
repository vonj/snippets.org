/***  MSBIN conversion routines    ***/
/***  public domain by Jeffery Foy ***/

#include "snipmath.h"

union Converter {
      unsigned char uc[10];
      unsigned int  ui[5];
      unsigned long ul[2];
      float          f[2];
      double         d[1];
};

/* MSBINToIEEE - Converts an MSBIN floating point number */
/*               to IEEE floating point format           */
/*                                                       */
/*  Input: f - floating point number in MSBIN format     */
/* Output: Same number in IEEE format                    */

float MSBINToIEEE(float f)
{
      union Converter t;
      int sign, exp;       /* sign and exponent */

      t.f[0] = f;

      /* extract the sign & move exponent bias from 0x81 to 0x7f */

      sign = t.uc[2] / 0x80;
      exp  = (t.uc[3] - 0x81 + 0x7f) & 0xff;

      /* reassemble them in IEEE 4 byte real number format */

      t.ui[1] = (t.ui[1] & 0x7f) | (exp << 7) | (sign << 15);
      return t.f[0];
} /* End of MSBINToIEEE */


/* IEEEToMSBIN - Converts an IEEE floating point number  */
/*               to MSBIN floating point format          */
/*                                                       */
/*  Input: f - floating point number in IEEE format      */
/* Output: Same number in MSBIN format                   */

float IEEEToMSBIN(float f)
{
      union Converter t;
      int sign, exp;       /* sign and exponent */

      t.f[0] = f;

      /* extract sign & change exponent bias from 0x7f to 0x81 */

      sign = t.uc[3] / 0x80;
      exp  = ((t.ui[1] >> 7) - 0x7f + 0x81) & 0xff;

      /* reassemble them in MSBIN format */

      t.ui[1] = (t.ui[1] & 0x7f) | (sign << 7) | (exp << 8);
      return t.f[0];
} /* End of IEEEToMSBIN */
