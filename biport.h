/*
**  BIPORT.H - Port TC/TC++/BC++ code using register pseudovariables
**
**  public domain by Bob Stout
*/

#ifndef __TURBOC__

#ifndef BIPORT__H
#define BIPORT__H

#include <dos.h>
#include "dirport.h"

extern union  REGS  BIP_regs_;
extern struct SREGS BIP_sregs_;

#define _AX BIP_regs_.x.ax
#define _BX BIP_regs_.x.bx
#define _CX BIP_regs_.x.cx
#define _DX BIP_regs_.x.dx
#define _AH BIP_regs_.h.ah
#define _AL BIP_regs_.h.al
#define _BH BIP_regs_.h.ah
#define _BL BIP_regs_.h.al
#define _CH BIP_regs_.h.ah
#define _CL BIP_regs_.h.al
#define _DH BIP_regs_.h.ah
#define _DL BIP_regs_.h.al
#define _SI BIP_regs_.x.si
#define _DI BIP_regs_.x.di
#define _CF BIP_regs_.x.cflag
#define _FF BIP_regs_.x.flags
#define _ES BIP_sregs_.es
#define _CS BIP_sregs_.cs
#define _SS BIP_sregs_.ss
#define _DS BIP_sregs_.ds
#define regload_() segread(&BIP_sregs_)

#ifndef geninterrupt
 unsigned PASCAL geninterrupt(int);
#endif

#endif /* BIPORT__H */

#endif /* __TURBOC__ */
