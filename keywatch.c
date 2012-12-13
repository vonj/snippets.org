/* keywatch.c 12-29-91 Robert Mashlan, Public Domain                *\

   DOS compiler portability modifications added by Bob Stout,
   1:106/2000.6

   This program monitors the keyboard interrupt, and stores the
   status of each key as to whether it is pressed or released.

   This is done by capturing interrupt 9, and watching the make/break
   codes.  The status is updated in the keys array, where 1 means
   that the key is pressed, while 0 means the key is released.  The
   key array is uses the scan code for an index instead of the ascii
   character.  It is simple enough to find the scan code for a key,
   just run this program and watch the display.

   The ekeys array will reflect the status of keys found on an AT
   keyboard.  For instance, the left and right alt keys are
   differentiated, as well as the edit control keys on the numeric
   keypad and the one not on the numeric keypad.

   Since this program installs an interrupt handler, it should be
   terminated normally, such the keyboard handler can be removed.
   The ^C/^Break exit is captured via signal(), but all possible
   exits should be trapped.

\*                                                                  */

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <signal.h>
#include "sniptype.h"
#include "pchwio.h"

volatile char keys[128];           /* array of key states           */
volatile char ekeys[128];          /* array of AT key states        */

#define KEYPORT        0x60        /* keyboard scan code port       */
#define keyport()      inp(KEYPORT)
        /* macro that returns the scancode of the key that caused   */
        /* the interrupt                                            */

/* Define:                                                          *\

   installisr()
         installation macro, installs newkbisr() in the keyboard
         interrupt chain

   removeisr()
         removal macro, call to remove newkbisr() from interrupt
         chain.  oldkbisr()  must be removed before program ends
\*                                                                  */

#ifdef __ZTC__
 #define installisr() int_intercept(0x09, newkbisr, 0)
 #define removeisr()  int_restore(0x09);
#else
 #define installisr()  (oldkbisr=getvect(0x09),setvect(0x09,newkbisr))
 #define removeisr()   setvect(0x09,oldkbisr)
 #ifdef __TURBOC__
  void INTERRUPT (FAR *oldkbisr)(void);    /* address of old ISR   */
 #else
  void (INTERRUPT FAR *oldkbisr)(void);    /* address of old ISR   */
 #endif
#endif

#if defined(__ZTC__)
 int newkbisr(struct INT_DATA *pd)
#elif defined(__TURBOC__)
 void INTERRUPT newkbisr(void)
#else
 void INTERRUPT FAR newkbisr(void)
#endif
{
      static extkey;
      BYTE scancode = (BYTE)keyport();         /* read keyboard scan code */

      if (scancode == 0xe0)
            extkey = 1;               /* use ekey array on next scan code */
      else
      {
            if (scancode & 0x80)                          /* key released */
                  (extkey ? ekeys : keys)[scancode & 0x7f] = 0;
            else  (extkey ? ekeys : keys)[scancode] = 1;
            extkey = 0;
      }

#ifdef __ZTC__
      return 0;                         /* chain to previous keyboard ISR */
#else
      oldkbisr();                       /* chain to previous keyboard ISR */
#endif
}

int keyspressed(void)           /* returns number of keys being held down */
{
      int i, result = 0;

      for (i = 0; i < 128; i++)
      {
            result += keys[i];
            result += ekeys[i];
      }
      return result;
}

int main(void)
{
      int lastkeycount = 0;

      signal(SIGINT,SIG_IGN);  /* ignore ^C and ^Break */
      installisr();            /* install interrupt handler */
      while(1)
      {
            int i;

            if (keyspressed() != lastkeycount) /* change in keystatus */
            {
                  lastkeycount = keyspressed();
                  puts("---");
                  for(i = 0; i < 128; i++)
                  {
                        if (keys[i])
                              printf("key with scan code %02x "
                                    "has been pressed\n", i);
                        if (ekeys[i])
                              printf("key with scan codes e0 %02x "
                                    "had been pressed\n", i);
                  }
            }
            if (kbhit() && getch()==0x1b) /* terminate when Esc pressed */
                  break;
      }
      removeisr();   /* remove interrupt handler */
      return 0;
}
