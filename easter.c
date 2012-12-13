/*
**  EASTER.C - Determine the date of Easter for any given year
**
**  public domain by Ed Bernal
*/

#include <stdlib.h>
#include "datetime.h"


void easter(int year,int *easter_month, int *easter_day)
{
      int a,b,c,e,g,h,i,k,u,x,z;
      div_t f;

      /*
      **  Gauss' famous algorithm (I don't know how or why it works,
      **  so there's no commenting)
      */

      a = year % 19;
      f = div(year,100);
      b = f.quot;
      c = f.rem;
      f = div(b,4);
      z = f.quot;
      e = f.rem;
      f = div((8*b + 13),25);
      g = f.quot;
      f = div((19*a + b - z - g + 15),30);
      h = f.rem;
      f = div((a + 11*h),319);
      u = f.quot;
      f = div(c,4);
      i = f.quot;
      k = f.rem;
      f = div((2*e + 2*i - k - h + u + 32),7);
      x = f.rem;
      f = div((h-u+x+90),25);
      *easter_month = f.quot;
      f = div((h-u+x + *easter_month +19),32);
      *easter_day = f.rem;
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      int yr, mo, dy;

      while (--argc)
      {
            yr = atoi(*++argv);
            if (100 > yr)
                  yr += 1900;
            easter(yr, &mo, &dy);
            printf("Easter in %d on %d/%d\n", yr, mo, dy);
      }
      return 0;
}

#endif /* TEST */
