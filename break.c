/*
**  Set or determine the status of the DOS "SET BREAK=" command
*/

#include <dos.h>
#include "snpdosys.h"

/*
**  Returns status of DOS "SET BREAK" command
*/

int isBreakOn(void)
{
      union REGS regs;

      regs.x.ax = 0x3300;
      intdos(&regs, &regs);
      return (int)regs.h.dl;
}

void setBreak(int OnOff)      /* Off = 0, On = 1      */
{
      union REGS regs;

      regs.x.ax = 0x3301;
      regs.h.dl = OnOff;
      intdos(&regs, &regs);
}
