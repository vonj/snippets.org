/*
**  Some semi-Posix compliant macros for DOS/Windows/NT
*/

#ifndef SNIP_STAT_H
#define SNIP_STAT_H

#include <sys/stat.h>

#ifndef S_ISDIR
 #define S_ISDIR(x) ((x) & _S_IFDIR )
 #define S_ISREG(x) ((x) & _S_IFREG )
#endif

#endif /* SNIP_STAT_H */
