/*
** Public Domain mode 13h Bresenham line/circle algorithms
** By Brian Dessent
**
** Circle algorithm and other stuff rewritten by Kurt Kuzba
**
** Written for Borland, modified for others by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include "bresnham.h"
#include "mk_fp.h"

/* uses BIOS to set video mode */

void setmode(int mode)
{
      union REGS r;

      r.x.ax = mode;
      int86(0x10, &r, &r);
}


/* plots a dot at (x, y) with color c */

void plotdot(int x, int y, char c)
{
      register char far *addr;

      if(x < 0 || x > MAX_X || y < 0 || y > MAX_Y)
            return;

      addr = MK_FP(0xa000, (SCREEN_WIDTH * y) + x);
      *addr = c;
}


/* draws a line from (x, y) to (x2, y2) in color c */

void bresenham_line(int x, int y, int x2, int y2, char c)
{
      int i, steep = 0, sx, sy, dx, dy, e;

      dx = abs(x2 - x);
      sx = ((x2 - x) > 0) ? 1 : -1;
      dy = abs(y2 - y);
      sy = ((y2 - y) > 0) ? 1 : -1;

      if(dy > dx)
      {
            steep =  x;   x =  y;   y = steep;  /* swap  x and  y */
            steep = dx;  dx = dy;  dy = steep;  /* swap dx and dy */
            steep = sx;  sx = sy;  sy = steep;  /* swap sx and sy */
            steep = 1;
      }

      e = 2 * dy - dx;
      for(i = 0; i < dx; i++)
      {
            if(steep)
                  plotdot(y, x, c);
            else  plotdot(x, y, c);
            while(e >= 0)
            {
                  y += sy;
                  e -= 2 * dx;
            }
            x += sx;
            e += 2 * dy;
      }
      plotdot(x2, y2, c);
}

/* draws a circle at (xc, yc) with radius r in color c
**
** note: the scaling factor of (SCREEN_WIDTH / SCREEN_HEIGHT) is used when
** updating d.  This makes round circles.  If you want ellipses, you can
** modify that ratio.
*/

void bresenham_circle(int xc, int yc, int r, char c)
{
      int x = 0, d = 2 * (1 - r), w = 2 * SCREEN_WIDTH / SCREEN_HEIGHT;

      while(r >= 0)
      {
            plotdot(xc + x, yc + r, c);
            plotdot(xc + x, yc - r, c);
            plotdot(xc - x, yc + r, c);
            plotdot(xc - x, yc - r, c);
            if (d + r > 0)
                  d -= (w * --r) - 1;
            if (x > d)
                  d += (2 * ++x) + 1;
      }
}

#ifdef TEST

#ifndef __TURBOC__
 #include "bc_rand.h"
#else
 #include <time.h>
#endif

/* draws random lines and circles until a key is pressed in mode 13h */
/* (draws in colors 0 - 63 only) */

int main()
{
      int i=0;

      randomize();
      setmode(0x13);
      while(!kbhit())
      {
            bresenham_line(random(SCREEN_WIDTH), random(SCREEN_HEIGHT),
                  random(SCREEN_WIDTH), random(SCREEN_HEIGHT), i = ++i % 64);
            bresenham_circle(random(SCREEN_WIDTH), random(SCREEN_HEIGHT),
                  random(50), i = ++i % 64);
      }
      getch();
      setmode(0x03);  /* set to color text mode, clearing screen */
      return 0;
}

#endif /* TEST */
