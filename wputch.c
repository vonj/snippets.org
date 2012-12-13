/*
**  WPUTCH.C - Redirect text output to a scrollable window
**
**  public domain demo by Gaines Wright
**
**  Note: This code has been tested to work with Borland C++ 4.0+,
**        Microsoft C 7.0+, and Mix Power C 2.2.0+.
**
**        It will *not* work With Zortech C++ or Symantec C++ 6.x.
**
**        It compiles, but does not currently work correctly, with
**        Watcom C 10.0+ or Symantec C++ 7.0+ - this will be fixed ASAP.
*/

#include <dos.h>
#include <stdlib.h>

#if __POWERC | __TURBOC__
  #pragma option -N-
  #define IREGS unsigned r_bp, unsigned r_di, unsigned r_si, unsigned r_ds,\
                unsigned r_es, unsigned r_dx, unsigned r_cx, unsigned r_bx,\
                unsigned r_ax, unsigned r_ip, unsigned r_cs, unsigned r_flags
  #ifdef __cplusplus
    #define PAR ...
  #else
    #define PAR void
  #endif
  #ifdef __TURBOC__
       void interrupt far (*oldint29)(PAR);
  #else
    void interrupt (far *oldint29)(PAR);
  #endif
  #define ICAST (void interrupt (far*)(PAR))
#else
  #if (defined(__ZTC__) && !defined(__SC__)) || \
        (defined(__SC__) && __SC__ < 700)
    #error
  #endif

  #pragma check_stack(off)
  #pragma check_pointer(off)
  #define IREGS unsigned r_es, unsigned r_ds, unsigned r_di, unsigned r_si,\
                unsigned r_bp, unsigned r_sp, unsigned r_bx, unsigned r_dx,\
                unsigned r_cx, unsigned r_ax, unsigned r_ip, unsigned r_cs,\
                unsigned flags
  void (interrupt far *oldint29)();
  #if !defined(__WATCOMC__)
    #define REGS _REGS
  #endif
  #define ICAST (void (__interrupt far*)())
  #define getvect(a) _dos_getvect(a)
  #define setvect(a,b) _dos_setvect(a,b)
  #define interrupt __interrupt
#endif

#define SCRPOS(x,y) ((x)*2+(y)*160)        /* For upper left corner at 0,0. */
#define UP   6
#define DOWN 7

char xtop,ytop,xbot,ybot;                  /* Window coordinates. */
char attrib;                               /* Window attribute. */
union REGS iregs,oregs;
char far *topl,far *topr,far *ptr,far *fin;/* Window edge pointers */
char far *screen=(char far*)0xb8000000l;   /* Screen pointer. */

void scrollwin(char dir,char lines,char at,char yt,char xt,char yb,char xb)
{
      iregs.h.ah=dir;
      iregs.h.al=lines;
      iregs.h.bh=at;
      iregs.h.ch=yt;
      iregs.h.cl=xt;
      iregs.h.dh=yb;
      iregs.h.dl=xb;
      int86(0x10,&iregs,&oregs);
}


void setwindow(int xt,int yt,int xb,int yb,char a)
{
      xtop=xt;                            /* Set window coordinates. */
      ytop=yt;
      xbot=xb;
      ybot=yb;
      attrib=a;
      topl=screen+SCRPOS(xtop,ytop); /* Initialize pointers to window edges. */
      topr=screen+SCRPOS(xbot,ytop);
      fin=screen+SCRPOS(xbot,ybot);
      ptr=topl;
      scrollwin(UP,0,attrib,ytop,xtop,ybot,xbot);   /* Clears window */
}

void winputch(char c)                     /* Confines output to a window. */
{
      switch(c)
      {
      case '\r':
            return;

      case '\n':
            ptr=topr+1;
            break;       /* Treat '\n' as "\r\n". */

      case '\t':
            ptr+=16;
            break;          /* Expand tabs. */

      default  :
            *ptr++=c;
            ptr++;
            break;  /* Skip attribute byte. */
      }
      if(ptr>fin)                   /* If at bottom of window scroll up. */
      {
            scrollwin(UP,1,attrib,ytop,xtop,ybot,xbot);
            ptr=topl;                     /* Reset back to left edge. */
      }
      else if(ptr>topr)                   /* Wrap around at window right edge. */
      {
            topl+=160;                    /* For a screen width of 80. */
                  topr+=160;
            ptr=topl;
      }
}
      
#ifdef __TURBOC__
#pragma argsused /* shut up 'argument xxx is never used' warnings */
#endif

void interrupt far newint29(IREGS)
{
      winputch((char)r_ax);
}

main(void)
{
      oldint29=getvect(0x29);
      setvect(0x29,ICAST newint29);
      setwindow(0,0,79,24,7);                   /* Just to clear the screen. */
      setwindow(0,4,45,9,15+(1<<4));
      system("dir");
      setwindow(30,18,74,23,15+(1<<4));
      system("dir");
      setvect(0x29,oldint29);
      return 0;
}
