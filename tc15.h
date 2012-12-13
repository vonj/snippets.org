/*
**  SNIPPETS doesn't support many older compilers, but it's easy to add
**  compiler or version specific header to make things portable. Here's
**  a sample header for Turbo C 1.5.
*/

#ifndef TC15__H
#define TC15__H

#include "clock.h"
#include "strftime.h"

#define signal ssignal

#define FILENAME_MAX 80

#endif /* TC15__H */
