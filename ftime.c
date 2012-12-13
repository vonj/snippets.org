/*
**  Public domain by Jeff Dunlop & Bob Stout
*/

#include <time.h>
#include "ftime.h"

#if !defined(__TURBOC__) && !defined(__SC__) && !defined(__POWERC)

#ifdef __ZTC__
 #pragma ZTC align 1
 #define DOS_GETFTIME dos_getftime
 #define DOS_SETFTIME dos_setftime
 typedef unsigned FTIME_T_;
#else
 #pragma pack(1)
 #define DOS_GETFTIME _dos_getftime
 #define DOS_SETFTIME _dos_setftime
 #ifdef __WATCOMC__
  typedef unsigned short FTIME_T_;
 #else
  typedef unsigned FTIME_T_;
 #endif
#endif

int getftime (int handle, struct ftime *ftimep)
{
      int retval = 0;
      union
      {
            struct
            {
                  unsigned time;
                  unsigned date;
            } msc_time;
            struct ftime bc_time;
      } FTIME;

      if (0 == (retval = DOS_GETFTIME(handle,
            (FTIME_T_ *)&FTIME.msc_time.date,
            (FTIME_T_ *)&FTIME.msc_time.time)))
      {
            *ftimep = FTIME.bc_time;
      }
      return retval;
}

int setftime (int handle, struct ftime *ftimep)
{
      union
      {
            struct
            {
                  unsigned time;
                  unsigned date;
            } msc_time;
            struct ftime bc_time;
      } FTIME;

      FTIME.bc_time = *ftimep;

      return DOS_SETFTIME(handle, FTIME.msc_time.date, FTIME.msc_time.time);
}

#endif

static void _ftimecnvrt(struct ftime *ft, struct tm *time, time_t *tt)
{
      time->tm_sec  = ft->ft_tsec * 2;
      time->tm_min  = ft->ft_min;
      time->tm_hour = ft->ft_hour;
      time->tm_mday = ft->ft_day;
      time->tm_mon  = ft->ft_month - 1;
      time->tm_year = ft->ft_year + 80;
      *tt = mktime(time);                 /* Fill in rest of the struct */
}

void ftime2tm(struct ftime *ft, struct tm *time)
{
      time_t tt;
      
      _ftimecnvrt(ft, time, &tt);
}

time_t ftime2time(struct ftime *ft)
{
      struct tm time;
      time_t tt;
      
      _ftimecnvrt(ft, &time, &tt);
      return tt;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif
#include "errors.h"
#include "sniptype.h"

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      struct ftime ft;
      struct tm time;
      FILE *fp;
      char timeline[80];

      fp = cant(argv[0], "r");
      if (Success_ != getftime(fileno(fp), &ft))
            ErrExit("getftime() failed");
      ftime2tm(&ft, &time);
      printf("ft_tsec = %d\n", ft.ft_tsec);
      printf("ft_min  = %d\n", ft.ft_min);
      printf("ft_hour = %d\n", ft.ft_hour);
      printf("ft_day  = %d\n", ft.ft_day);
      printf("ft_month= %d\n", ft.ft_month);
      printf("ft_year = %d\n", ft.ft_year);
      puts("");
      printf("tm_sec  = %d\n", time.tm_sec);
      printf("tm_min  = %d\n", time.tm_min);
      printf("tm_hour = %d\n", time.tm_hour);
      printf("tm_mday = %d\n", time.tm_mday);
      printf("tm_mon  = %d\n", time.tm_mon);
      printf("tm_year = %d\n", time.tm_year);

      strftime(timeline, 80, "\n%c\n", &time);
      puts(timeline);
      return EXIT_SUCCESS;
}

#endif /* TEST */
