/*----------------------------------------------------------------------*
* Program:     touch                                                    *
* Programmer:  Ray L. McVay                                             *
* Started:     8 Aug 91                                                 *
* Updated:     13 Feb 93 Thad Smith                                     *
* Updated:     15 Feb 93 Bob Stout                                      *
*-----------------------------------------------------------------------*
* Simple touch program to test BC time stamping function.               *
* Public Domain                                                         *
*----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined(__TURBOC__) || defined(__SC__)
 #include <dos.h>
 #include <io.h>
#else
 #include "ftime.h"                 /* Borland work-alike in SNIPPETS   */
#endif

void usage(void);

main(int argc, char **argv)
{
      time_t    tnow;
      struct tm tmnow;
      struct ftime ft;
      FILE *f;

      if (argc < 2)
            usage();

      tnow  = time(NULL);
      tmnow = *localtime(&tnow);

      ft.ft_year  = tmnow.tm_year - 80;
      ft.ft_month = tmnow.tm_mon + 1;
      ft.ft_day   = tmnow.tm_mday;
      ft.ft_hour  = tmnow.tm_hour;
      ft.ft_min   = tmnow.tm_min;
      ft.ft_tsec  = tmnow.tm_sec/2;

      if ((f = fopen(argv[1], "r+b")) != NULL)
            setftime(fileno(f), &ft);
      else if ((f = fopen(argv[1], "w")) != NULL)
            setftime(fileno(f), &ft);
      else  perror("Can't open file");

      if (f)
            fclose(f);

      return EXIT_SUCCESS;
}

void usage(void)
{
      puts("Usage: TOUCH filename\n");
      puts("  The timestamp of filename will be set to the current time.");
      puts("  A zero-length file will be created if the file doesn't exist.");
      exit(EXIT_FAILURE);
}
