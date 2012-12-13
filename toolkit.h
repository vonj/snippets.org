/*
**  This is a copyrighted work which is functionally identical to work
**  originally published in Micro Cornucopia magazine (issue #52, March-April,
**  1990) and is freely licensed by the author, Walter Bright, for any use.
*/

/*_ toolkit.h   Tue Apr 18 1989   Modified by: Walter Bright */

#ifndef TOOLKIT_H
#define TOOLKIT_H

/* Define stuff that's different between machines.
 * PROTOTYPING          1 if compiler supports prototyping
 * HOSTBYTESWAPPED      1 if on the host machine the bytes are
 *                swapped (1 for 6809, 68000, 0 for 8088
 *                and VAX).
 */

#if defined(MSDOS) || defined(__MSDOS__)
#define PROTOTYPING     1
#define HOSTBYTESWAPPED 0

#define BITSPERBYTE 8
#define SIZEOFINT   sizeof(int)
#define SIZEOFLONG  sizeof(long)

#else
#ifdef M_UNIX     /* SCO UNIX using Microsoft C. */
#define PROTOTYPING     1
#define HOSTBYTESWAPPED 0
#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

#define BITSPERBYTE 8
#define SIZEOFINT   sizeof(int)
#define SIZEOFLONG  sizeof(long)
#else                   /* NOTE: host.h is *NOT* included in SNIPPETS    */
#include    "host.h"    /* Compiler/environment-specific stuff goes here */
#endif

#endif

/* Static definitions do not appear in the linker .MAP file. Override   */
/* the definition here to make them global if necessary.          */
#ifndef STATIC
#define STATIC    static
#endif

#define arraysize(array)      (sizeof(array) / sizeof(array[0]))

/* Macros so that we can do prototyping, but still work with non- */
/* prototyping compilers:                                   */

#if PROTOTYPING
#define P(s)      s
#else
#define P(s)      ()
#endif

#ifdef DEBUG
#define debug(a)  (a)
#else
#define debug(a)
#endif

#endif /* TOOLKIT_H */
