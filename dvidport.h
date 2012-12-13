/*
**  DVIDPORT.H - Direct video portability functions for PC compilers
**
**  For ease of use, makes everything look like Borland, but adds dvid_open()
**  and dvid_close() for Symantec & Zortech
**
**  public domain by Dave Knapp & Bob Stout
*/

#ifndef DVIDPORT__H
#define DVIDPORT__H

#include <stdio.h>
#include <conio.h>
#include <dos.h>

#ifdef M_I86      /* Identifier for MSC, QC, Watcom, or SC/ZTC    */
 #ifndef __ZTC__
  #include <graph.h>
  #define dvid_open()
  #define dvid_close()
  #define cputs(s) _outtext((char _far *)(s))
  #define gotoxy(col,row) _settextposition(row,col)
 #else   /* if SC/ZTC   */
  #include <disp.h>
  void gotoxy(int, int);
  #define dvid_open  disp_open
  #define dvid_close  disp_close
  #define cputs(s) disp_puts(s)
  #define cprintf(s) disp_printf(s)
 #endif  /* if ZTC      */
#else
 #define dvid_open()
 #define dvid_close()
 #if defined(__POWERC)
  #define gotoxy(col,row) poscurs(row,col)
 #endif
#endif

#endif /* DVIDPORT__H */
