/*
**  SNIPPETS header file for MOREPROC.C & ADJSCROL.C
**
**  Note: As written, this code is DOS-specific. To enhance portability:
**
**        1. Use UNXGETCH.C from SNIPPETS for portable EXT_KEYS.H functions.
**        2. Use constants or other non-DOS definitions for SCRNxxxx macros.
**        3. Conditional code in DELAY.C allows use with non-DOS compilers.
*/

#ifndef MORE__H
#define MORE__H

#include "scrnmacs.h"                     /* For SCREENROWS, SCREENCOLS */
#include "snip_str.h"                     /* For strn1cpy(), strmove()  */
#include "ext_keys.h"                     /* For ext_getch()            */
#include "delay.h"                        /* For delay()                */

int more_proc(char *str);                             /* MOREPROC.C     */
int adj_scroll(char *str);                            /* ADJSCROL.C     */

#endif /* MORE__H */
