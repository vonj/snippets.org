// Implements Avatar display class
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#include <iostream.h>
#include <ctype.h>
#include <string.h>

#include "avatar.hpp"

#define AVT_DEFAULT   3
#define AVT_DLE       0x10


avatar::avatar(video & v)
  : scrinterp(v),
    dispfunc(0),
    counter(-1),
    arg1(-1),
    rptlen(-1)
{}

void
avatar::reset()
{
  dispfunc = 0;
  counter = -1;
  arg1 = -1;
  insertmode = 0;
  rptlen = -1;
  scrinterp::reset();
}

  // This is the top level dispatcher. It really only
  // Looks for special control characters

#define ctrl(x)   (x-'@')

void
avatar::putch(int ch)
{
  if (dispfunc)
    (this->*dispfunc)(ch);
  else
  {
    switch (ch)
    {
      case ctrl('L'):             // cls
        flushbuf();
        vs.gotoxy(0, 0);
        vs.setattr(AVT_DEFAULT);
        vs.cls();
        insertmode = 0;
        break;

      case ctrl('Y'):             // repeat chr
        flushbuf();
        dispfunc = &avatar::rptchar;
        counter = -1;
        break;

      case ctrl('V'):             // Video
        flushbuf();
        dispfunc = &avatar::setvideo;
        break;

      default:
        scrinterp::putch(ch);
        break;
    }
  }
}

void
avatar::rptchar(int ch)
{
  if (counter == -1)
    counter = ch;
  else
  {
    dispfunc = 0;
    int count = ch;
    ch = counter;
    counter = -1;
    while (count-- > 0)
      putch(ch);
  }
}

void
avatar::setvideo(int ch)
{
  cell_t  x, y,
          mx, my;
  int     nx =0,
          ny =0;

  switch (ch)
  {
    case ctrl('A'):
      dispfunc = &avatar::setattr;
      arg1 = -1;
      break;
    case ctrl('B'):       // Blink
      vs.setbg(cell_t(vs.getbg() | 0x8));
      dispfunc = 0;
      break;
    case ctrl('C'):       // Row up
      ny = -1;
      goto mvxy;
    case ctrl('D'):       // Row down
      ny = 1;
      goto mvxy;
    case ctrl('E'):       // Column to left (erase)
      nx = -1;
      goto mvxy;
    case ctrl('F'):       // Column to right (forward)
      nx = 1;
    mvxy:
      vs.wherexy(x, y);
      x = cell_t(x + nx);
      y = cell_t(y + ny);
      vs.maxxy(mx, my);
      if (x < mx && y < my)
        vs.gotoxy(x, y);
      dispfunc = 0;
      break;
    case ctrl('H'):       // Set cursor position
      arg1 = -1;
      dispfunc = &avatar::setpos;
      break;
    case ctrl('I'):       // Toggle insertmode on
      insertmode = 1;
      dispfunc = 0;
      return;
    case ctrl('J'):       // scroll area up
    case ctrl('K'):       // scroll area down
      dispfunc = &avatar::scroll;
      arg1 = ch;          // Save type of scroll
      counter = -1;       // Used as index to area coord
      break;
    case ctrl('L'):       // Clear area to attribute
      dispfunc = &avatar::clrarea;
      counter = -1;
      break;
    case ctrl('M'):
      dispfunc = &avatar::setarea;
      counter = -1;
      break;
    case ctrl('N'):
      // unsupported
      dispfunc = 0;
      break;
    case ctrl('G'):       // Clear to eol
      vs.wherexy(x, y);
      vs.maxxy(mx, my);
      vs.repchr(' ', mx - x);
      vs.gotoxy(x, y);
      dispfunc = 0;
      break;
    case ctrl('Y'):       // Repeat sequence
      dispfunc = &avatar::rptseq;
      counter = rptlen = -1;
      return;   // Don't reset insertmode
  }
  insertmode = 0;
}

void
avatar::resetattr(int ch)
{
  vs.setattr(cell_t(ch & 0x7f));
  dispfunc = 0;
  arg1 = -1;
}

void
avatar::setattr(int ch)
{
  if (arg1 == -1)         // No DLE
  {
    if (ch == AVT_DLE)    // Got a DLE
    {
      arg1 = 0;           // Call with next byte, and do ESC
      return;
    }
    resetattr(ch);
  }
  else    // Previous char was DLE escaped
  {
    if (ch & 0x80)        // This has a hi-bit
      resetattr(ch);      // So displaying is ok
    else
    {
      resetattr(AVT_DLE); // Else use DLE as attribute
      putch(ch);          // And redisplay this character
    }
  }
}

void
avatar::setpos(int ch)
{
  if (arg1 == -1)
    arg1 = ch;
  else
  {
    cell_t  x = cell_t(ch),
            y = cell_t(arg1);
    cell_t  mx, my;
    vs.maxxy(mx, my);
    --x; --y; // Avatar uses 1-based coords
    if (x < mx && y < my)
      vs.gotoxy(x, y);
    counter = arg1 = -1;
    dispfunc = 0;
  }
}

void
avatar::rptseq(int ch)
{
  if (rptlen == -1)             // Counter
  {
    counter = (ch & 0xff);
    ++rptlen;
  }
  else if (rptlen == counter)   // We've reached the end
  {
    int count = ch;             // Save these locally so we don't run into
    int len = rptlen;           // reentrancy problems (but recusion problems, yes!)
    rptlen = 0;                 // Reset everything to norm
    dispfunc = 0;
    arg1 = counter = -1;
    if (len)                    // For zero count, we can ignore the lot
    {
      char tmp[256];            // Make local copy of repeated sequence too
      memcpy(tmp, rptbuf, len); // Now, output the whole thing 'count' times
      for (int times = 0; times < count; times++)
        for (int idx = 0; idx < len; idx++)
          putch(tmp[idx]);
    }
  }                             // Just storing repeated sequence
  else rptbuf[rptlen++] = char(ch);
}

void
avatar::scroll(int ch)
{
  area[counter++] = cell_t(ch);   // Store data
  if (counter == 5)
  {
    #define s_lines area[0]
    #define s_upper area[1]
    #define s_left  area[2]
    #define s_lower area[3]
    #define s_right area[4]
    vs.scroll(--s_left, --s_upper, --s_right, --s_lower, s_lines, arg1 == ctrl('J'));
    rptlen = arg1 = counter = -1;
    dispfunc = 0;
  }
}

void
avatar::clrarea(int ch)
{
  area[counter++] = cell_t(ch);     // Store data
  if (counter == 3)
  {
    #define cl_attr  area[0]
    #define cl_rows  area[1]
    #define cl_cols  area[2]
    vs.setattr(cl_attr);
    cell_t x, y;
    vs.wherexy(x, y);
    vs.scroll(x, y, cell_t(x + cl_cols - 1), cell_t(y + cl_rows - 1), cl_rows, 1);
    rptlen = arg1 = counter = -1;
    dispfunc = 0;
  }
}

void
avatar::setarea(int ch)
{
  area[counter++] = cell_t(ch);   // Store data
  if (counter == 4)
  {
    #define sa_attr  area[0]
    #define sa_char  area[1]
    #define sa_rows  area[2]
    #define sa_cols  area[3]
    vs.setattr(sa_attr);
    vs.setfill(sa_char);
    cell_t x, y;
    vs.wherexy(x, y);
    vs.scroll(x, y, cell_t(x + sa_cols - 1), cell_t(y + sa_rows - 1), sa_rows, 1);
    vs.setfill(' ');
    rptlen = arg1 = counter = -1;
    dispfunc = 0;
  }
}

