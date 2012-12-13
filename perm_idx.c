/*
** Determine the permutation index for a given permutation list.
** Written by Thad Smith III, Boulder, CO  8/31/91
** Hereby contributed to the Public Domain.
**
** The following function computes the ordinal of the given permutation,
** which is index of the permutation in sorting order:
**  1, 2, ..., n-1, n   is index 0
**  1, 2, ..., n, n-1   is index 1
**  ...
**  n, n-1, ..., 2, 1   is index n! -1
**
** The actual values of the elements are immaterial, only the relative
** ordering of the values is used.
**
** pit[] is the array of elements of length size.
** The return value is the permutation index.
*/

#include "snipmath.h"

int perm_index (char pit[], int size)
{
      int i;
      register int j, ball;
      int index = 0;

      for (i = 1; i < size; i++)
      {
            ball = pit[i-1];
            for (j = i; j < size; j++)
            {
                  if (ball > pit[j])
                        index ++;
            }
            index *= size - i;
      }
      return index;
}
