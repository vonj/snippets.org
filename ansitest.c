/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                     *
 *       ANSITEST.C                                                    *
 *                                                                     *
 *       Demonstration of ANSICODE.H (includes ANSI detection)         *
 *                                                                     *
 *       Donated to the public domain 96-11-12 by                      *
 *       Tom Torfs (tomtorfs@www.dma.be, 2:292/516@fidonet)            *
 *                                                                     *
 *       The ANSI detection in this module is not the usual method,    *
 *       but unlike most of the other methods it allows detection      *
 *       through a serial connection etc. (e.g. BBS software)          *
 *                                                                     *
 *       This module requires DOSGETCH.ASM.                            *
 *                                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#include "ansicode.h"
#include "dosgetch.h"

int main(void)
{
   int ch;
   int x,y;
   clock_t waitclock;
   
   ANSI_report();
   
   waitclock = clock() + CLK_TCK; /* wait at most 1 second */
   do
      ch = dosgetch();
   while (ch==EOF && clock()<waitclock);
   
   if (ch!=27 || scanf("[%d;%dR",&y,&x)!=2)
   {
      printf("No ANSI screen driver detected.\n");
      return 1;
   }

   ANSI_attrcolor(ANSI_WHITE,ANSI_BLUE,ANSI_BOLD);
   ANSI_cls();
   
   ANSI_fgcolor(ANSI_RED);
   printf("Very primitive ANSI screen I/O demonstration\n\n");

   ANSI_fgcolor(ANSI_GREEN);
   printf("The cursor position at startup was %d,%d.\n",x,y);
   
   ANSI_fgcolor(ANSI_WHITE);
   ANSI_locate(1,12);
   printf("On a regular screen this should be about the middle.\n");
   
   ANSI_down(2);

   printf("And this is a few lines lower.\n");

   ANSI_locate(1,25);
   ANSI_attrib(ANSI_BLINK);
   printf("Press any key to continue..."); /* no \n, don't want to scroll */

   doswaitkey();
   
   ANSI_videomode(ANSI_640x350x2);

   ANSI_attrcolor(ANSI_WHITE,ANSI_BLACK,ANSI_NORMAL);
   ANSI_cls();

   printf("You like EGA 640x350x2 graphics mode ?\n\n");

   printf("If not, press a key...\n");
   
   doswaitkey();
   
   ANSI_videomode(ANSI_80x25);
   
   ANSI_attrcolor(ANSI_WHITE,ANSI_RED,ANSI_BOLD);
   ANSI_cls();

   printf("Aah that's better...\n\n");
   
   printf("Press any key to exit...\n");
   
   doswaitkey();

   ANSI_attrcolor(ANSI_WHITE,ANSI_BLACK,ANSI_NORMAL);
   ANSI_cls();
   
   return 0;
}
