// Base class for screen interpreter
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#include <iostream.h>
#include "scrintrp.hpp"

const int scrinterp::CBUFSZ = 127;

scrinterp::scrinterp(video & v)
  : vs(v),
    chrbuf(new char [CBUFSZ+1]),
    chridx(0),
    flushevery(0),
    insertmode(0)
{}

scrinterp::~scrinterp()
{
  flushbuf();
  delete[] chrbuf;
}

void scrinterp::reset()
{
  chridx = 0;
}

  // This is the basic get-a-character/display-it
  // loop that is used to drive the engine

void
scrinterp::display(istream & is)
{
  int ch =0;
  reset();
  while (is.good() && (ch = is.get()) != EOF)
    putch(ch);
  flushbuf();
}

void
scrinterp::flushbuf()
{
  if (chridx)
  {
    chrbuf[chridx] = '\0';
    vs.put(chrbuf);
    chridx = 0;
  }
}

void
scrinterp::putbuf(int ch)
{
  if (chridx >= CBUFSZ)
    flushbuf();
  chrbuf[chridx++] = char(ch);
}

void
scrinterp::putch(int ch)
{
  cell_t x, y, mx, my;

  // We can handle all of the standard control chrs here
  switch (ch)
  {
  case '\r':                  // CR
    flushbuf();
    vs.wherexy(x, y);
    vs.gotoxy(0, y);
    break;

  case '\t':                  // TAB
    flushbuf();
    vs.wherexy(x, y);
    vs.maxxy(mx, my);
    x = cell_t((((x + 8) / 8) + 1) * 8);
    if (x >= mx)
    {
      x = cell_t(x % mx);
      goto dolf;
    }
    vs.gotoxy(x, y);
    break;

  case '\n':                  // NL
    flushbuf();
    vs.wherexy(x, y);
    vs.maxxy(mx, my);
  dolf:
    if (y == cell_t(my - 1))  // On last line
      vs.scroll(0, 0, mx, my, 1);
    else
      ++y;
    vs.gotoxy(x, y);
    break;

  case '\007':                 // BELL
    // beep() !
    break;

  case '\b':
    vs.wherexy(x, y);
    if (x > 0)
      vs.gotoxy(--x, y);
    break;

  case '\x0c':                // LF
    // Should to a cls here with home, perhaps...
    break;

  default:
    putbuf(ch);
    if (flushevery)
      flushbuf();
    break;
  }
}

