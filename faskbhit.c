/*
**  by David Goodenough & Bob Stout
**
**  Revisions:
**  30-Mar-96  Ed Blackman  OS/2 mods
*/

#include <stdio.h>
#include <dos.h>
#include "snipkbio.h"
#include "extkword.h"
#include "ext_keys.h"
#include "mk_fp.h"

#if defined(__OS2__)

/* 30-Mar-96 - EBB:  it really isn't good to poll the keyboard in OS/2.
** A better design would be to call KbdCharIn(..., IO_WAIT, ...) in a
** separate thread, and sending a message via some form of IPC to the main
** thread when a key is available.  This code is intended to be more of an
** example of how to use the API, rather than something you would copy into
** production code.
*/

int fast_kbhit_os2(void)
{
    extern KBDKEYINFO ki;         /* defined in ISSHIFT.C */

    KbdPeek(&ki, 0);              /* peek in the keyboard buffer */
    DosSleep(1);                  /* give up the rest of the time slice */

    return (ki.fbStatus & (1 << 6));  /* only return true if key is 'final' */
}

void fast_kbflush_os2(void)
{
    KbdFlushBuffer(0);
}

#else   /* assume DOS */
 #define biosseg 0x40

 #define HEAD (*((unsigned short FAR *)MK_FP(biosseg, 0x1a)))
 #define TAIL (*((unsigned short FAR *)MK_FP(biosseg, 0x1c)))

/*
**  Detect a pending keypress
*/

int fast_kbhit_dos(void)
{
      int retval;

      disable();
      retval = HEAD - TAIL;
      enable();
      return retval;
}

/*
**  Clear the keyboard buffer
*/

void fast_kbflush_dos(void)
{
      disable();
      HEAD = TAIL;
      enable();
}
#endif  /* !__OS2__ */

/*
**  Enhanced work-alike for BASIC's INKEY$ function
*/

int ext_inkey(void)
{
      if (!fast_kbhit())
            return EOF;
      else  return ext_getch();
}

#ifdef TEST

#include <conio.h>
#include <time.h>

main()
{
      clock_t Wait;
      int key;

      puts("Hit some keys while I kill some time...");
      Wait = clock();
      while (2 > ((clock() - Wait) / CLK_TCK))
            ;

      puts("OK, stop hitting keys while I flush the keyboard...");
      Wait = clock();
      while (2 > ((clock() - Wait) / CLK_TCK))
            ;

      fast_kbflush();
      puts("Optionally, hit some key you didn't hit before...");
      Wait = clock();
      while (2 > ((clock() - Wait) / CLK_TCK))
            ;
      if (EOF == (key = ext_inkey()))
            puts("You didn't hit anything");
      else  printf("You hit extended keycode 0x%04X\n", key);
      
      puts("OK, now hit some other key you didn't hit before...");

      while (!fast_kbhit())
            ;
      printf("You hit extended keycode 0x%04X\n", ext_getch());
      return 0;
}

#endif /* TEST */
