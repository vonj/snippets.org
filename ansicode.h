#ifndef   ANSICODE__H
#define   ANSICODE__H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                     *
 *       ANSICODE.H                                                    *
 *                                                                     *
 *       Macro implementation of ANSI screen control codes             *
 *                                                                     *
 *       Donated to the public domain 96-11-12 by                      *
 *       Tom Torfs (tomtorfs@mail.dma.be, 2:292/516@fidonet)           *
 *                                                                     *
 *       Based on code originally contributed to the public domain on  *
 *       12-26-91 by by Matthew J. Glass.                              *
 *                                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include <stdio.h>

enum {ANSI_NORMAL=0,   ANSI_BOLD=1,     ANSI_UNDER=4,
      ANSI_BLINK=5,    ANSI_REVERSE=7,  ANSI_HIDDEN=8};

enum {ANSI_BLACK=0,    ANSI_RED=1,      ANSI_GREEN=2,
      ANSI_YELLOW=3,   ANSI_BROWN=3,    ANSI_BLUE=4,
      ANSI_MAGENTA=5,  ANSI_CYAN=6,     ANSI_WHITE=7};

enum {ANSI_40x25_BW=0,     ANSI_40x25=1,
      ANSI_80x25_BW=2,     ANSI_80x25=3,
      ANSI_320x200x4=4,    ANSI_320x200x2=5,
      ANSI_640x200x2=6,    ANSI_LINEWRAP=7,
      ANSI_320x200x16=13,  ANSI_640x200x16=14,
      ANSI_640x350x2=15,   ANSI_640x350x16=16,
      ANSI_640x480x2=17,   ANSI_640x480x16=18,
      ANSI_320x200x256=19};

#define ANSI_home()        printf("\x1B[H")
#define ANSI_locate(x,y)   printf("\x1B[%d;%dH",(int)(y),(int)(x))
#define ANSI_up(lines)     printf("\x1B[%dA",(int)(lines))
#define ANSI_down(lines)   printf("\x1B[%dB",(int)(lines))
#define ANSI_right(cols)   printf("\x1B[%dC",(int)(cols))
#define ANSI_left(cols)    printf("\x1B[%dD",(int)(cols))
#define ANSI_save()        printf("\x1B[s")
#define ANSI_restore()     printf("\x1B[u")
#define ANSI_cls()         printf("\x1B[2J")
#define ANSI_clreol()      printf("\x1B[K")
#define ANSI_attrib(attr)  printf("\x1B[%dm",(int)(attr))
#define ANSI_fgcolor(fc)   printf("\x1B[%dm",(int)(fc)+30)
#define ANSI_bgcolor(bc)   printf("\x1B[%dm",(int)(bc)+40)
#define ANSI_fullcolor(fc,bc) \
             printf("\x1B[%d;%dm",(int)(fc)+30,(int)(bc)+40)
#define ANSI_attrcolor(fc,bc,attr) \
             printf("\x1B[%d;%d;%dm",(int)(fc)+30,(int)(bc)+40,(int)(attr))
#define ANSI_videomode(nr) printf("\x1B[=%dh",(int)(nr))
#define ANSI_resetmode(nr) printf("\x1B[=%dl",(int)(nr))
#define ANSI_report()      printf("\x1B[6n")

#endif /* ANSICODE__H */
