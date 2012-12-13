/*
**  S O U N D E X 5
**
**  Originally from SNIPPETS, written by Bob Jarvis.
**  Modified by M. Stapleton of Graphic Bits on Dec  8 1994
**  Uses compressed heptal - eliminates codes for non-trailing zeros.
*/

#include <stdlib.h>
#include <ctype.h>
#include "phonetic.h"

int soundex5(char *instr)
{
      int count = 0, init, snd = 0, notlast = 1;
      int ch;
      static int table[] =
      {
      /* A  B  C  D  E     F  G  H  I  J */
         0, 1, 2, 3, 0,    1, 2, 0, 0, 2,

      /* K  L  M  N  O     P  Q  R  S  T */
         2, 4, 5, 5, 0,    1, 2, 6, 2, 3,

      /* U  V  W  X  Y  Z */
         0, 1, 0, 2, 0, 2,
      };

      /* Skip leading non-alpha */

      while(*instr && (!isalpha(*instr)))
            instr++;

      if(!*instr)     /* Hey!  Where'd the string go? */
            return -1;

      /* Convert initial letter to int. */

      init = (char)toupper(instr[0]) - 'A';

      for(instr++; *instr && (count < 3); instr++)
      {
            if(isalpha(*instr) && (*instr != *(instr-1)))
            {
                  if(NUL != (ch = table[toupper(*instr) - 'A']))
                  {
                        /* Convert to "compressed heptal" */

                        snd = (7 - notlast) * snd + ch - notlast;
                        notlast = ++count < 2;
                  }
            }
      }

      /* Adjust */

      switch(count)
      {
      case 0:     /* default:    Shouldn't get here! */
            snd = 0;
            break;

      case 1:
            snd += 1;
            break;

      case 2:
            snd *= 7;
            /* Fall through */

      case 3:
            snd += 7;
            break;
      }
      return SNDMAX * init + snd;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      if (argc != 2)
      {
            puts("Usage: SOUNDEX5 string");
            return EXIT_FAILURE;
      }

      printf("soundex5(\"%s\") returned %d\n", argv[1], soundex5(argv[1]));

      return EXIT_SUCCESS;
}

#endif /* TEST */
