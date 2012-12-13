/*
**  BINSRCH.H - Header file for binary searching of an array for a key.
**
**  Public domain demo by Bob Stout
*/

#ifndef BINSRCH__H
#define BINSRCH__H

#include <stddef.h>
#include "sniptype.h"

#if !(__cplusplus)

int BinSearchI(int key, int *r, size_t n);
int BinSearchC(signed char key, signed char *r, size_t n);
int BinSearchB(BYTE key, BYTE *r, size_t n);
int BinSearchS(signed short key, signed short *r, size_t n);
int BinSearchW(WORD key, WORD *r, size_t n);
int BinSearchL(signed long key, signed long *r, size_t n);
int BinSearchDW(DWORD key, DWORD *r, size_t n);
int BinSearchF(float key, float *r, size_t n);
int BinSearchD(double key, double *r, size_t n);

#else /* C++ */

template <class T>
int BinSearch(T key, T *r, size_t n)
{
      int high, i, low;

      if (n > 1)
      {
            for (low = 0, high = n-1;  high - low > 1; )
            {
                  i = (high+low) / 2;
                  if (key == r[i])
                        return i;
                  else if (key < r[i])
                        high = i;
                  else  low  = i;
            }
            if (key == r[high])
                  return high;
            }
            else if (key == r[low])
                  return low;
            else  return -1;
      }
      else if (r[0] == key)
            return 0;
      else  return -1;
}

#endif /* C/C++ */

#endif /* BINSRCH__H */
