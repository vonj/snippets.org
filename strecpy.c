/*
**  STRECPY.C - A form of strcpy() where the result returned is the
**              NUL terminating character of the first argument. In many
**              cases this function is more efficient than the equivalent
**              strcpy, followed by strcat.
**
**  public domain by Leslie Satenstein and Thad Smith.
**
**  NOTE: The name of this funtion violates ANSI/ISO 9899:1990 sec. 7.1.3,
**        but this violation seems preferable to either violating sec. 7.13.8
**        or coming up with some hideous mixed-case or underscore infested
**        naming. Also, many SNIPPETS str---() functions duplicate existing
**        functions which are supported by various vendors, so the naming
**        violation may be required for portability.
*/

#include <stdio.h>
#include "sniptype.h"
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strecpy(char *target, const char *src)
{
      if (src && target)
      {
            while ((*target++ = *src++) != NUL)
                  ;
            return (--target);
      }
      else  return NULL;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
