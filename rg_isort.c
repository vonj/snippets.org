/*
** isort()  --  insertion sort
**
** Raymond Gardner   public domain   2/93
**
** qsort() compatible, but uses insertion sort algorithm.
*/

#include <stddef.h>             /* for size_t definition */
#include "snipsort.h"

void isort(void *base, size_t nmemb, size_t size,
            int (*comp)(const void *, const void *))
{
      char *i, *j, *lim;

      lim = (char *)base + nmemb * size;    /* pointer past end of array */
      for ( j = (char *)base, i = j+size; i < lim; j = i, i += size )
      {
            for ( ; comp((void *)j, (void *)(j+size)) > 0; j -= size )
            {
                  char *a, *b;
                  char tmp;
                  size_t k = size;

                  a = j;
                  b = a + size;
                  do
                  {
                        tmp = *a;
                        *a++ = *b;
                        *b++ = tmp;
                  } while ( --k );
                  if ( j == (char *)base )
                        break;
            }
      }
}
