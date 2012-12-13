/*
**  Miscellaneous text spinners demonstration
**
**  public domain by Jon Guthrie, Bob Stout, and others
*/

#include <stdio.h>

#define DURATION 500    /* Length of demo       */
#define SSLOWDOWN 15    /* Make spinner look ok */
#define TSLOWDOWN 20    /* Make target look ok  */

main()
{
      unsigned i;
      char spinner[] = "|/-\\", target[] = ".oO";

      for (i = 0; i < DURATION; ++i)
      {
            unsigned scount = i / SSLOWDOWN, tcount = i / TSLOWDOWN;
            unsigned scountdown  = DURATION / SSLOWDOWN;
            unsigned tcountdown  = DURATION / TSLOWDOWN;

            printf("CW %c ... CCW %c ... Explode %c ... Implode %c\r",
                  spinner[scount & 3], spinner[(scountdown - scount) & 3],
                  target[tcount % 3], target[(tcountdown - tcount) % 3]);
      }
      return 0;
}
