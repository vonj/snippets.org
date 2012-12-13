// Implements ansi.sys display class
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#include <iostream.h>
#include <ctype.h>
#include <string.h>

#include "ansisys.hpp"


ansisys::ansisys(video & v)
  : scrinterp(v),
    dispfunc(0),
    savx(0),
    savy(0)
{}

void
ansisys::reset()
{
  dispfunc = 0;
  savx = savy = 0;
  scrinterp::reset();
}

  // This is the top level dispatcher. It really only
  // Looks for special control characters

void
ansisys::putch(int ch)
{
  if (dispfunc)
    (this->*dispfunc)(ch);
  else if (ch == '\x1b')
  {
    flushbuf();
    dispfunc = &ansisys::esc; // Hand over to ESC routine
  }
  else
    scrinterp::putch(ch);
}

  // This parses for the leadin for an escape sequence

void
ansisys::esc(int ch)
{
  switch (ch)
  {
  case '[':                   // ANSI lead-in
    dispfunc = &ansisys::seq; // Now parse the sequences
    break;

  default:                    // Huh?
    putbuf('?');
    dispfunc = 0;
    break;
  }
}

int
ansisys::evalargs(cell_t * plist)
{
  chrbuf[chridx] = '\0';

  char const * s = chrbuf;
  int idx = 0;
  while (idx < 32)
  {
    cell_t res = 0;
    while (*s && isdigit(*s))   // Evaluate numeric
      res = cell_t((res * 10) + (*s++ - '0'));
    plist[idx++] = res;
    if (*s == ';')              // Skip over
      ++s;
    else break;                 // Not a valid delimiter, quit
  }

  return idx;
}


  // Parses esc sequences

void
ansisys::seq(int ch)
{
  if (ch == ';' || isdigit(ch))   // Accumulate parameters
  {
    if (chridx < CBUFSZ)
      chrbuf[chridx++] = char(ch);
  }
  else
  {
    cell_t  plist[32];
    cell_t  x,
            y,
            mx,
            my;
    int     nx =0,
            ny =0;

    int args = evalargs(plist);

    switch (ch)
    {
    default:    // Unknown sequences
      reset();
      break;

    case 'A':                 // Cursor up
      ny = -((args < 1 || plist[0] == 0) ? 1 : plist[0]);
      goto mvxy;

    case 'B':                 // Cursor down
      ny =  ((args < 1 || plist[0] == 0) ? 1 : plist[0]);
      goto mvxy;

    case 'C':                 // Cursor right
      nx =  ((args < 1 || plist[0] == 0) ? 1 : plist[0]);
      goto mvxy;

    case 'D':                 // Cursor left
      nx = -((args < 1 || plist[0] == 0) ? 1 : plist[0]);
    mvxy:
      vs.wherexy(x, y);
      x = cell_t(x + nx);
      y = cell_t(y + ny);
      goto goxy;

    case 'H':                 // Cursor position
      y = cell_t(((args < 1 || plist[0] == 0) ? 1 : plist[0]) - 1);
      x = cell_t(((args < 2 || plist[1] == 0) ? 1 : plist[1]) - 1);
    goxy:
      vs.maxxy(mx, my);
      if (x < mx && y < my)
        vs.gotoxy(x, y);
      break;

    case 'J':                 // Erase display
      if (args && plist[0] == 2)
        vs.gotoxy(0, 0);
      vs.cls();
      break;

    case 'K':                 // Erase line
      vs.wherexy(x, y);
      vs.maxxy(mx, my);
      vs.repchr(' ', mx - x);
      vs.gotoxy(x, y);
      break;

    case 'n':                 // Device status report
      // Not supported here
      // Need to send back ESC [ x+1 ; y+1 R
      break;

    case 's':                 // Save cursor position
      vs.wherexy(savx, savy);
      break;

    case 'u':                 // Restore cursor position
      vs.gotoxy(savx, savy);
      break;

    case 'm':                 // Set graphics rendition (colour)
      if (!args)
        plist[args++] = 0;
      for (int idx = 0 ; idx < args ; ++idx)
      {
        cell_t val = plist[idx];
        static cell_t colmap[] = { 0, 4, 2, 6, 1, 5, 3, 7 };

        switch (val)
        {
        case 0:
          vs.setattr(7);
          break;
        case 1:
          vs.setfg(cell_t(vs.getfg() | 0x8));
          break;
        case 2:
          vs.setfg(cell_t(vs.getbg() | 0x8));
          break;
        case 3:
          vs.setfg(cell_t((vs.getbg() & ~0x8) | 1));
          break;
        case 5:
          vs.setbg(cell_t(vs.getbg() | 0x8));
          break;
        case 7:
          vs.setfg(0);
          vs.setbg(cell_t(7 + (vs.getbg() & 0x8)));
          break;
        case 8:
          vs.setfg(cell_t(vs.getbg() & ~0x8));
          break;
        default:
          if (val >= 30 && val <= 37)
            vs.setfg(cell_t(colmap[val-30] | (vs.getfg() & 0x8)));
          else if (val >= 40 && val <= 47)
            vs.setbg(cell_t(colmap[val-40] | (vs.getbg() & 0x8)));
          break;
        }
      }
      break;

    }
    chridx = 0;
    dispfunc = 0;
  }
}

