/*
**  TODAY.C - Return today's date in a scalar date format.
**
**  public domain by Bob Stout - uses Ray Gardner's SCALDATE.C
*/

#include <time.h>
#include "scaldate.h"


long today(void)
{
      time_t tnow;
      struct tm *tmnow;

      time(&tnow);
      tmnow = localtime(&tnow);
      return ymd_to_scalar(tmnow->tm_year + 1900, tmnow->tm_mon + 1,
            tmnow->tm_mday);
}

#ifdef TEST

#include <stdio.h>

main()
{
      unsigned yr, mo, dy;
      long dnow = today();

      scalar_to_ymd(dnow, &yr, &mo, &dy);
      printf("Today is %d/%d/%d\n", mo, dy, yr);
      return 0;
}

#endif /*TEST */
