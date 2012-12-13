/*
**  KBTRAP.C - Traps Ctrl-Alt-Del, Ctrl-C, Ctrl-Break, SysRq, PrintScreen,
**             and Pause keys.
**
**  Derived from public domain sources, uses SNIPPETS header files
*/

#include <stdlib.h>
#include <dos.h>
#include "pchwio.h"
#include "sniptype.h"
#include "sniprint.h"

#define CTRLALT         (0x08|0x04) /* bit flags set in kbstat()        */
#define CTRL            (0x04)      /* bit flags set in kbstat()        */
#define ALT             (0x08)      /* bit flags set in kbstat()        */
#define DELSCAN         0x53        /* keyboard scan code for <Del>     */
#define CSCAN           0x2E        /* keyboard scan code for <C>       */
#define BREAKSCAN       0x46        /* keyboard scan code for <Break>   */
#define PRTSCRNSCAN     0x37        /* keyboard scan code for <PrtScrn> */
#define SYSREQSCAN      0x54        /* keyboard scan code for <SysReq>  */
#define MULTI2SCAN      0xE0        /* 1st byte of 2 byte scan code     */
#define MULTI3SCAN      0xE1        /* 1st byte of 3 byte scan code     */
#define KEYPORT         0x60        /* keyboard scan code port          */
#define CONTROLLERPORT  0x20        /* interrupt controller port        */
#define kbstat()        Peekw(0,0x417) /* BIOS data area - kb flags     */

#define keyport()      inp(KEYPORT)
            /* macro that returns the scancode of the key that caused   */
            /* the interrupt                                            */

#define install()      (oldkbisr=getvect(0x09),setvect(0x09,newkbisr))
            /* installation macro, installs newkbisr() in the keyboard  */
            /* interrupt chain                                          */

#define uninstall()       setvect(0x09,oldkbisr)
            /* removal macro, call to remove newkbisr() from interrupt  */
            /* chain.  oldkbisr()  must be removed before program ends  */

static void (INTERRUPT FAR * oldkbisr)(void);
            /* address of old keyboard ISR                              */

static Boolean_T PrtScrnPending = False_;
            /* semaphore to flag pending print screen operations        */

static void cleanup(void);
            /* atexit()-registered version of uninstall() macro         */

static Boolean_T KBtrap_active;
static Boolean_T KBtrap_installed = False_;

static void KBtrap(void);
static void cleanup(void);
static void INTERRUPT FAR newkbisr(void);


/*
**  Called to activate trapping the system keys. When called the first time,
**  it calls KBtrap() to install the ISR. Registers the ISR removal function
**  using atexit() so the ISR will always be uninstalled upon program
**  termination.
*/

void activate_KBtrap(void)
{
      if (!KBtrap_installed)
            KBtrap();
      KBtrap_active = True_;
#ifdef TEST
      puts("Keyboard trap activated");
#endif
}

/*
**  Called to deactivate trapping the system keys, but leave ISR installed.
**  If the user had attempted a PrintScreen operation while the trap was
**  active, it is now called.
*/

void deactivate_KBtrap(void)
{
      KBtrap_active = False_;
#ifdef TEST
      puts("Keyboard trap deactivated");
#endif
      if (PrtScrnPending)
      {
            if (Success_ == PrtScrn())
                  PrtScrnPending = False_;
      }
}

/*
**  Traps system keys - Ignores Ctrl-Alt-Del, Ctrl-C, Ctrl-Break, SysRq,
**                      and Pause.
**
**                      Defers execution of PrintScreen
*/

static void INTERRUPT FAR newkbisr(void)
{
      static int count = 0;
      int key = keyport();

      if (KBtrap_active)
      {
            if (count)
            {
                  unsigned char kbin;

                  --count;

                  if (PRTSCRNSCAN == key)
                        PrtScrnPending = True_;
IGNORE:
                  kbin = (unsigned char)inp(KEYPORT+1); /* reset keyboard  */
                  outp(KEYPORT+1, kbin|0x80);
                  outp(KEYPORT+1, kbin);
                  disable();
                  outp(CONTROLLERPORT,0x20); /* tell controller to shut up */
                  enable();
                  return;
            }
            else switch (key)
            {
            case MULTI2SCAN:
                  count = 1;
                  goto IGNORE;

            case MULTI3SCAN:
                  count = 2;
                  goto IGNORE;

            case DELSCAN:
                  if (CTRLALT == (kbstat() & CTRLALT))
                        goto IGNORE;
                  break;
                  
            case PRTSCRNSCAN:
                  PrtScrnPending = True_;
                  goto IGNORE;
                  
            case CSCAN:
            case BREAKSCAN:
                  if (CTRL == (kbstat() & CTRL))
                        goto IGNORE;
                  break;
                  
            case SYSREQSCAN:
                  goto IGNORE;
            }
      }
      else
      {
            if (count)
            {
                  --count;
                  return;
            }
      }
      oldkbisr();                         /* chain to old keyboard isr */
}

static void KBtrap(void)
{
      install();
      atexit(cleanup);
      KBtrap_installed = True_;
#ifdef TEST
      puts("Keyboard trap installed");
#endif
}

static void cleanup(void)
{
      uninstall();
#ifdef TEST
      puts("Keyboard trap uninstalled");
#endif
}

#ifdef TEST

#include <stdio.h>
#include <conio.h>
#include <signal.h>
#include <errno.h>

main()
{
      int ch = 0;

      puts("This is a test of Ctrl-Alt-Del disabling.");
      puts("Press any key, but only Esc should stop this program.");
      
      activate_KBtrap();

      while (0x1b != ch)
      {
            if (kbhit())
            {
                  ch = getch();
                  printf("key value = %02Xh, PrtScrnPending = %d\n",
                         ch, PrtScrnPending);
            }
      }
      
      deactivate_KBtrap();

      return EXIT_SUCCESS;
}

#endif /* TEST */
