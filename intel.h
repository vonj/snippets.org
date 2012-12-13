/*
**  SNIPPETS functions for Intel CPUs and NDP's
*/

#ifndef INTEL__H
#define INTEL__H

#include "sniptype.h"         /* For True_ and False_                   */
#include "extkword.h"         /* For CDECL                              */

int CDECL cpu_ID(void);       /* Returns: AL - CPU family
                                               0 = 8086/8088
                                               1 = 80186
                                               2 = 80286
                                               3 = 80386
                                               4 = 80486
                                               5 = Pentium
                                               6 = Pentium Plus
                                          AH - Features bitmap
                                               01h = Has coprocessor
                                               02h = Is 386/287
                                               04h = Is 386SX
                                               08h = Is Cyrix
                                               10h = Is NEC
                                               20h = Is NexGen
                                               40h = is AMD
                                               80h = Is UMC             */

extern CDECL save_8087(void);       /* Save coprocessor environment     */
extern CDECL restore_8087(void);    /* Restore coprocessor environment  */

#endif /* INTEL__H */
