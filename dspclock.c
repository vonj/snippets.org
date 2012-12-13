/* public domain TSR clock code. By Michelangelo Jones, 1:141/575 or
// 1:1/124. Very little support available; this is a quick hack, not
// an example of the best way to write a clock TSR! Use at own risk.
// Runs under TC++/BC++. Your mileage may vary.
*/

#ifndef __HUGE__
#error "Must be compiled using HUGE model-- for now"
#endif

#include <dos.h>

extern unsigned _heaplen = 0;
extern unsigned _stklen = 512;

static char * screenbase =
           (char *)                  MK_FP (0xb800, 0); /* change for mono */
static const long int * volatile ticks =                /* to 0xb000, 0 */
           (long int * volatile)     MK_FP (0, 0x046c);

int calls, lastsec, lastmin, lasthr;
const double tps = 18.20648;          /* found by experimentation! */

void interrupt (*oldhandler)(void);

void displayclock (void)
{
    char *moveinto = screenbase + 300;
    char *initwith = "      :     :       ";

/* NOTE: This initializer only works because the attribute I want for the
//       clock HAPPENS to be the same as the ASCII value for the SPACE char!
//       Modify every alternate character if you want some other attribute.
*/

    while (*initwith)
        *moveinto++ = *initwith++;
    lastsec = -1;
    lastmin = -1;
    lasthr  = -1;
    calls   = 20;
}

void interrupt clockproc(void)
{
    static long seconds;

    if (calls < 17)
        calls++;
    else
    {
        seconds = (long) ((double) *ticks / tps);
        if (screenbase[301] != ' ') /* if the attribute has changed, */
            displayclock();         /* the screen scrolled, so update. */
        if (seconds % 60 != lastsec)
        {
            lastsec = seconds % 60;
            calls = 0;
            screenbase[314] = (char) (lastsec/10) + 48;
            screenbase[316] = (char) (lastsec%10) + 48;
            if ((! lastsec) || (lastmin < 0))
            {
                lastmin = (seconds % 3600) / 60;
                screenbase[308] = (char) (lastmin/10) + 48;
                screenbase[310] = (char) (lastmin%10) + 48;
                if ((! lastmin) || (lasthr < 0))
                {
                    lasthr = ((seconds % 86400L) / 3600L);
                    screenbase[302] = (char) (lasthr/10) + 48;
                    screenbase[304] = (char) (lasthr%10) + 48;
                }
            }
        }
    }
    oldhandler ();
}

main()
{
    oldhandler = getvect (0x1c);
    displayclock();
    setvect (0x1c, clockproc);
    keep (0, (_SS + (_SP/16) - _psp));
    return 0;
}
