/* jdn_l.c -- Julian Day Number computation
**
** public domain Julian Day Number functions
**
** Based on formulae originally posted by
**    Tom Van Flandern / Washington, DC / metares@well.sf.ca.us
**       in the UseNet newsgroup sci.astro.
**    Reposted 14 May 1991 in FidoNet C Echo conference by
**       Paul Schlyter (Stockholm)
** Minor corrections, added JDN to julian, and recast into C by
**    Raymond Gardner  Englewood, Colorado
**
** Synopsis:
**      long ymd_to_jdnl(int year, int month, int day, int julian_flag)
**      void jdnl_to_ymd(long jdnl, int *year, int *month, int *day,
**                                                      int julian_flag)
**      year is negative if BC
**      if julian_flag is >  0, use Julian calendar
**      if julian_flag is == 0, use Gregorian calendar
**      if julian_flag is <  0, routines decide based on date
**
** These routines convert Gregorian and Julian calendar dates to and
** from Julian Day Numbers.  Julian Day Numbers (JDN) are used by
** astronomers as a date/time measure independent of calendars and
** convenient for computing the elapsed time between dates.  The JDN
** for any date/time is the number of days (including fractional
** days) elapsed since noon, 1 Jan 4713 BC.  Julian Day Numbers were
** originated by Joseph Scaliger in 1582 and named after his father
** Julius, not after Julius Caesar.  They are not related to the
** Julian calendar.
**
** For dates from 1 Jan 4713 BC thru 12 Dec Feb 32766 AD, ymd_to_jdnl()
** will give the JDN for noon on that date.  jdnl_to_ymd() will compute
** the year, month, and day from the JDN.  Years BC are given (and
** returned) as negative numbers.  Note that there is no year 0 BC;
** the day before 1 Jan 1 AD is 31 Dec 1 BC.  Note also that 1 BC,
** 5 BC, etc. are leap years.
**
** Pope Gregory XIII decreed that the Julian calendar would end on
** 4 Oct 1582 AD and that the next day would be 15 Oct 1582 in the
** Gregorian Calendar.  The only other change is that centesimal
** years (years ending in 00) would no longer be leap years
** unless divisible by 400.  Britain and its possessions and
** colonies continued to use the Julian calendar up until 2 Sep
** 1752, when the next day became 14 Sep 1752 in the Gregorian
** Calendar.  These routines can be compiled to use either
** convention.  By default, the British convention will be used.
** Simply #define PAPAL to use Pope Gregory's convention.
**
** Each routine takes, as its last argument, a flag to indicate
** whether to use the Julian or Gregorian calendar convention.  If
** this flag is negative, the routines decide based on the date
** itself, using the changeover date described in the preceding
** paragraph.  If the flag is zero, Gregorian conventions will be used,
** and if the flag is positive, Julian conventions will be used.
**
** Proper JDN's are always floating point values so as to include the
** time as well as the date. These functions avoid the overhead of
** floating point math by computing only the integral value of the JDN.
*/

#include "datetime.h"

#ifdef PAPAL                    /* Pope Gregory XIII's decree */
#define LASTJULDATE 15821004L   /* last day to use Julian calendar */
#define LASTJULJDN  2299160L    /* jdn of same */
#else                           /* British-American usage */
#define LASTJULDATE 17520902L   /* last day to use Julian calendar */
#define LASTJULJDN  2361221L    /* jdn of same */
#endif


long ymd_to_jdnl(int y, int m, int d, int julian)
{
        long jdn;

        if (julian < 0)         /* set Julian flag if auto set */
                julian = (((y * 100L) + m) * 100 + d  <=  LASTJULDATE);

        if (y < 0)              /* adjust BC year */
                y++;

        if (julian)
                jdn = 367L * y - 7 * (y + 5001L + (m - 9) / 7) / 4
                + 275 * m / 9 + d + 1729777L;
        else
                jdn = (long)(d - 32076)
                + 1461L * (y + 4800L + (m - 14) / 12) / 4
                + 367 * (m - 2 - (m - 14) / 12 * 12) / 12
                - 3 * ((y + 4900L + (m - 14) / 12) / 100) / 4
                + 1;            /* correction by rdg */

        return jdn;
}


void jdnl_to_ymd(long jdn, int *yy, int *mm, int *dd, int julian)
{
        long x, z, m, d, y;
        long daysPer400Years = 146097L;
        long fudgedDaysPer4000Years = 1460970L + 31;

        if (julian < 0)                 /* set Julian flag if auto set */
                julian = (jdn <= LASTJULJDN);

        x = jdn + 68569L;
        if ( julian )
        {
                x += 38;
                daysPer400Years = 146100L;
                fudgedDaysPer4000Years = 1461000L + 1;
        }
        z = 4 * x / daysPer400Years;
        x = x - (daysPer400Years * z + 3) / 4;
        y = 4000 * (x + 1) / fudgedDaysPer4000Years;
        x = x - 1461 * y / 4 + 31;
        m = 80 * x / 2447;
        d = x - 2447 * m / 80;
        x = m / 11;
        m = m + 2 - 12 * x;
        y = 100 * (z - 49) + y + x;

        *yy = (int)y;
        *mm = (int)m;
        *dd = (int)d;

        if (*yy <= 0)                   /* adjust BC years */
                (*yy)--;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
      int day, yr, mo;
      char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
      long jdn;

      if (3 > argc)
      {
            puts("Usage: JDN d m y");
            puts("where: d = day (1 - 31)");
            puts("       m = month (1 - 12)");
            puts("       y = year (1 - 99, 1800 - 3000)");
            return -1;
      }

      yr  = atoi(argv[3]);
      mo  = atoi(argv[2]);
      day = atoi(argv[1]);

      if (mo < 1 || 12 < mo)
            return -1;

      if (day < 1 || 31 < day)
            return -1;

      if (100 > yr)
            yr += 1900;

      printf("User specified %d-%s-%d...\n\n", day, months[mo - 1], yr);

      jdn = ymd_to_jdnl(yr, mo, day, 0);
      jdnl_to_ymd(jdn, &yr, &mo, &day, 0);

      printf("jdn_l(%d-%s-%d) = %ld\n", day, months[mo - 1], yr, jdn);
      return 0;
}

#endif /* TEST */
