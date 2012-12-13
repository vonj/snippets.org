/*
** jgets() - line input w/editing
*/

#ifndef EDITGETS__H
#define EDITGETS__H

#include "minmax.h"

extern int TabSize;

/*
**  Password mode:
**
**  0 - Normal operation
**  1 - Conventional password mode - '*' is echoed for all characters,
**      only ESC, ENTER, BACKSPC, and CTLHOME are active.
**  2 - Secure password mode - same as above except random characters are
**      displayed rather than '*'s to distract snoops from watching the keys.
*/

extern int password_mode;

/*
**  Line editing formatted text entry function.
*/

int editgets(char *s, int maxlen, char *string);


/*
**  Get a string of unknown length
*/

char *getstring(void);


#endif /* EDITGETS__H */
