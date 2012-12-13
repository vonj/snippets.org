/*
**  PRTSTAT.C - Determine printer status
**
**  public domain by Bob Stout
*/

#include "sniprint.h"

#include <dos.h>

/*
**  prtstat() - Call with printer number (0 = LPT1, 1 = LPT2, 2 = LPT3)
**
**  Returns status which can be mapped to a PrStatus struct
*/

int prtstat(unsigned int printer_no)
{
      union REGS regs;

      regs.h.ah = 2;
      regs.x.dx = printer_no;
      int86(0x17, &regs, &regs);
      return regs.h.ah;
}

#ifdef TEST

#include <stdio.h>

#define show(x) printf(#x" is %strue (LPT1)\n", mystat.x ? "" : "not ");

main()
{
      struct PrStatus mystat;

      *((int *)&mystat) = prtstat(0);
      show(notbusy);
      show(selected);
      show(paperout);
      return 0;
}

#endif /* TEST */
