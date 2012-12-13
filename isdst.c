/*
**  ISDST.C - Determine is a date falls within Daylight Savings Time.
**
**  The rule is that DST starts at 2:00 AM on the first Sunday of April
**  and ends at 2:00 AM on the last Sunday of October.
**
**  Original Copyright 1995 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include "sniptype.h"
#include "scaldate.h"

/*
**  Encode DST rules here - defaults to standard U.S. rules.
*/

unsigned    DST_start_mo = 4;             /* Month when DST starts      */
unsigned    DST_start_dt = 1;             /* Date when it can start     */
enum DOW_T  DST_start_dy = SUNDAY;        /* Day of week, or DOW_IGNORE */

unsigned    DST_stop_mo = 10;             /* Month when DST stops       */
unsigned    DST_stop_dt = 31;             /* Date when it can stop      */
enum DOW_T  DST_stop_dy = SUNDAY;         /* Day of week, or DOW_IGNORE */

/*
**  isDST()
**
**  Parameters: 1 - Year of interest.
**              2 - Month to check.
**              3 - Day to check.
**              4 - Pointer to storage for scalar date representation of
**                  the year's DST start date.
**              5 - Pointer to storage for scalar date representation of
**                  the year's DST stop date.
**
**  Returns: True_ if date is in DST range
**           False_ if date is not in DST  range
**           Error_ if date is invalid,
*/

Boolean_T isDST(unsigned  yr,
                unsigned  mo,
                unsigned  dy,
                long     *Start,
                long     *Stop)
{
      long date;

      if (!valiDate(yr, mo, dy))
            return Error_;
      else  date = ymd_to_scalar(yr, mo, dy);

      *Start = ymd_to_scalar(yr, DST_start_mo, DST_start_dt);
      *Stop  = ymd_to_scalar(yr, DST_stop_mo, DST_stop_dt);

      if (DST_start_dy != DOW_IGNORE)
      {
            while (DST_start_dy != (*Start % 7L))
                  ++*Start;
      }

      if (DST_stop_dy != DOW_IGNORE)
      {
            while (DST_stop_dy != (*Stop % 7L))
                  --*Stop;
      }

      return (date >= *Start && date < *Stop);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include "datetime.h"

main(int argc, char *argv[])
{
      long Start, Stop;
      unsigned yr, mo, dy;
      Boolean_T retval;

      if (2 > argc)
      {
            puts("Usage: ISDST \"date_string\"");
            return EXIT_FAILURE;
      }
      if (Success_ != parse_date(argv[1], &yr, &mo, &dy, USA))
      {
            printf("Error parsing date \"%s\"\n", argv[1]);
            return EXIT_FAILURE;
      }

      if (Error_ == (retval = isDST(yr, mo, dy, &Start, &Stop)))
            printf("%d/%d,%d is invalid!\n", mo, dy, yr);
      else
      {
            printf("%d/%d/%d is%s within DST range\n\n", mo, dy, yr,
                  retval ? "" : " not");
            scalar_to_ymd(Start, &yr, &mo, &dy);
            printf("In %d, DST starts on %d/%d\n", yr, mo, dy);
            scalar_to_ymd(Stop, &yr, &mo, &dy);
            printf("In %d, DST stops on %d/%d\n", yr, mo, dy);
      }

      return EXIT_SUCCESS;
}

#endif /* TEST */
