// Implements a simple text video control class
// public domain
// by David L. Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet
// Implementation for 16 & 32-bit - MSDOS & OS/2

#include "video.hpp"
#include <string.h>

#if defined(__EMX__)
#define __OS2__
#endif

#if defined(__OS2__)
#define INCL_VIO
#define INCL_KBD
#include <os2.h>
#else
# include <dos.h>
# include <bios.h>
# if defined(__386__)   // Note: assumes DOS4GW
#  define vbios(r) int386(0x10,(r),(r))
#  undef far
#  define far
static unsigned short * screenPtr = (unsigned short *)0xb8000;
# else
#  define vbios(r) int86(0x10,(r),(r))
static unsigned short far * screenPtr = (unsigned short far *)MK_FP(0xb800,0);
# endif
#endif

#define NO_VAL  cell_t(0xffffu)

video::video()
  : maxx(NO_VAL), maxy(NO_VAL),
    curx(NO_VAL), cury(NO_VAL),
    attr(7), fill(' ')
{
  maxxy(maxx, maxy);
  wherexy(curx, cury);
}

video::~video()
{
  putcursor();
}

void
video::cls()
{
  scroll(0, 0, NO_VAL, NO_VAL, NO_VAL, 1);
}

void
video::scroll(cell_t left, cell_t top, cell_t right, cell_t bottom, cell_t lines, char dir)
{
#if defined(__OS2__)
  BYTE cell[2] = { (BYTE)fill, (BYTE)attr };
  if (dir)
    VioScrollUp(top, left, bottom, right, lines, cell, 0);
  else
    VioScrollDn(top, left, bottom, right, lines, cell, 0);
#else
  union REGS r;
  r.h.ah = (unsigned char)(dir ? 6 : 7);
  r.h.al = (unsigned char)((lines == NO_VAL) ? 0 : lines);
  r.h.bh = (unsigned char)attr;
  r.h.bl = (unsigned char)fill;
  r.h.ch = (unsigned char)top;
  r.h.cl = (unsigned char)left;
  r.h.dh = (unsigned char)((bottom == NO_VAL) ? (maxy-1) : bottom);
  r.h.dl = (unsigned char)((right == NO_VAL) ? (maxx-1) : right);
  vbios(&r);
#endif
}

void
video::gotoxy(cell_t x, cell_t y)
{
  cury = y;
  curx = x;
}

void
video::putcursor()
{
  if (cury > maxy)
  {
    cell_t toscroll = cell_t(cury - maxy + 1);
    scroll(0, 0, maxx, maxy, toscroll);
    cury -= toscroll;
  }
#if defined(__OS2__)
  VioSetCurPos(cury, curx, 0);
#else
  union REGS r;
  r.h.ah = 2;
  r.h.bh = 0;
  r.h.dh = (unsigned char)cury;
  r.h.dl = (unsigned char)curx;
  vbios(&r);
#endif
}


void
video::maxxy(cell_t & cols, cell_t & rows)
{
  if (maxx == NO_VAL && maxy)
  {
#if defined(__OS2__)
    VIOMODEINFO vm;
    vm.cb = sizeof(vm);
    VioGetMode(&vm, 0);
    maxx = vm.col;
    maxy = vm.row;
#else
    union REGS r;
    r.x.ax = 0x0f00;
    vbios(&r);
    maxx = r.h.ah;
    if (r.h.al == 7 || r.h.al == 15)
# if defined(__386__)
      screenPtr = (unsigned short *)(0xB000 << 4);
# else
      screenPtr = (unsigned short far *)MK_FP(0xB000,0);
# endif
    unsigned char far * p = (unsigned char far *)0x484;
    maxy = *p;
    if (!maxy)
      maxy = 25;
    else
      ++maxy;
#endif
  }
  cols = maxx;
  rows = maxy;
}

void
video::wherexy(cell_t & x, cell_t & y)
{
  if (curx == NO_VAL && cury == NO_VAL)
  { // Only need to check this the once
#if defined(__OS2__)
    VioGetCurPos(&cury, &curx, 0);
#else
    union REGS r;

    r.h.ah = 3;
    r.h.bh = 0;
    vbios(&r);
    cury = r.h.dh;
    curx = r.h.dl;
#endif
  }
  x = curx;
  y = cury;
}

void
video::put(register int ch)
{
  putcursor();
#if defined(__OS2__)
  BYTE tmp[2];
  tmp[0] = BYTE(ch);
  tmp[1] = BYTE(attr);
  VioWrtCellStr((PCH)tmp, 2, cury, curx, 0);
#else
  unsigned short far * ptr = screenPtr + (cury * maxx) + curx;
  *ptr++ = (unsigned short)((ch & 0xff) | (attr << 8));
#endif
  adjustcursor(1);
}

void
video::put(char const * s)
{
  putcursor();
#if defined(__OS2__)
  USHORT l = USHORT(strlen(s) * 2);
  BYTE * tmp = new BYTE[l];
  for (USHORT i = 0 ; i < l ;)
  {
    tmp[i++] = *s++;
    tmp[i++] = BYTE(attr);
  }
  VioWrtCellStr((PCH)tmp, l, cury, curx, 0);
  delete[] tmp;
  l /= 2;
#else
  unsigned short far * ptr = screenPtr + (cury * maxx) + curx;
  unsigned short l = (unsigned short) strlen(s);
  for (unsigned short i = 0 ; i < l ; ++i)
    *ptr++ = (unsigned short)((*s++ & 0xff) | (attr << 8));
#endif
  adjustcursor(l);
}

void
video::adjustcursor(cell_t cols, cell_t rows)
{
  curx += cols;
  rows += (curx / maxx);
  curx  = cell_t(curx % maxx);
  cury += rows;
  gotoxy(curx, cury);
}

void
video::repchr(int ch, int n)
{
#if defined(__OS2__)
  USHORT l = USHORT(n * 2);
  BYTE * tmp = new BYTE[l];
  for (USHORT i = 0 ; i < l ;)
  {
    tmp[i++] = char(ch);
    tmp[i++] = BYTE(attr);
  }
  VioWrtCellStr((PCH)tmp, l, cury, curx, 0);
  delete[] tmp;
#else
  unsigned short far * ptr = screenPtr + (cury * maxx) + curx;
  for (unsigned short i = 0 ; i < (unsigned short)n ; ++i)
    *ptr++ = (unsigned short)((ch & 0xff) | (attr << 8));
#endif
  adjustcursor(cell_t(n));
}

unsigned short
video::getkey()
{
  putcursor();
#if defined(__OS2__)
  KBDKEYINFO K;
  KbdCharIn(&K, IO_WAIT, 0);
  return (unsigned short)((K.chScan << 8) | K.chChar);
#else
  return _bios_keybrd(_KEYBRD_READ);
#endif
}

