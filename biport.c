/*
**  BIPORT.C - Port TC/TC++/BC++ code using register pseudovariables
**
**  public domain by Bob Stout
*/

#ifndef __TURBOC__

#include "biport.h"

union  REGS  BIP_regs_;
struct SREGS BIP_sregs_;

#ifndef geninterrupt

unsigned PASCAL geninterrupt(int int_no)
{
      int86x(int_no, &BIP_regs_, &BIP_regs_, &BIP_sregs_);
      return BIP_regs_.x.ax;
}

#endif /* geninterrupt() */

#endif /* __TURBOC__ */
