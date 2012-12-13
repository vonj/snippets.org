/*  ======================================================================
    DEFINES.h       Standard definitions etc.
                    For simplification or for debugging substitution.

                    A.Reitsma, Delft, Nederland.
                    v0.23  94-07-02
----------------------------------------------------------------------- */

#ifndef STK_DEFS_H
#define STK_DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char       byte;
typedef unsigned short      byte2;
typedef unsigned long       byte4;
typedef unsigned int        word;

#define MALLOC(size,type)   (type *) malloc( (size) * sizeof( type ))
#define FREE(mem)           free( mem )
#define CALLOC(size,type)   (type *) calloc( (size), sizeof( type))

#ifdef DEBUG_NPA

extern void * NPAdata ;     /* Declare it in the main() file !!!       */

#if sizeof( void * ) != sizeof( void far * )   /* small data models */
#define NPAsize             0x40
#else
#define NPAsize             0x100 * sizeof( void far * )
#endif                      /* protect interrupt vectors for
                               large data models                       */

#define NPAinit()           NPAdata = malloc( NPAsize );      \
                            memcpy( NPAdata, NULL, NPAsize )
                            /* call NPAinit as first function in main() */

#define NPAcheck()          if( memcmp( NPAdata, NULL, NPAsize ))        \
                            {   printf( "\aNPA before line %d"           \
                                    " in file %s", __LINE__, __FILE__ ); \
                                memcpy( NULL, NPAdata, NPAsize );        \
                            }
#if 0
#define return(x)           { NPAcheck(); return(x); }
                            /* does NOT work for format "return XXX ;"  */
#else
#define return              NPAcheck(); return
                            /* does also work for format "return XXX ;" */
#endif                      /* but not for:
                                  if(...) return ...;  without { }      */
                            /* that causes warnings of unreachable code */
#pragma warn +rch           /* when switched on with this pragma ...    */
                            /* TC2.0, BC3.1 */
#else

#define NPAinit()
#define NPAcheck()

#endif

#endif /* STK_DEFS_H */

/*  === DEFINES.h end ================================================= */
