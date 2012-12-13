/*
** quicksort.c -- quicksort integer array
**
** public domain by Raymond Gardner     12/91
*/

#include "snipsort.h"

static void swap(int *a, int *b)
{
      register int t;

      t = *a;
      *a = *b;
      *b = t;
}

void quicksort(int v[], unsigned n)
{
      unsigned i, j, ln, rn;

      while (n > 1)
      {
            swap(&v[0], &v[n/2]);
            for (i = 0, j = n; ; )
            {
                  do
                        --j;
                  while (v[j] > v[0]);
                  do
                        ++i;
                  while (i < j && v[i] < v[0]);
                  if (i >= j)
                        break;
                  swap(&v[i], &v[j]);
            }
            swap(&v[j], &v[0]);
            ln = j;
            rn = n - ++j;
            if (ln < rn)
            {
                  quicksort(v, ln);
                  v += j;
                  n = rn;
            }
            else
            {
                  quicksort(v + j, rn);
                  n = ln;
            }
      }
}
