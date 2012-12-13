/* =======================================================================
    CLOCK.H     A simple macro based implementation of the ANSI/ISO
                standard clock() function and related items.

                Some older C compilers for DOS do not implement these
                items. E.g.: Turbo C 1.5 ... This file fixes it.
                WARNING: There is no provision for handling the
                24-hour/midnight rollover problem.

                Suggested use: #include this file _after_ time.h if
                there are problems related to clock().

                v1.00  95-10-19  Initial version.

 _____          This version is Public Domain.
 /_|__|         Auke Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef CLOCKS_PER_SEC

#include "extkword.h"   /* from Snippets, for definition of FAR */

typedef long clock_t;
#define clock()         *((clock_t FAR *)0x0040006cL)
#define CLOCKS_PER_SEC  18.2
#define CLK_TCK         18.2   /* Non-ANSI/ISO but _very_ common. */

#endif

/* === clock.h end ===================================================== */
