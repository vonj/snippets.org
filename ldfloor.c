/* ldfloor() -- long double floor
** public domain by Raymond Gardner  Englewood, CO
** tested with TC++
** assumptions: 80-bit IEEE format numbers, stored LSB first 
**   (Intel style), longs & ints are accessed from arbitrary boundaries
*/

#include "snipmath.h"

long double ldfloor(long double a)
{
    long double a0;
    int e, n;
   
    a0 = a;
    e = ((int *)&a)[4] & 0x7FFF;        /* extract exponent         */
    if ( e == 0 )                       /* 0 is special case        */
        return (long double) 0.0;
    e -= 16383;                         /* unbias exponent          */
    if (e < 0)                          /* if < 0, num is < 1,...   */
    {
        a = 0.0;                        /* so floor is zero         */
    }
    else if ((n = 63 - e) > 0)          /* clear n least sig. bits  */
    {
        if (n < 32)                     /* clear n lowest bits      */
        {
            ((unsigned long *)&a)[0] &= ~((1L << n) - 1);
        }
        else                            /* n >= 32 */
        {
            ((unsigned long *)&a)[0] = 0; /* clear lower 32 bits            */
            n -= 32;                    /* how many left to clear ?         */
            if (n)                      /* if any, clear n next lowest bits */
            {
                ((unsigned long *)&a)[1] &= ~((1L << n) - 1);
            }
        }
    }
    if (a0 < 0 && a0 != a)          /* if neg. and it had fractional bits */
        a -= 1.0;                   /* adjust the floor                   */
    return a;                       /* return it                          */
}
