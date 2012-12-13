/*
**  DOANSI_2.C - OS-Specific ANSI screen code interpreter functions
**
**  From DRSK_105.LZH (ansi.c), hereby declared free for use for whatever
**  purposes by author: Mark Kimes
*/

#include "doansi.h"
#include "mk_fp.h"

int   realmaxy, realmaxx;
int   maxx, maxy;

#ifdef OS2

#define INCL_DOS
#define INCL_VIO

#include <os2.h>

int vidhandle = 0;  /* can be changed for AVIO */

void set_screensize (int reservedlines)
{
      VIOMODEINFO vm;

      vm.cb = sizeof(VIOMODEINFO);
      VioGetMode(&vm, vidhandle);
      maxx = vm.col;
      maxy = vm.row - reservedlines;
      realmaxx = maxx;
      realmaxy = vm.row;
}

void pos_hardcursor (int x,int y)
{
      VioSetCurPos(y,x,vidhandle);
}

void hardcursor_off (void)
{
      VIOCURSORINFO vc;

      VioGetCurType(&vc,vidhandle);
      vc.attr = -1;
      VioSetCurType(&vc,vidhandle);
}

void hardcursor_on (int x,int y)
{
      VIOCURSORINFO vc;

      VioGetCurType(&vc,vidhandle);
      vc.attr = 0;
      VioSetCurType(&vc,vidhandle);
      VioSetCurPos(y,x,vidhandle);
}

void put_char (char c, char attr, int x, int y)
{
      VioWrtCharStrAtt(&c,1,y,x,&attr,vidhandle);
}

void scroll_up (int tx,int ty,int bx,int by,char attr)
{
      int attrib = ' ' | (attr << 8);

      VioScrollUp(ty,tx,by,bx,1,(char *)&attrib,vidhandle);
}

void clearwindow (int tx,int ty,int bx,int by,char attr)
{
      int attrib = ' ' | (attr << 8);

      VioScrollUp(ty,tx,by,bx,-1,(char *)&attrib,vidhandle);
}

void cleartoeol (int x,int y,int ex,char attr)
{
      int attrib = ' ' | (attr << 8);

      VioScrollUp(y,x,y,ex,-1,(char *)&attrib,vidhandle);
}

#else

/* MS-DOS -- (urp) */

#include <dos.h>

#if !defined(MK_FP)
 #define MK_FP(seg,off) ((void far *)(((long)(seg) << 16)|(unsigned)(off)))
#endif

static int far *vseg;
char           usebios = 0; /* if true, output through BIOS */

int vmode (void)
{
      union REGS r;

      r.h.ah = 15;
      r.x.bx = 0;
      int86(0x10,&r,&r);
      return r.h.al;
}

void set_screensize (int reservedlines)
{
      union REGS   r;
      unsigned int vbase;

      r.h.ah = 0x0f;
      r.x.bx = 0;
      int86 (0x10, &r, &r);
      maxx = (int) r.h.ah;
      if (maxx < 80)                                  /* gimme a break! */
      {
            r.x.ax = 0x0003;
            int86(0x10,&r,&r);
            maxx = 80;
      }
      realmaxx = maxx;
      r.x.ax = 0x1130;
      r.x.dx = maxy;
      int86 (0x10, &r, &r);
      realmaxy = maxy = (r.x.dx == 0) ? 25 : (r.x.dx + 1);
      maxy -= reservedlines;
      vbase = (vmode () == 7 ? 0xb000 : 0xb800);
      vseg = MK_FP(vbase,0);        /* address of video ram as pointer */
}

void pos_hardcursor (int x,int y)
{
      union REGS r;

      r.x.ax = 0x0200;
      r.x.bx = 0;
      r.x.dx = ((y << 8) & 0xff00) + x;
      int86(0x10,&r,&r);
}

void hardcursor_off (void)
{
      union REGS r;

      r.x.ax = 0x0200;
      r.x.bx = 0;
      r.x.dx = ((realmaxy << 8) & 0xff00);
      int86(0x10,&r,&r);
}

void hardcursor_on (int x,int y)
{
      union REGS r;

      r.x.ax = 0x0200;
      r.x.bx = 0;
      r.x.dx = ((y << 8) & 0xff00) + x;
      int86(0x10,&r,&r);
}

void put_char (char c, char attr, int x, int y)
{
      if(!usebios)
      {
            register int far *v;

             /* point v to right spot in vid RAM */

            v = vseg + ((y * realmaxx) + x);
            *v = (c | (attr << 8));                   /* display */
      }
      else
      {

            union REGS r;

            r.x.ax = 0x0200;
            r.x.bx = 0;
            r.x.dx = ((y << 8) & 0xff00) + x;
            int86(0x10,&r,&r);
            r.h.ah = 0x09;
            r.h.bh = 0;
            r.h.bl = attr;
            r.x.cx = 1;
            r.h.al = c;
            int86(0x10,&r,&r);
      }
}

void scroll_up (int tx,int ty,int bx,int by,char attr)
{
      union REGS r;

      r.h.ah = 6;
      r.h.al = 1;
      r.h.bh = attr;
      r.h.cl = tx;
      r.h.ch = ty;
      r.h.dl = bx;
      r.h.dh = by;
      int86(0x10,&r,&r);
}

void clearwindow (int tx,int ty,int bx,int by,char attr)
{
      union REGS r;

      r.h.ah = 6;
      r.h.al = 0;
      r.h.bh = attr;
      r.h.cl = tx;
      r.h.ch = ty;
      r.h.dl = bx;
      r.h.dh = by;
      int86(0x10,&r,&r);
}

void cleartoeol (int x,int y,int ex,char attr)
{
      union REGS r;

      r.h.ah = 6;
      r.h.al = 0;
      r.h.bh = attr;
      r.h.cl = x;
      r.h.ch = y;
      r.h.dl = ex;
      r.h.dh = y;
      int86(0x10,&r,&r);
}

#endif
