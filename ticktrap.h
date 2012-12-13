/*
**  TICKTRAP.H - Trap the PC timer interrupt.
**
**  public domain by Bob Stout
*/

#ifndef TICKTRAP__H
#define TICKTRAP__H

#include <stdlib.h>
#include "extkword.h"

#ifdef __IS_ZORTECH__
 #include <int.h>
#else
 #include "pchwio.h"
#endif

void ticktrap(int (*fptr)(void));
void tickchain(void);

#endif /* TICKTRAP__H */
