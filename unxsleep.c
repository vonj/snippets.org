/* microsleep.c */

/* Author:
 * Pieter de Jong
 * Released in Public Domain 7-1996
 */


/* This module contains the unix-specific versions the microsleep function.
 * There are actually two versions of microsleep defined here, because
 * BSD, SysV, and V7 all need quite different implementations.
 *
 * At this time, the select() call is not (yet) in the POSIX standard, but
 * it is in BSD4.2+ , and SVR4.
 * poll() is SYSV only. BSD has a usleep() function that looks & feels
 * like microsleep(), but implements it using setitimer(). It has
 * (usually) unwanted side effects when multiple timers are active.
 *
 * This version should interact correctly with other timers set
 * by the calling process, and is not interrupted if a signal is caught
 */
/* #include "config.h"*/

# ifdef BSD
/* For BSD, we use select() */

#include<sys/types.h>
#include<sys/time.h>
#include<stddef.h>

void
microsleep(unsigned int microsecs)
{
	struct timeval tval;

	tval.tv_sec = microsecs/ 1000000;
	tval.tv_usec= microsecs% 1000000;
	select(0, NULL, NULL, NULL, &tval);
}
#endif

# ifdef SYSV
/* For System-V, we use poll to implement the timeout. */
/* R4 has select(), but implements it using poll() */
/* older versions may only have poll() */

#include<sys/types.h>
#include<sys/poll.h>
#include<stropts.h>

void
microsleep(unsigned int microsecs)
{
	struct pollfd dummy;
	int timeout;

	if ((timeout = microsecs / 1000) <= 0)
		timeout = 1;
	poll(&dummy, 0, timeout);
}

# endif /* !BSD */
