/* prnspool.c 12-22-91 Robert Mashlan, public domain */
/* DOS print spooler interface functions             */

#include <stdio.h>
#include <dos.h>
#include "prnspool.h"

int printspool_errno = 0;

char *printspool_errlist[] = {
      "No error",
      "Function Invalid",
      "File not found",
      "Path not found",
      "Too many open files",
      "Access denied",
      "",
      "",
      "Queue full",
      "Spooler busy",
      "",
      "",
      "Name too long",
      "",
      "Drive invalid"
      };

/* returns -1 if printspooler installed */
/* 0 otherwise                          */

int printspool_installed(void)
{
      union REGS r;

      r.x.ax = 0x0100;
      int86(0x2f, &r, &r);
      if(r.h.al==0xff)
      {
            printspool_errno=0;
            return -1;
      }
      else
      {
            printspool_errno=1;
            return 0;
      }
}

/* submits a file name to be printed */
/* returns error code                */

int printspool_submit( const char *pathname )
{
      struct PACKET packet;
      union REGS r;
      struct SREGS s;

      packet.level = 0;
      packet.pathname = (char FAR *)pathname;
      s.ds   = FP_SEG(&packet);
      r.x.dx = FP_OFF(&packet);
      r.x.ax = 0x0101;
      int86x(0x2f, &r, &r, &s);
      if(!r.x.cflag)
            return printspool_errno = 0;
      else  return printspool_errno = r.x.ax;
}

/* removes a file from the print queue */

int printspool_remove( const char FAR *fname )
{
      union REGS r;
      struct SREGS s;

      s.ds   = FP_SEG(fname);
      r.x.dx = FP_OFF(fname);
      r.x.ax = 0x0102;
      int86x(0x2f, &r, &r, &s);
      if(!r.x.cflag)
            return printspool_errno = 0;
      else  return printspool_errno=r.x.ax;
}

/* cancels all files in the print queue */

int printspool_cancel(void)
{
      union REGS r;

      r.x.ax = 0x0103;
      int86(0x2f, &r, &r);
      if(!r.x.cflag)
            return printspool_errno = 0;
      else  return printspool_errno = r.x.ax;
}

/* ends hold state after a call to printspool_getqueue */
/* or printspool_errorcount                            */

void printspool_endhold(void)
{
      union REGS r;

      r.x.ax = 0x0105;
      int86(0x2f, &r, &r);
}

/* returns a far pointer to the printspooler queue, */
/* an array of 64 char asciiz strings               */

char FAR *printspool_getqueue(void)
{
      char FAR *result;
      union REGS r;
      struct SREGS s;

      r.x.ax = 0x0104;
      int86x(0x2f, &r, &r, &s);
      result = MK_FP(s.ds,r.x.si);
      if (!r.x.cflag)
      {
            printspool_errno = 0;
            return result;
      }
      else
      {
            printspool_errno = r.x.ax;
            return NULL;
      }
}

/* returns the error count from the printspooler */

int printspool_errorcount(void)
{
      union REGS r;

      r.x.ax = 0x0104;
      int86(0x2f, &r, &r);
      if (!r.x.cflag)
      {
            printspool_errno = 0;
            return r.x.dx;          /* return the number of errors */
      }
      else
      {
            printspool_errno = r.x.ax;
            return r.x.dx;
      }
}
