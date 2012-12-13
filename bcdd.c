/*FILE*****************************************************************/
/* File Id:                    bcd.c.                                 */
/* Author:                     Stan Milam.                            */
/* Date Written:               31-Jan-95.                             */
/* Description:                                                       */
/*     Routines to manage binary coded decimal.                       */
/*                                                                    */
/*     bcd_to_double() - Convert BCD to type double value.            */
/*     double_to_bcd() - Convert double type value to BCD.            */
/*                                                                    */
/* Placed in the public domain by the author, 8-Sep-95                */
/*                                                                    */
/*****************************************************************FILE*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "snipmath.h"

/*FUNCTION*************************************************************/
/* Name: bcd_to_double().                                             */
/*                                                                    */
/* Description:                                                       */
/*     This function will convert buffer containing binary coded dec- */
/*     imal to a double.                                              */
/*                                                                    */
/* Arguments:                                                         */
/*     void     *buf  - Address of buffer containing binary coded     */
/*                      decimal number.                               */
/*     size_t   buflen- Length of the buffer.                         */
/*     int      digits- The number of digits to the right of the      */
/*                      decimal point.                                */
/*                                                                    */
/* Return Value:                                                      */
/*     A value of type double which should be the equivalent of the   */
/*     BCD value.                                                     */
/*                                                                    */
/**********************************************************************/

double bcd_to_double(void *buf, size_t len, int digits)
{
      double   rv = 0.0;
      char     *buffer = (char *) buf;
      size_t   high, low, index, max = len - 1;

      digits = abs(digits);

    /******************************************************************/
    /* Loop through the buffer repeatedly extracting the high and low */
    /* 4 bits from each byte and calculating the return value.        */
    /******************************************************************/

      for (index = 0; index < max; index++)
      {
            low  = buffer[index] & 0x0f;
            high = (buffer[index] & 0xf0) >> 4;
            rv = (( rv * 10.0 + high ) * 10.0 + low);
      }

    /******************************************************************/
    /* The first byte of the buffer contains the lowest order digit   */
    /* in the upper 4 bits and the sign in the lower 4 bits.          */
    /******************************************************************/

      low  = buffer[max] & 0x0f;
      high = (buffer[max] & 0xf0) >> 4;
      rv = rv * 10.0 + high;
      if (digits > 0)
            rv /= pow(10, digits);
      if (low == 0x0d)
            rv = -rv;
      return rv;
}

/*FUNCTION*************************************************************/
/* Name: double_to_bcd().                                             */
/*                                                                    */
/* Description:                                                       */
/*     This function will convert a value of type double to a legit-  */
/*     mate Binary Coded Decimal (BCD) value.                         */
/*                                                                    */
/* Arguments:                                                         */
/*     double   arg    - The value to be converted.                   */
/*     char     *buf   - The buffer where the BCD value is stored.    */
/*     size_t   length - The number of significant digits to store in */
/*                       BCD buffer.                                  */
/*     size_t   digits - The number of digits to the right of the     */
/*                       decimal point to be stored in the BCD        */
/*                       buffer.                                      */
/*                                                                    */
/* Return Value:                                                      */
/*     An integer value indicating the length of the BCD value in the */
/*     buffer.  -1 is returned if an error occured.                   */
/*                                                                    */
/*************************************************************FUNCTION*/

int double_to_bcd(double arg, char *buf, size_t length, size_t digits )
{
      char wrkbuf[50], format[DBL_DIG + 1];
      int  y_sub, x_sub, rv, negative=0;

    /******************************************************************/
    /* Do a couple of sanity checks first.                            */
    /******************************************************************/

      if ((length == 0 && digits == 0) || (length + digits > DBL_DIG))
            rv = -1;
      else
      {
        /**************************************************************/
        /* If the double argument is negative make a note of it and   */
        /* con- vert the value to be positive.                        */
        /**************************************************************/

            if (arg < 0.0)
            {
                  arg = -arg;
                  negative = 1;
            }

        /**************************************************************/
        /* Adjust for decimal digits.                                 */
        /**************************************************************/

            if (digits > 0)
            {
                  length += digits;
                  arg *= pow( 10, digits );
            }

        /**************************************************************/
        /* Build the format string, build the string and compute the  */
        /* return value.                                              */
        /**************************************************************/

            sprintf( format, "%%0%d.0f", length );
            sprintf( wrkbuf, format, floor( arg ) );
            if ((rv = (length / 2 ) + (length / 2 != 0)) == 0)
                  rv = 1;

        /**************************************************************/
        /* Compute the subscript values and clear the BCD buffer.     */
        /**************************************************************/

            y_sub = rv - 1;
            x_sub = strlen( wrkbuf ) - 1;
            memset( buf, 0, y_sub + 1 );

        /**************************************************************/
        /* Plug in the sign bits and first BCD digit.                 */
        /**************************************************************/

            buf[y_sub] = negative == 1 ? 0xd : 0xc;
            buf[y_sub--] |= ( ( wrkbuf[x_sub--] - '0' ) << 4 );

        /**************************************************************/
        /* While we have more digits to plug....                      */
        /**************************************************************/

            while ( --length > 0 )
            {
        
            /**********************************************************/
            /* Do the low nibble of the BCD byte.                     */
            /**********************************************************/

                  buf[y_sub] = wrkbuf[x_sub--] - '0';
                  if (--length <= 0)
                        break;

            /**********************************************************/
            /* Now do the high nibble.                                */
            /**********************************************************/

                  buf[y_sub--] |= ((wrkbuf[x_sub--] - '0') << 4);
            }
      }
      return rv;
}
/**/
#ifdef TEST

typedef struct {
    double value, expect;
    int    length, digits;
} TEST_T;

int main(void)
{ 
      double rv, value;
      char   wrkbf[25];
      int    rc, x_sub, y_sub, size, len, digits;
      char   format[] = "    %10.3f      %d          %d        %d      ";
      TEST_T testvals[] = {
            {  12345.67,     123.45,   5, 0 },
            {  12345.67,   12345.0,   5, 1 },
            {  12345.67,   12345.67,  4, 3 },
            {  12345.678,   2345.67,  1, 2 },
            { -12345.00,  -12345.00,  8, 2 },
            { -1234.56,  -12345.00,   5, 3 },
            {  1234.567,      0.60,   1, 3 }
      };

      size = sizeof( testvals ) / sizeof( TEST_T );
      printf("    Double       Length     Digits    Return\n");
      printf("    Argument     Argument   Argument  Value    Buffer\n");
      printf("    ================================================="
             "=========\n");

      for (x_sub = 0; x_sub < size; x_sub++)
      {
            len = testvals[x_sub].length;
            digits = testvals[x_sub].digits;
            value  = testvals[x_sub].value;

            rc = double_to_bcd(value, wrkbf, len, digits);
            if (rc > 0)
            {
                  printf( format, value, len, digits, rc );
                  for ( y_sub = 0; y_sub < rc; y_sub++ )
                        printf("%02X ", wrkbf[y_sub]);
                  printf("\n");
            }
      }
      return 0;
}
#endif
