/*
**  hugesort.c -- huge qsort() -- public domain by Raymond Gardner 6/92
**  tested with Borland C++ 3.0 (C mode)
*/

#include "big_mall.h"
#include "snipsort.h"

static void swap(char HUGE *a, char HUGE *b, unsigned n)
{
      char tmp;

      do
      {
            tmp = *a; *a++ = *b; *b++ = tmp;
      } while ( --n );
}

void hugesort(void HUGE *basep,
              unsigned   nel,
              unsigned   width,
              int      (*comp)(void HUGE *, void HUGE *))
{
      char HUGE *i, HUGE *j;
      unsigned int lnel, rnel;
      char HUGE *base = (char HUGE *)basep;

      while (nel > 1)
      {
            swap(base, base + (long)width * (nel / 2), width);
            for (i = base, j = base + (long)width * nel; ; )
            {
                  do
                        j -= width;
                  while ( (*comp)(j, base) > 0 );
                  do
                        i += width;
                  while ( i < j && (*comp)(i, base) < 0 );
                  if (i >= j)
                        break;
                  swap(i, j, width);
            }
            swap(j, base, width);
            lnel = (unsigned)((long)(j - base) / width);
            rnel = nel - lnel - 1;
            j += width;
            if (lnel < rnel)
            {
                  hugesort(base, lnel, width, comp);
                  base = j;
                  nel = rnel;
            }
            else
            {
                  hugesort(j, rnel, width, comp);
                  nel = lnel;
            }
      }
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

#define PADSIZE 300

typedef struct x {
    int key;
    char pad[PADSIZE];
    } X;

int cmpv(void HUGE *a, void HUGE *b) /* (note void HUGE *) passed here */
{
      return ((X HUGE *)a)->key < ((X HUGE *)b)->key ? -1 :
            ((X HUGE *)a)->key > ((X HUGE *)b)->key ? 1 : 0;
}

int main(int argc, char **argv)
{
      X HUGE *v;
      int n;
      int i, j;

      n = 300;                            /* default element count */
      if (argc > 1)
            n = atoi(argv[1]);
      printf("test with %d elements\n", n);

      if (NULL == (v = BigMalloc(sizeof(X), n)))
            ErrExit("Insufficient memory"); /* be sure we got memory */

      for (i = 0; i < n; ++i)             /* random init */
      {
            v[i].key = rand();
            for (j = 0; j < PADSIZE; ++j)
                  v[i].pad[j] = rand();
      }
      for (i = 0; i < n; ++i)             /* display before */
            printf(" %d", v[i].key);
      printf("\n");
      hugesort(v, n, sizeof(X), cmpv);    /* sort it */
      for ( i = 0; i < n; ++i )           /* display after */
            printf(" %d", v[i].key);
      printf("\n");
      BigFree(v);
      return 0;
}

#endif /* TEST */
