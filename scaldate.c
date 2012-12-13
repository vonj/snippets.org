/*
** scalar date routines    --    public domain by Ray Gardner
** Numerically, these will work over the range 1/01/01 thru 14699/12/31.
** Practically, these only work from the beginning of the Gregorian 
** calendar thru 14699/12/31.  The Gregorian calendar took effect in
** much of Europe in about 1582, some parts of Germany in about 1700, in
** England and the colonies in about 1752ff, and in Russia in 1918.
*/

#include "scaldate.h"

int isleap (unsigned yr)
{
   return yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0);
}

static unsigned months_to_days (unsigned month)
{
   return (month * 3057 - 3007) / 100;
}

static long years_to_days (unsigned yr)
{
   return yr * 365L + yr / 4 - yr / 100 + yr / 400;
}

long ymd_to_scalar (unsigned yr, unsigned mo, unsigned day)
{
   long scalar;
   scalar = day + months_to_days(mo);
   if ( mo > 2 )                         /* adjust if past February */
      scalar -= isleap(yr) ? 1 : 2;
   yr--;
   scalar += years_to_days(yr);
   return scalar;
}

void scalar_to_ymd (long scalar, unsigned *yr, unsigned *mo, unsigned *day)
{
   unsigned n;                /* compute inverse of years_to_days() */

   for ( n = (unsigned)((scalar * 400L) / 146097L); years_to_days(n) < scalar;)
      n++;                          /* 146097 == years_to_days(400) */
   *yr = n;
   n = (unsigned)(scalar - years_to_days(n-1));
   if ( n > 59 ) {                       /* adjust if past February */
      n += 2;
      if ( isleap(*yr) )
         n -= n > 62 ? 1 : 2;
   }
   *mo = (n * 100 + 3007) / 3057;    /* inverse of months_to_days() */
   *day = n - months_to_days(*mo);
}
