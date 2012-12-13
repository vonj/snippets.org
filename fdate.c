/*
**  Public domain by Bob Stout
*/

#if !defined(__TURBOC__) && !defined(__SC__)
 #include "ftime.h"
#endif
#include <io.h>
#include <fcntl.h>
#include "ftime.h"
#include "scaldate.h"

/*
**  getfdate() - Return DOS file (handle) date as a SNIPPETS scalar date
*/

int getfdate (int handle, long *date)
{
      static struct ftime ftimep;
      int retval = 0;

      if (0 == (retval = getftime(handle, &ftimep)))
      {
            *date = ymd_to_scalar(ftimep.ft_year + 1980, ftimep.ft_month,
                  ftimep.ft_day);
      }
      return retval;
}

/*
**  getdatef() - Return DOS file (filename) date as a SNIPPETS scalar date
*/

int getdatef (char *fname, long *date)
{
      int fh;

      if (-1 != (fh = open(fname, O_RDONLY, 0)))
      {
            int retval;

            retval = getfdate(fh, date);
            close(fh);
            return retval;
      }
      else  return fh;
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      long date;

      while (--argc)
      {
            if (0 == getdatef(*++argv, &date))
            {
                  unsigned yr, mo, dy;

                  scalar_to_ymd(date, &yr, &mo, &dy);
                  printf("%-15s: %2d/%02d/%04d\n", *argv, mo, dy, yr);
            }
      }
      return 0;
}

#endif /* TEST */
