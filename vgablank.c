/*
**  Functions to blank and resore EGA/VGA screens
*/

#include <dos.h>
#include "pchwio.h"
#include "vgablank.h"

void blank_EVGA(void)
{
	disable();
	inp(0x3da);
	outp(0x3c0, 0);
	enable();
}

void restore_EVGA(void)
{
	disable();
	inp(0x3da);
	outp(0x3c0, 0x20);
	enable();
}

#ifdef TEST

#include "delay.h"      /* From SNIPPETS - use dos.h if your compiler's
                           standard library supports delay().           */
main()
{
      blank_EVGA();     /* Blank the screen...  */
      delay(1500);      /* ...delay 1.5 secs... */
      restore_EVGA();   /* ...& restore it      */
      return 0;
}

#endif /* TEST */
