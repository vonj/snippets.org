/*
** void FAR *BigMalloc(Size_T num_elem, Size_T size_elem);
** void BigFree(void FAR *ptr);
*/

#ifndef BIG_MALL__H
#define BIG_MALL__H

#include "extkword.h"

#if (defined(MSDOS) || defined(__MSDOS__)) && !defined(__FLAT__)
 #if defined(__TURBOC__) || defined(__POWERC) || defined(__ZTC__)
  #if defined(__ZTC__)
   #include <dos.h>
  #else     /* Borland or Mix */
   #include <alloc.h>
  #endif
  typedef unsigned long Size_T;
  #define BigMalloc(i,n) (void FAR *)farmalloc((Size_T)(i)*(Size_T)(n))
  #define BigFree(p) farfree(p)
 #else                                          /* MSC, Watcom    */
  #include <malloc.h>
  #include <stddef.h>                           /* for size_t     */
  typedef size_t Size_T;
  #define BigMalloc(i,n) (void FAR *)halloc((Size_T)(i),(Size_T)(n))
  #define BigFree(p) hfree(p)
 #endif
#else
 #include <stdlib.h>
 #include <stddef.h>                            /* for size_t     */
 typedef size_t Size_T;
 #define BigMalloc(i,n) malloc((Size_T)(i)*(Size_T)(n))
 #define BigFree(p) free(p)
#endif

#endif /* BIG_MALL__H */
