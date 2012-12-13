/*
**  DAYNUM.C - Functions to return statistics about a given date.
**
**  public domain by Bob Stout - uses Ray Gardner's SCALDATE.C
*/

#include "scaldate.h"

static long jan1date;

/*
**  Determine if a given date is valid
*/

Boolean_T valiDate(unsigned yr, unsigned mo, unsigned day)
{
      unsigned int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

      if (1 > mo || 12 < mo)
            return False_;
      if (1 > day || day > (days[mo - 1] + (2 == mo && isleap(yr))))
            return False_;
      else  return True_;
}

/*
**  Return the day of the week
*/

unsigned dow(unsigned yr, unsigned mo, unsigned day)
{
 
#if (!ISO_CAL)    /* Sunday(0) -> Saturday(6) (i.e. U.S.) calendars  */
      return (unsigned)(ymd_to_scalar(yr, mo, day) % 7L);
#else             /* International Monday(0) -> Sunday(6) calendars  */
      return (unsigned)((ymd_to_scalar(yr, mo, day) - 1L) % 7L);
#endif
}

/*
**  Return the day of the year (1 - 365/6)
*/

int daynum(int year, int month, int day)
{
      jan1date = ymd_to_scalar(year, 1, 1);
      return (int)(ymd_to_scalar(year, month, day) - jan1date + 1L);
}

/*
**  Return the week of the year (1 - 52, 0 - 52 if ISO)
*/

int weeknum(int year, int month, int day)
{
      int wn, j1n, dn = daynum(year, month, day);

      dn += (j1n = (int)((jan1date - (long)ISO_CAL) % 7L)) - 1;
      wn = dn / 7;
      if (ISO_CAL)
            wn += (j1n < 4);
      else  ++wn;
      return wn;
}

/*
**  Return the phase of the moon (0 -> 7, 0 = new, 4 = full)
*/

char *MoonPhaseText[8] = {"new", "waxing crescent", "first quarter",
      "waxing gibbous", "full", "waning gibbous", "third quarter",
      "waning crescent"
      };

unsigned moonphase(unsigned yr, unsigned mo, unsigned dy)
{
      unsigned long date = (unsigned long)ymd_to_scalar(yr, mo, dy);

      date *= 9693L;
      date /= 35780L;
      date -= 4L;
      date %= 8L;
      return (unsigned)date;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

void do_err(void);

main(int argc, char *argv[])
{
      int day, month, year;
      char *days[] =
#if (!ISO_CAL)
            {"Sunday", "Monday", "Tuesday", "Wednesday",
             "Thursday", "Friday", "Saturday"};
#else
            {"Monday", "Tuesday", "Wednesday", "Thursday",
             "Friday", "Saturday", "Sunday"};
#endif

      if (4 > argc)
      {
            puts("Usage: DAYNUM month day year");
            return EXIT_FAILURE;
      }

      month = atoi(argv[1]);
      day   = atoi(argv[2]);
      year  = atoi(argv[3]);
      if (100 > year)
            year += 1900;

      if (!valiDate(year, month, day))
            printf("%d/%d/%d is invalid!\n", month, day, year);
      else
      {
            printf("%d/%d/%d is a %s, day #%d in week %d\n", month, day, year,
                  days[dow(year, month, day)], daynum(year, month, day),
                  weeknum(year, month, day));
            printf("The phase of the moon is %s\n",
                  MoonPhaseText[moonphase(year, month, day)]);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
