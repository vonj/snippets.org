/*
**  SOUND.C
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include "pchwio.h"
#include "sound.h"

void soundon(void)
{
      short value;

      value = inp(SCNTRL);
      value |= SOUNDON;
      outp(SCNTRL, value);
}

void soundoff(void)
{
      short value;

      value = inp(SCNTRL);
      value &= SOUNDOFF;
      outp(SCNTRL, value);
}
