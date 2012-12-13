/*
**  DELAY.H - SNIPPETS header file for DELAY.C
*/

#ifndef DELAY__H
#define DELAY__H

#if defined(__WATCOMC__) || (defined(__TURBOC__) && \
      (!defined(_Windows)) || defined(__DPMI16__) || defined(__DPMI32__))
 #include <dos.h>
#else
 void delay(unsigned short msec);
#endif

#endif /* DELAY__H */
