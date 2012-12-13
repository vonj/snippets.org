/*   Written by Blair Haukedal 91/09 and placed in the public domain */

/*  mdalloc - a multi dimensional array allocator
 *  mdfree  - a companion function to mdalloc for freeing storage
 *  synopsis:
 *      void *mdalloc(int ndim, int width, ...);
 *          where:  ndim:   number of array dimensions
 *                  width:  size of elements in array
 *                  variable args are dimensions of array
 *          returns: n-way indirect pointer to allocated storage
 *                   or NULL if insufficient storage
 *
 *      void mdfree(void *p, ndim);
 *          where:  p:      pointer to storage obtained by mdalloc
 *                  ndim:   number of dimensions used in mdalloc
 *
 *  example:
 *      int ***tip;
 *      tip = mdalloc(3, sizeof(int), 2, 3, 4);
 *        tip will be a triple indirect pointer to a 3 dimensional array
 *        tip[0][0][0] refers to the first int in a contiguous area of
 *                     storage that is 2*3*4*sizeof(int) bytes long
 *        tip[0][0] is the address of the first int
 *      memset can be used to initialize array elements as follows:
 *        memset(tip[0][0], 0, 2*3*4*sizeof(int));
 *      mdfree is used to free storage obtained with mdalloc:
 *        mdfree(tip, 3)
 *
 *  notes:
 *      - must be compiled with appropriate memory model
 *      - memory is allocated for each dimension for indirect pointers
 *          eg. 3x4x5 array of longs
 *              (assuming 4 byte longs, small mem model)
 *              p = mdalloc(3, sizeof(long), 3, 4, 5)            - bytes
 *                  3       pointers allocated for 1st dimension -  6
 *                  3x4     pointers allocated for 2nd dimension -  24
 *                  3x4x5   longs allocated for array elements   -  240
 *              total of 270 bytes allocated
 *      - if insufficient memory, nothing will be allocated.
 *          ie. intermediate pointer arrays that were successfully
 *              allocated will be freed.
 *      - the intent of mdalloc is to facilitate dynamic array creation,
 *        it will use more memory than statically declared arrays, and
 *        the required dereferencing will be slower than the use of
 *        statically declared arrays.
 *      - this function assumes that sizeof(char) == 1.
 */

#include <stdarg.h>
#include <stdlib.h>
#include "snparray.h"

static void **md2(int n_units, int ndim, int *dims);
static void md3(char ***tip, int n_units, int ndim, int *dims);

static int w_units;

/* mdalloc: entry point for mdalloc function described above
 *      - reduces variable arg list to fixed list with last arg
 *      represented as pointer to int (array dimensions).
 *      Calls md2 to allocate storage.
 *      Calls md3 to initialize intermediate pointers.
 *      Returns pointer.
 */

void *mdalloc(int ndim, int width, ...)
{
      va_list argp;
      int *dims, i;
      char ***tip;

      va_start(argp, width);

      /* allocate storage for variable args (dimensions) */

      dims = malloc(ndim*sizeof(int));
      if(dims == NULL)
            return NULL;

      /* initialize dimensions array for subsequent calls */

      for(i=0; i<ndim; i++)
            dims[i] = va_arg(argp,int);

      w_units = width;    /* global used by md2 and md3 */

      /* allocate required pointer and array element storage */

      tip = (char ***)md2(dims[0], ndim, &dims[1]);

      if(ndim>1 && tip)
            md3(tip, dims[0], ndim-1, &dims[1]); /* init pointers */

      free(dims);
      return tip;
}

/* mdfree:  companion function to mdalloc
 *          frees storage obtained by mdalloc
 */

void mdfree(void *tip, int ndim)
{
      if(ndim == 1)
            free(tip);
      else
      {
            mdfree(((void **)tip)[0], ndim-1);
            free(tip);
      }
}

/* md2:  allocates storage for n-way indirect pointer arrays
 *       allocates storage for requested array elements
 */

static void **md2(int n_units, int ndim, int *dims)
{
      char **tip;

      if(ndim == 1)
            /* recursed to final dimension - allocate element storage */
            tip = malloc(n_units*w_units);
      else
      {
            /* allocate pointer array for dimension n */
            tip = malloc(n_units*sizeof(char *));
            if(tip)
            {
                  /* recurse until final dimension */
                  tip[0] = (char *)md2(n_units*dims[0], ndim-1, &dims[1]);
                  if(tip[0] == NULL)
                  {
                        /* allocate error - fall back up freeing everything */
                        free(tip);
                        tip = NULL;
                  }
            }
      }
      return (void **)tip;
}

/* md3: initializes indirect pointer arrays */

static void md3(char ***tip, int n_units, int ndim, int *dims)
{
      int i;

      for(i=1; i<n_units; i++)
      {
            if(ndim == 1)
                  /* final dimension - must scale by element width */
                  tip[i] = (char **)((char *)tip[0] + i*dims[0]*w_units);
            else
                  /* intermediate dimension - scale by pointer size */
                  tip[i] = tip[0] + i*dims[0];
      }
      if(ndim > 1)
            /* not at final dimension - continue to recurse */
            md3((char ***)tip[0], n_units*dims[0], ndim-1, &dims[1]);
}
