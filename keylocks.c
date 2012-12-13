#include "extkword.h"
#include "snipkbio.h"

static volatile unsigned char FAR *keyflags =
      (unsigned char FAR *)0x00400017L;

/*
**  Caps Lock
*/

void setcaps(void)
{
      *keyflags |= 0x40;
}

void clrcaps(void)
{
      *keyflags &= ~0x40;
}

/*
**  Num Lock
*/

void setnumlock(void)
{
      *keyflags |= 0x20;
}

void clrnumlock(void)
{
      *keyflags &= ~0x20;
}

/*
**  Scroll Lock
*/

void setscrlock(void)
{
      *keyflags |= 0x10;
}

void clrscrlock(void)
{
      *keyflags &= ~0x10;
}
