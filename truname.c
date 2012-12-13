/*
** tname.c -- wrapper for the undocumented DOS function TRUENAME
**
** TRUENAME: interrupt 0x21 function 0x60
**
**   Call with: ah    =  60h
**              es:di -> destination buffer
**              ds:si -> source buffer
**
**   Returns:   carry bit set if there were problems
**
** This code hereby contributed to the public domain.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include "dosfiles.h"
#include "snip_str.h"         /* For trim()     */

/*
** Truename itself. Note that I'm using intdosx() rather than
** playing with some inline assembler -- I've discovered some
** people that actually don't have an assembler, poor bastards :-)
*/

char *Truename(char *dst, char *src)
{
      union REGS rg;
      struct SREGS rs;

      if (!src || !*src || !dst)
            return NULL;

      src = trim(src);

      rg.h.ah = 0x60;
      rg.x.si = FP_OFF(src);
      rg.x.di = FP_OFF(dst);
      rs.ds   = FP_SEG(src);
      rs.es   = FP_SEG(dst);

      intdosx(&rg, &rg, &rs);

      return (rg.x.cflag) ? NULL : dst;
}

#ifdef TEST

/*
** ... and a little test function.
*/

int main(int argc, char *argv[])
{
      char buf[128]="                             ", *s;
      int i;

      if (3 > _osmajor)
      {
            puts("Only works with DOS 3+");
            return -1;
      }
      if(argc > 1)
      {
            for(i = 1; i < argc; i++)
            {
                  s = Truename((char *)buf,(char *)argv[i]);
                  printf("%s=%s\n",argv[i], s ? buf : "(null)");
            }
      }
      else  printf("Usage: TRUENAME [filename [filename...]]\n");

      return 0;
}

#endif /* TEST */
