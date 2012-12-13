/* PD by Michelangelo Jones, 1:1/124. */

/*
**  Returns 0 for new moon, 15 for full moon,
**  29 for the day before new, and so forth.
*/

/*
**  This routine sometimes gets "off" by a few days,
**  but is self-correcting.
*/

#include "datetime.h"

int moon_age(int month, int day, int year)
{
      static short int ages[] =
            {18, 0, 11, 22, 3, 14, 25, 6, 17,
             28, 9, 20, 1, 12, 23, 4, 15, 26, 7};
      static short int offsets[] =
            {-1, 1, 0, 1, 2, 3, 4, 5, 7, 7, 9, 9};

      if (day == 31)
            day = 1;
      return ((ages[(year + 1) % 19] + ((day + offsets[month-1]) % 30) +
            (year < 1900)) % 30);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

static char *description[] = {
      "new",                  /* totally dark                         */
      "waxing crescent",      /* increasing to full & quarter light   */
      "in its first quarter", /* increasing to full & half light      */
      "waxing gibbous",       /* increasing to full & > than half     */
      "full",                 /* fully lighted                        */
      "waning gibbous",       /* decreasing from full & > than half   */
      "in its last quarter",  /* decreasing from full & half light    */
      "waning crescent"       /* decreasing from full & quarter light */
      };

static char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int main(int argc, char *argv[])
{
      int month, day, year, phase;

      if (4 > argc)
      {
            puts("Usage: MOON_AGE month day year");
            return EXIT_FAILURE;
      }
      month = atoi(argv[1]);
      day   = atoi(argv[2]);
      year  = atoi(argv[3]);
      if (100 > year)
            year += 1900;
      printf("moon_age(%d, %d, %d) returned %d\n", month, day, year,
            phase = moon_age(month, day, year));
      printf("Moon phase on %d %s %d is %s\n", day, months[month - 1], year,
            description[(int)((phase + 2) * 16L / 59L)]);
      return EXIT_SUCCESS;
}

#endif /* TEST */
