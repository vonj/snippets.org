/*

AMALLOC - multi-dimensional malloc()

Allocates a multidimensional array dynamically, at runtime, so that
  1: its elements can be accessed using multiple indirection
  2: it can be deallocated using a call to the standard free() function
Note: On PC's the max array size is 64K

Paul Schlyter, 1992-02-09.  Released to the public domain.

*/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "snparray.h"


#define MAXDIMS  5              /* Defines the maximum number of dimensions */
#define MAXSIZE  ((size_t) -1L) /* Maximum size of array */


void *amalloc( int esiz, void *initval, int dims, ... )
/*
 *  Input:   esiz     size of each array elements, as given by sizeof
 *           initval  pointer to initial value. NULL ==> zero fill
 *           dims     number of dimensions: 1..MAXDIMS (5)
 *           ...      number of elements in each dimension (int's)
 *
 *  Returns:  NULL    error: out of memory, or illegal parameters
 *                    otherwise base pointer to array
 */
{
      unsigned int dim[MAXDIMS], accdim[MAXDIMS];
      va_list ap;
      int i, j;
      long int totsiz;
      void **q;
      char *p, *r, *s;

      if (dims < 1  ||  dims > MAXDIMS)
            return NULL;

      memset(dim, 0, sizeof(dim));          /* Read dimension numbers */
      memset(accdim, 0, sizeof(accdim));
      va_start(ap, dims);
      dim[0] = accdim[0] = va_arg(ap,int);
      for (i = 1; i < dims; i++)
      {
            dim[i] = va_arg(ap,int);
            accdim[i] = accdim[i-1] * dim[i];
      }
      va_end(ap);

                                            /* Compute total array size */
      totsiz = esiz * accdim[dims-1];       /* Data size */

      for (i = 0; i < dims - 1; i++ )       /* Add space for pointers */
            totsiz += sizeof(void *) * accdim[i];

      if (totsiz > MAXSIZE)                 /* Exit if totsiz too large */
            return NULL;

      p = malloc((size_t) totsiz);          /* Allocate memory */
      if (p == NULL)                        /* Out-of-memory   */
            return NULL;
      memset(p, 0, (unsigned int) totsiz);  /* Zero out allocated memory */
      q = (void **) p;

      if (dims == 1)
            r = (char *) q + esiz * accdim[0];

      for (i = 1; i < dims; i++)            /* Fill in pointers */
      {
            int siz;
            int accd = accdim[i-1], d = dim[i];

            siz =  i == dims-1 ? esiz : sizeof(void *);

            r = (char *) q + sizeof(void *) * accd;
            for (j = 0; j < accd; j++)
            {
                  *q++ = r;
                  r += siz * d;
            }
      }

      if (initval != NULL)
      {
            for (s = (char *) q; s < r; s += esiz)
                  memcpy(s, initval, esiz);
      }

      return p;

}  /* amalloc */


#ifdef TEST   /* Test program */

#include <stdio.h>

main()
{
      static char init_d[8] = { 0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF };
      int init_i = 0x1111;
      double *a   = amalloc( sizeof(double), init_d, 1, 4 );
      double **b  = amalloc( sizeof(double), init_d, 2, 4, 5 );
      double ***c = amalloc( sizeof(double), init_d, 3, 4, 5, 6 );
      int ***d = amalloc( sizeof(int), &init_i, 3, 4, 5, 6 );
      int i, j, k;

      for (i = 0; i < 4; i++)
            for (j = 0; j < 5; j++ )
                  for (k = 0; k < 6; k++ )
                        d[i][j][k] = (i * 256) + (j * 16) + k;

      a = a, b = b, c = c;

      return 0;
}

#endif /* TEST */
