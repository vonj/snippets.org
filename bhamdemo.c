/*
**  BHAMDEMO.C
**
**  Demonstration of Bresenham algorithms for line and circle.
**  public domain by Kurt Kuzba
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "bresnham.h"

int main()
{
      unsigned d = 0, c = 0, color = 0, lines[473][2];

      srand(*((unsigned*)0x46c));
      setmode(0x13);    /* set to color graphics mode, clearing screen  */
      bresenham_circle(160, 100, 75, 15);
      for (d = 0; d < 64000l; d++)
      {
            if(*((char far*)0xa0000000l + d) == 15)
                  lines[c][1] = d / 320, lines[c++][0] = d % 320;
      }
      while (*((char far*)0x41a) == *((char far*)0x41c))
      {
            bresenham_circle(160, 100,
                  (++color & 127) + 80, (color / 5 ) & 255);
            d = rand() % 465 + 4;
            c = (d + 3) % 472;
            bresenham_line(lines[d][0], lines[d][1],
                  lines[c][0], lines[c][1], color & 255);
      }
      setmode(0x03);    /* set to color text mode, clearing screen      */
      getch();
      return 0;
}
