/*
**  ext_getch()
**
**  A getch() work-alike for use with extended keyboards.
**
**  Parameters: none
**
**  Returns: Extended key code as follows:
**           0->255     Normal key
**           256->511   Numeric pad key or Function key
**           512->767   Cursor pad key or Numeric pad
**                      "duplicate" key (Enter, /, *, -, +)
**
**  Original Copyright 1992 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Revisions:
**  30-Mar-96  Ed Blackman  OS/2 mods
*/

#include <dos.h>
#include <ctype.h>
#include "hilobyte.h"
#include "snipkbio.h"
#include "ext_keys.h"

#define USING_DOS 0     /* Set to 1 to call DOS instead of the BIOS     */

int ext_getch(void)
{
      int key;
#ifdef __OS2__
      extern KBDKEYINFO ki;         /* defined in ISSHIFT.C */
      KBDINFO kb_state;

      kb_state = setkbmode();       /* Change keyboard to binary mode */
      KbdCharIn(&ki, IO_WAIT, 0);   /* Get the key */
      restkbmode(kb_state);         /* restore previous keyboard mode */

      key = (ki.chScan << 8) + ki.chChar;       /* format it into an int */
#else                       /* assume DOS */
      union REGS regs;

 #if USING_DOS
      regs.h.ah = 7;
      intdos(&regs, &regs);
      key = regs.h.al;
      if (0 == key)
      {
            regs.h.ah = 7;
            intdos(&regs, &regs);
            key = (regs.h.al << 8);
      }
 #else
      regs.h.ah = 0x10;
      int86(0x16, &regs, &regs);
      key = regs.x.ax;
 #endif

      switch (LoByte(key))
      {
      case 0:
            key = HiByte(key) + 256;
            break;

      case 0xe0:
            key = HiByte(key) + 512;
            break;

      default:
            if (0xe0 == HiByte(key))
                  key = LoByte(key) + 512;
            else
            {
                  if (ispunct(LoByte(key)) && HiByte(key) > 0x36)
                        key = LoByte(key) + 512;
                  else  key = LoByte(key);
            }
      }
#endif
      return key;
}

int GetExtKey(int *isshift)
{
      int key =  ext_getch();

      *isshift = IsShift();
      return key;
}

#ifdef __OS2__
KBDINFO setkbmode(void)
{
    USHORT rc;
    KBDINFO kb_state;

    kb_state.cb = sizeof(kb_state);
    KbdGetStatus(&kb_state, 0);
    kb_state.fsMask &= ~KEYBOARD_ASCII_MODE;
    kb_state.fsMask |= KEYBOARD_BINARY_MODE;
    rc = KbdSetStatus(&kb_state, 0);

/*  if(rc) printf("KbdSetStatus rc = %04x\n", rc);  */

    return kb_state;
}

void restkbmode(KBDINFO kb_state)    /* restore keyboard mode */
{
    USHORT rc;

    rc = KbdSetStatus(&kb_state, 0);
}
#endif      /* !__OS2__ */

#ifdef TEST

#include <stdio.h>

main()
{
      int key0, key1, shift;

      puts("Hit keys, Esc twice to stop\n");
      for ( key0 = key1 = 0; !(key1 == Key_ESC && key0 == Key_ESC); )
      {
            key0 = key1;
            key1 = GetExtKey(&shift);
            printf("ext_getch() returned %0Xh, Shift is O%s\n",
                  key1, shift ? "n" : "ff");
      }
      return 0;
}

#endif /* TEST */
