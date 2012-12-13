/*
**  DOW.C
**
**  Public domain by VinhHao Nguyen, 03/94
**
**  Note: This function is redundant with the function of the same name
**        in DAYNUM.C. Which one to use?
**
**        If you're already using any functions in SCALDATE.C, DAYNUM.C,
**        or TODAY.C, ignore this file and use the other dow().
**
**        If you need a stand-alone DOW() function without using any of
**        the rest of the SCALDATE package, use this file.
*/

#include "scaldate.h"

#if ISO_CAL       /* monday == 0 */
 #define ADJ 5
#else             /* sunday == 0 */
 #define ADJ 6
#endif

unsigned DOW(unsigned y, unsigned m, unsigned d)
{
      if (m < 3)
      {
            m += 13;
            y--;
      }
      else  m++;
      return (d + 26 * m / 10 + y + y / 4 - y / 100 + y / 400 + ADJ) % 7;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      int Day;
      void usage(void);
      unsigned d, m, y, days[] = {31, 29, 31, 30, 31, 30,
                                  31, 31, 30, 31, 30, 31};
      char *day[2][7] = {
            {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"},
            {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}
      };
      char *month[]  = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",};
      

      if (4 > argc)
            usage();
      y = atoi(argv[1]);
      m = atoi(argv[2]);
      d = atoi(argv[3]);
      if (!m || m > 12)
            usage();
      if (!d || d > days[m - 1])
            usage();
      if (y < 100)
            y += 1900;
      Day = DOW(y, m, d);
      printf("DOW(ISO_CAL=%d) returned %d, so %d %s %d is a %s\n",
            ISO_CAL, Day, d, month[m - 1], y, day[ADJ - 5][Day]);
      return 0;
}

void usage(void)
{
      puts("Usage: DOW yy[yy] mm dd");
      exit(-1);
}

#endif /* TEST */
