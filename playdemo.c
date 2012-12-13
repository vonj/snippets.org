/*
**  PLAYDEMO.C - demo of background music functions
**
**  public domain by Bob Stout
*/

#include <stdio.h>
#include "uclock.h"
#include "sound.h"

main()
{
      int i;

      if (!playb_open(1024))
      {
            puts("\aError opening play buffer");
            return -1;
      }
      playb_note(C3, 8);
      playb_note(REST, 2);
      playb_note(C3, 8);
      playb_note(REST, 2);
      playb_note(E3, 8);
      playb_note(REST, 2);
      playb_note(E3, 8);
      playb_note(REST, 2);
      playb_note(G3, 8);
      playb_note(REST, 2);
      playb_note(G3, 8);
      playb_note(REST, 2);
      playb_note(E3, 18);
      playb_note(REST, 2);
      for (i = 0; i < 500; ++i)
            printf("i = %d\n", i);
      playb_close();
      return 0;
}
