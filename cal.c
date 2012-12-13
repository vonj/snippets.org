/*
**  CAL - a calendar for DOS
**
**  a public domain demo using Ray Gardner's SCALDATE.C scalar date functions
**  by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include "scaldate.h"

/*
**  calendar generation information
*/

int days[12]    = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char *month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char *daynames[8] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

/*
**  box drawing stuff
*/

#if defined(MSDOS) || defined(_WIN32)
 const char *topborder = "\xd5\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                         "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                         "\xcd\xcd\xcd\xcd\xb8";
 const char *midborder = "\xc6\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                         "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                         "\xcd\xcd\xcd\xcd\xb5";
 const char *botborder = "\xd4\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                         "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                         "\xcd\xcd\xcd\xcd\xbe";
 const char *line = "\xb3";
#else
 const char *line = "";
#endif

/*
**  tell 'em they messed up
*/

void usage(void)
{
      puts("Usage: CAL [m y]");
      puts("where: m = month (1 - 12)");
      puts("       y = year (1 - 99, 1800 - 3000)");
      exit(-1);
}

/*
**  here's where the real work's done
*/

int main(int argc, char *argv[])
{
      int day, day_1, numdays, i, j;
      unsigned yr, mo;

      if (argc < 3 && argc > 1)
            usage();

      if (argc >= 3)
      {
            yr = atoi(argv[2]);
            mo = atoi(argv[1]);
      }
      else
      {
            long dnow = today();
            unsigned dy;

            scalar_to_ymd(dnow, &yr, &mo, &dy);
      }

      if (!mo || 12 < mo)
            usage();

      if (100 > yr)
            yr += 1900;

      if (3000 < yr || 1800 > yr)
            usage();

      for (i = 0, mo -= 1; i < 3; ++i, ++mo)
      {
            if (!mo)
            {
                  mo = 12;
                  --yr;
            }
            if (12 < mo)
            {
                  mo = 1;
                  ++yr;
            }
            numdays = days[mo - 1];
            if (2 == mo && isleap(yr))
                  ++numdays;
            day_1 = (int)((ymd_to_scalar(yr, mo, 1) - (long)ISO_CAL) % 7L);

#if defined(MSDOS) || defined(_WIN32)
            if (!i)
                  puts(topborder);
#endif
            fputs(line, stdout);
            for (j = 0; j < 7; )
            {
                  fputs(daynames[ISO_CAL + j], stdout);
                  if (7 != ++j)
                        fputc(' ', stdout);
            }
            printf("%s < %s, %d\n%s", line, month[mo - 1], yr, line);

            for (day = 0; day < day_1; ++day)
                  fputs("    ", stdout);
            for (day = 1; day <= numdays; ++day, ++day_1, day_1 %= 7)
            {
                  if (!day_1 && 1 != day)
                        printf("\b%s\n%s", line, line);
                  printf("%3d ", day);
            }
            for ( ; day_1; ++day_1, day_1 %= 7)
                  fputs("    ", stdout);
#if defined(MSDOS) || defined(_WIN32)
            printf("\b%s\n", line);
            if (2 > i)
                  puts(midborder);
            else  puts(botborder);
#else
            fputc('\n', stdout);
#endif
      }
      return 0;
}
