/*
**  XSTRCAT.C - String concatenation function
**
**  Notes: 1st argument must be a buffer large enough to contain the
**         concatenated strings.
**
**         2nd thru nth arguments are the string to concatenate.
**
**         (n+1)th argument must be NULL to terminate the list.
*/

#include <stdarg.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *xstrcat(char *des, char *src, ...)
{
        char *destination = des;
        va_list v;

        va_start(v, src);

        while (src != 0)
        {
                while (*src != 0)
                        *des++ = *src++;
                src = va_arg(v, char *);
        }
        *des = 0;

        va_end(v);

        return destination;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
