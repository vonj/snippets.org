/*
**  by: Dan Kozak
**  Revisions:
**  30-Mar-96  Ed Blackman  OS/2 mods
*/

/*
**  For use with your code, strip out the demo main() and make this into
**  a header file.
*/

#ifndef KB_DATA__H
#define KB_DATA__H

#include "extkword.h"
#include "snipkbio.h"

typedef struct                            /* Keyboard status structure  */
{
  unsigned short                          /* Least Significant Bit      */
  right_shift_down  : 1,                  /* Right Shift key depressed  */ 
  left_shift_down   : 1,                  /* Left Shift key depressed   */
  ctrl_down         : 1,                  /* Ctrl key depressed         */
  alt_down          : 1,                  /* Alt key depressed          */
  scroll_on         : 1,                  /* Scroll Lock is on          */
  num_on            : 1,                  /* Num Lock is on             */
  caps_on           : 1,                  /* Caps Lock is on            */
  ins_on            : 1,                  /* Insert state is active     */
  left_ctl          : 1,                  /* Left Ctl key depressed     */
  left_alt          : 1,                  /* Left Alt key depressed     */
#if defined (__OS2__)
  right_ctl         : 1,                  /* Right Ctl key depressed    */
  right_alt         : 1,                  /* Right Alt key depressed    */
#else       /* assume DOS */
  sys_rq            : 1,                  /* SysRq depressed            */
  pause_on          : 1,                  /* Pause is active            */
#endif
  scroll_down       : 1,                  /* Scroll Lock key depressed  */
  num_down          : 1,                  /* Num Lock key depressed     */
  caps_down         : 1,                  /* Caps Lock key depressed    */
#if defined (__OS2__)
  sys_rq            : 1,                  /* SysRq depressed            */
#else       /* assume DOS */
  ins_down          : 1;                  /* Insert key depressed       */
#endif
} biosshiftstate;                         /* Most Significant Bit       */

biosshiftstate FAR * volatile kbd_status =
      (biosshiftstate FAR * volatile)(peekkey());

#ifdef TEST

#include <stdio.h>
#include <time.h>

main()
{
      clock_t start = clock();

      puts("Press some key stuff and I'll tell you what in 3 seconds...\n");

      while (((clock() - start) / CLOCKS_PER_SEC) < 3)
#if defined(__OS2__)
          /* to store key info where peekkey() can find it */
            if(kbhit()) ext_getch()
#endif
            ;


      printf("right_shift_down = %d\n",kbd_status->right_shift_down);
      printf("left_shift_down = %d\n",kbd_status->left_shift_down);
      printf("ctrl_down = %d\n",kbd_status->ctrl_down);
      printf("alt_down = %d\n",kbd_status->alt_down);
      printf("scroll_on = %d\n",kbd_status->scroll_on);
      printf("num_on = %d\n",kbd_status->num_on);
      printf("caps_on = %d\n",kbd_status->caps_on);
      printf("ins_on = %d\n",kbd_status->ins_on);
      printf("filler = %d\n",kbd_status->filler);
      printf("ctrl_numloc = %d\n",kbd_status->ctrl_numloc);
      printf("scroll_down = %d\n",kbd_status->scroll_down);
      printf("num_down = %d\n",kbd_status->num_down);
      printf("caps_down = %d\n",kbd_status->caps_down);
      printf("ins_down = %d\n",kbd_status->ins_down);

      return 0;
}

#endif /* TEST */

#endif /* KB_DATA__H */
