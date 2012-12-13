/*
**  MKTONE.C
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <stddef.h>
#include "uclock.h"
#include "sound.h"
#include "pchwio.h"


void dosound(int freq)
{
      unsigned i;

      outp(C8253, SETIMER);
      i = (unsigned)freq%256;
      outp(F8253, i);
      i = (unsigned)freq/256;
      outp(F8253, i);
}

void mktone(int freq, int update, unsigned delay)
{
      if (0 == freq)
      {
            soundoff();
            return;
      }
      dosound(freq);
      if (update != UPDATE)
            soundon();
      if (delay == 0)
            return;
      else  usec_delay(1000L * (unsigned long)delay);
      if (update == TOGGLE)
            soundoff();
}

#ifdef TEST

#include <stdio.h>

main()
{
      puts("Playing A2 for 1 sec.");
      mktone(A2, ON, 1000);
      puts("Playing A3 for 1 sec.");
      mktone(A3, UPDATE, 1000);
      mktone(0,  UPDATE, 0);
      return 0;
}

#endif /* TEST */
