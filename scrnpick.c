/* program:    mousword.c
 * programmer: Ray L. McVay
 * date:       20 Oct 1988
 * modified:   15 Feb 93 by Bob Stout to use Bob Jarvis' MOUSE.H and MOUSE.C
 *
 * Demonstration of picking "words" off a text mode PC screen using a mouse.
 * Submitted to the C_ECHO and placed in the public domain, 7 Jun 1992.
 */

#include <stdio.h>
#include <stdlib.h>
#include "mouse.h"
#include "scrnmacs.h"

char word[80];
unsigned FAR *scrn;

void getword(char *, int, int);

main(void)
{
      int done, b, x, y;

      scrn = SCRBUFF;
      ms_reset(&b); /* reset */
      ms_show_cursor();
      for (done = 0; !done; )
      {
            b = ms_get_mouse_pos(&x, &y);
            if (b == 1)
            {
                  ms_hide_cursor();
                  getword(word, x/8, y/8);
                  do
                  {
                        b = ms_get_mouse_pos(&x, &y);
                  } while (b);
                  if (*word)
                        printf("{%s}\n", word);
                  ms_show_cursor();
            }
            else if (b > 1)
                  done = 1;
      }
      ms_reset(&b);
      return 0;
}

void getword(char *w, int x, int y)
{
      int txs, txe, ci;
    
      for (txs = x; (txs >= 0) && ((scrn[80 * y + txs] & 255) != 32); txs--)
            scrn[80 * y + txs] = (scrn[80 * y + txs] & 255) | 0x7000;
      for (txe = x; (txe < 80) && ((scrn[80 * y + txe] & 255) != 32); txe++)
            scrn[80 * y + txe] = (scrn[80 * y + txe] & 255) | 0x7000;
      for (ci = txs + 1; ci < txe; ci++)
            *w++ = (char)scrn[80 * y + ci];
      *w = 0;
}
