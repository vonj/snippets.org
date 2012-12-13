/*
**  ADDTIME.C - Add a time period to a base time, normalizing the result
**
**  arguments: 1  - Base hours (0 -> 24)
**             2  - Base minutes (0 -> 59)
**             3  - Base seconds (0 -> 59)
**             4  - Span hours
**             5  - Span minutes
**             6  - Span seconds
**             7  - Address of returned hours (0 -> 23)
**             9  - Address of returned minutes (0 -> 59)
**             10 - Address of returned seconds (0 -> 59)
**             11 - Address of number of days to add to result
**
**  returns: 0 if no error, non-zero if base time range error
**
**  Notes: 1) Span values may be negative.
**         2) Overflowing midnight will cause a positive number of days to be
**            returned.
**         3) Underflowing midnight will cause a negative number of days to be
**            returned.
**
**  Original Copyright 1994 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <stdlib.h>
#include "datetime.h"

int add_time(unsigned basehrs, unsigned basemins, unsigned basesecs,
             int spanhrs, int spanmins, int spansecs,
             unsigned *hrs, unsigned *mins, unsigned *secs, int *days)
{
      int h, m, s;
      div_t r;

      if (basehrs > 24 || basemins > 59 || basesecs > 59)
            return -1;

      if (24 == basehrs)
            basehrs = 0;
      h = (int)basehrs  + spanhrs;
      m = (int)basemins + spanmins;
      s = (int)basesecs + spansecs;

      r = div(s, 60);
      if (s < 0)
      {
            r.rem += 60;
            --r.quot;
      }
      *secs = r.rem;
      m += r.quot;

      r = div(m, 60);
      if (m < 0)
      {
            r.rem += 60;
            --r.quot;
      }
      *mins = r.rem;
      h += r.quot;

      r = div(h, 24);
      if (h < 0)
      {
            r.rem += 24;
            --r.quot;
      }
      *hrs = r.rem;
      *days = r.quot;

      return 0;
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      unsigned bh, bm, bs, h, m, s;
      int sh, sm, ss, days;

      if (7 > argc)
      {
            puts("Usage: ADDTIME base_hrs base_mins base_secs "
                  "span_hrs span_mins span_secs");
            return EXIT_FAILURE;
      }

      bh = (unsigned)atoi(argv[1]);
      bm = (unsigned)atoi(argv[2]);
      bs = (unsigned)atoi(argv[3]);
      sh = atoi(argv[4]);
      sm = atoi(argv[5]);
      ss = atoi(argv[6]);

      printf("add_time() returned %d\n",
            add_time(bh, bm, bs, sh, sm, ss, &h, &m, &s, &days));

      printf("%2d:%02d:%02d + %2d:%02d:%02d = %2d:%02d:%02d + %d days\n",
            bh, bm, bs, sh, sm, ss, h, m, s, days);
      return EXIT_SUCCESS;
}

#endif /* TEST */
