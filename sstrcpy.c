/*
**  strcpy() and strcat() work-alikes which allow overlapping buffers.
*/

#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *sstrcpy(char *to, char *from)
{
    memmove(to, from, 1+strlen(from));
    return to;
}

char *sstrcat(char *to, char *from)
{
    sstrcpy(to + strlen(to), from);
    return to;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
