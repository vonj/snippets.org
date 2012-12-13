/*
**  DOANSI_1.C - Portable ANSI screen code interpreter
**
**  From DRSK_105.LZH (ansi.c), hereby declared free for use for whatever
**  purposes by author: Mark Kimes
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "doansi.h"

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif

#ifdef __TURBOC__   /* shut up 'Parameter not used' warnings        */
 #pragma warn -par  /* in the BC3.1 IDE the setting of Options,     */
#endif              /* Compiler,Messages,Display must NOT be 'All'. */

/*
 * to initialize:
 *   call set_screensize(<# lines to reserve>);
 * to print through ansi interpreter:
 *   call ansi_out(<string>);
 */

char               curattr = 7;
int                curx = 0,cury = 0;
int                maxx = 80, maxy = 25;  /* size of ansi output window */
int                realmaxy,realmaxx;     /* real screen size */
char               useansi = 1;           /* while true, interp ansi seqs */
int                tabspaces = 8;
static int         savx,savy,issaved = 0;
static char        ansi_terminators[] = "HFABCDnsuJKmp";

#define MAXARGLEN       128

#define NOTHING         0
#define WASESCAPE       1
#define WASBRKT         2

/* "generic" support functions closely related to ansi_out */

void set_pos (char *argbuf,int arglen,char cmd)
{
      int   y,x;
      char *p;

      if (!*argbuf || !arglen)
      {
            curx = cury = 0;
      }
      y = atoi(argbuf) - 1;
      p = strchr(argbuf,';');
      if (y >= 0 && p)
      {
            x = atoi(p + 1) - 1;
            if(x >= 0)
            {
                  curx = x;
                  cury = y;
            }
      }
}

void go_up (char *argbuf,int arglen,char cmd)
{
      int x;

      x = atoi(argbuf);
      if (!x)
            x = 1;
      for ( ; x ; x--)
      {
            if (!cury)
                  break;
            cury--;
      }
}

void go_down (char *argbuf,int arglen,char cmd)
{
      int x;

      x = atoi(argbuf);
      if (!x)
            x = 1;
      for ( ; x ; x--)
      {
            if (cury == maxy - 1)
                  break;
            cury++;
      }
}

void go_left (char *argbuf,int arglen,char cmd)
{
      int x;

      x = atoi(argbuf);
      if (!x)
            x = 1;
      for ( ; x ; x--)
      {
            if(!curx)
                  break;
            curx--;
      }
}

void go_right (char *argbuf,int arglen,char cmd)
{
      int x;

      x = atoi(argbuf);
      if (!x)
            x = 1;
      for ( ; x ; x--)
      {
            if (curx == maxx - 1)
                  break;
            curx++;
      }
}

void report (char *argbuf,int arglen,char cmd)
{
      /* you figure out how to implement it ... */
}

void save_pos (char *argbuf,int arglen,char cmd)
{
      savx = curx;
      savy = cury;
      issaved = 1;
}

void restore_pos (char *argbuf,int arglen,char cmd)
{
      if(issaved)
      {
            curx = savx;
            cury = savy;
            issaved = 0;
      }
}

void clear_screen (char *argbuf,int arglen,char cmd)
{
      /* needs error checking */

      clearwindow(0,0,maxx - 1,maxy - 1,curattr);
      curx = cury = 0;
}

void kill_line (char *argbuf,int arglen,char cmd)
{
      cleartoeol(curx,cury,maxx - 1,curattr);
}


void set_colors (char *argbuf,int arglen,char cmd)
{
      char *p,*pp;

      if (*argbuf && arglen)
      {
            pp = argbuf;
            do
            {
                  p = strchr(pp,';');
                  if (p && *p)
                  {
                        *p = 0;
                        p++;
                  }
                  switch (atoi(pp))
                  {
                  case 0: /* all attributes off */
                        curattr = 7;
                        break;

                  case 1: /* bright on */
                        curattr |= 8;
                        break;

                  case 2: /* faint on */
                        curattr &= (~8);
                        break;

                  case 3: /* italic on */
                        break;

                  case 5: /* blink on */
                        curattr |= 128;
                        break;

                  case 6: /* rapid blink on */
                        break;

                  case 7: /* reverse video on */
                        curattr = 112;
                        break;

                  case 8: /* concealed on */
                        curattr = 0;
                        break;

                  case 30: /* black fg */
                        curattr &= (~7);
                        break;

                  case 31: /* red fg */
                        curattr &= (~7);
                        curattr |= 4;
                        break;

                  case 32: /* green fg */
                        curattr &= (~7);
                        curattr |= 2;
                        break;

                  case 33: /* yellow fg */
                        curattr &= (~7);
                        curattr |= 6;
                        break;

                  case 34: /* blue fg */
                        curattr &= (~7);
                        curattr |= 1;
                        break;

                  case 35: /* magenta fg */
                        curattr &= (~7);
                        curattr |= 5;
                        break;

                  case 36: /* cyan fg */
                        curattr &= (~7);
                        curattr |= 3;
                        break;

                  case 37: /* white fg */
                        curattr |= 7;
                        break;

                  case 40: /* black bg */
                        curattr &= (~112);
                        break;

                  case 41: /* red bg */
                        curattr &= (~112);
                        curattr |= (4 << 4);
                        break;

                  case 42: /* green bg */
                        curattr &= (~112);
                        curattr |= (2 << 4);
                        break;

                  case 43: /* yellow bg */
                        curattr &= (~112);
                        curattr |= (6 << 4);
                        break;

                  case 44: /* blue bg */
                        curattr &= (~112);
                        curattr |= (1 << 4);
                        break;

                  case 45: /* magenta bg */
                        curattr &= (~112);
                        curattr |= (5 << 4);
                        break;

                  case 46: /* cyan bg */
                        curattr &= (~112);
                        curattr |= (3 << 4);
                        break;

                  case 47: /* white bg */
                        curattr |= 112;
                        break;

                  case 48: /* subscript bg */
                        break;

                  case 49: /* superscript bg */
                        break;

                  default: /* unsupported */
                        break;
                  }
                  pp = p;
            } while (p);
      }
}

int ansi_out (char *buf)
{
      int  arglen = 0, ansistate = NOTHING, x;
      char *b = buf, argbuf[MAXARGLEN] = "";

      /* cursor is off while string is being displayed so we don't have
         to keep updating it.  works to our detriment only if using
         BIOS writes under MS-DOS
      */

      hardcursor_off();

      if (!useansi)                       /* is ANSI interp on? */
      {
            ansistate = NOTHING;
            arglen = 0;
            *argbuf = 0;
      }

      while (*b)
      {
            switch (ansistate)
            {
            case NOTHING:
                  switch (*b)
                  {
                  case '\x1b':
                        if (useansi)
                        {
                              ansistate = WASESCAPE;
                              break;
                        }

                  case '\r':
                        curx = 0;
                        break;

                  case '\n':
                        cury++;
                        if (cury > maxy - 1)
                        {
                              scroll_up(0,0,maxx - 1,maxy - 1,curattr);
                              cury--;
                        }
                        break;

                  case '\t':     /* so _you_ figure out what to do... */
                        for (x = 0; x < tabspaces; x++)
                        {
                              put_char(' ',curattr,curx,cury);
                              curx++;
                              if (curx > maxx - 1)
                              {
                                    curx = 0;
                                    cury++;
                                    if (cury > maxy - 1)
                                    {
                                          scroll_up(0, 0, maxx - 1, maxy - 1,
                                                curattr);
                                          cury--;
                                    }
                              }
                        }
                        break;

                  case '\b':
                        if (curx)
                        {
                              curx--;
                        }
                        break;

                  case '\07':     /* usually a console bell */
                        putchar('\07');
                        break;

                  default:
                        put_char(*b,curattr,curx,cury);
                        curx++;
                        if (curx > maxx - 1)
                        {
                              curx = 0;
                              cury++;
                              if (cury > maxy - 1)
                              {
                                    scroll_up(0,0,maxx - 1,maxy - 1,curattr);
                                    cury--;
                              }
                        }
                        break;
                  }
                  break;

            case WASESCAPE:
                  if (*b == '[')
                  {
                        ansistate = WASBRKT;
                        arglen = 0;
                        *argbuf = 0;
                        break;
                  }
                  ansistate = NOTHING;
                  break;

            case WASBRKT:
                  if (strchr(ansi_terminators, (int)*b))
                  {
                        switch ((int)*b)
                        {
                        case 'H':   /* set cursor position */
                        case 'F':
                              set_pos(argbuf,arglen,*b);
                              break;

                        case 'A':   /* up */
                              go_up(argbuf,arglen,*b);
                              break;

                        case 'B':   /* down */
                              go_down(argbuf,arglen,*b);
                              break;

                        case 'C':   /* right */
                              go_right(argbuf,arglen,*b);
                              break;

                        case 'D':   /* left */
                              go_left(argbuf,arglen,*b);
                              break;

                        case 'n':   /* report pos */
                              report(argbuf,arglen,*b);
                              break;

                        case 's':   /* save pos */
                              save_pos(argbuf,arglen,*b);
                              break;

                        case 'u':   /* restore pos */
                              restore_pos(argbuf,arglen,*b);
                              break;

                        case 'J':   /* clear screen */
                              clear_screen(argbuf,arglen,*b);
                              break;

                        case 'K':   /* delete to eol */
                              kill_line(argbuf,arglen,*b);
                              break;

                        case 'm':   /* set video attribs */
                              set_colors(argbuf,arglen,*b);
                              break;

                        case 'p':   /* keyboard redef -- disallowed */
                              break;

                        default:    /* unsupported */
                              break;
                        }
                        ansistate = NOTHING;
                        arglen = 0;
                        *argbuf = 0;
                  }
                  else
                  {
                        if (arglen < MAXARGLEN)
                        {
                              argbuf[arglen] = *b;
                              argbuf[arglen + 1] = 0;
                              arglen++;
                        }
                  }
                  break;

            default:
                  pos_hardcursor(curx,cury);
                  fputs("\n **Error in ANSI state machine.\n",stderr);
                  break;
            }
            b++;
      }
      pos_hardcursor(curx,cury);
      hardcursor_on(curx,cury);

      return ((int)b - (int)buf);
}
