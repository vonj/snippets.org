/*
**  TICKTRAP.C - Trap the PC timer interrupt.
**
**  public domain by Bob Stout
*/

#include <dos.h>
#include "ticktrap.h"

static int (*newptr)(void);
static void endtrap(void);

#if defined(__ZTC__) || defined(__SC__)

static int do_new(struct INT_DATA *idp);
static struct INT_DATA idp;

/*
**  Installs a user-defined funtion in an Int 1Ch ISR. The user-supplied
**  function should take no arguments and should return an int. If zero
**  is returned from the user-supplied function, the previous Int 1Ch ISR
**  is called upon exit. If non-zero is returned from the user-supplied
**  function, the previous ISR is not called.
**
**  Side effect: Registers endtrap() with atexit() to restore Int 1Ch
**               upon program termination.
*/

void ticktrap(int (*fptr)(void))
{
      newptr = fptr;
      int_intercept(0x1c, do_new, 0);
      atexit(endtrap);
}

/*
**  Calls the previous Int 1Ch ISR
*/

void tickchain(void)
{
      int_prev(&idp);
}

static int do_new(struct INT_DATA *idp)
{
      return (*newptr)();
}

static void endtrap(void)
{
      int_restore(0x1c);
}

#else /* It's not Symantec or Zortech */

static void (INTERRUPT FAR *old1c)(void);

static void INTERRUPT FAR do_new(void);

/*
**  Installs a user-defined funtion in an Int 1Ch ISR. The user-supplied
**  function should take no arguments and should return an int. If zero
**  is returned from the user-supplied function, the previous Int 1Ch ISR
**  is called upon exit. If non-zero is returned from the user-supplied
**  function, the previous ISR is not called.
**
**  Side effect: Registers endtrap() with atexit() to restore Int 1Ch
**               upon program termination.
*/

void ticktrap(int (*fptr)(void))
{
      old1c = _dos_getvect(0x1c);
      newptr = fptr;
      disable();
      _dos_setvect(0x1c, do_new);
      enable();
      atexit(endtrap);
}

/*
**  Calls the previous Int 1Ch ISR
*/

void tickchain(void)
{
      (*old1c)();
}

static void INTERRUPT FAR do_new(void)
{
      int retval;
      retval = (*newptr)();
      if (0 == retval)
            (*old1c)();
}

static void endtrap(void)
{
      disable();
      _dos_setvect(0x1c, old1c);
      enable();
}

#endif
