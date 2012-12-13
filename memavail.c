/*
**  MEMAVAIL.C - Report available DOS memory
**
**  public domain by Thor Johnson
*/

#include <dos.h>
#include "snpdosys.h"

long memavail(void)
{
      union REGS regs;

      /* Request impossibly large number of 16-byte paragraphs from DOS */

      regs.h.ah = 0x48;
      regs.x.bx = 0xFFFF;

      int86(0x21,&regs,&regs);

      return((long)regs.x.bx * 16L);
}

#ifdef TEST

#include <stdio.h>

main()
{
      printf("Available DOS memory = %ld bytes\n", memavail());
      return 0;
}

#endif /* TEST */
