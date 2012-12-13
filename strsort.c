/*
**  strsort() -- Shell sort an array of string pointers via strcmp()
**  Written in ANSI C and optimized for size under Borland TC and BC++.
**
**  Public domain by Raymond Gardner 12/05/91   :-)
**  based on a public domain version by Thad Smith 12/05/91,
**  based on a public domain version by
**  Ray Gardner   Denver, CO   12/88
*/

#include <string.h>
#include "snipsort.h"

void STRSORT(char **v, unsigned n)
{
      register unsigned int gap;
      unsigned int i, j;
      register char **a;
      char **b;

      gap = 0;
      do
      {
            gap <<= 1;
      } while (++gap < n);

      while ((i = (gap >>= 1)) != 0)
      {
            for ( ; (j = i) < n; i++)
            {
                  a = v + j;
                  do
                  {
                        j -= gap;
                        b = a;
                        a -= gap;
                        if (strcmp(*a, *b) > 0)
                        {
                              register char *tmp;
                              tmp = *a;
                              *a = *b;
                              *b = tmp;
                        }
                        else  break;      /* better have this break! */
                  } while (j >=  gap);
            }
      }
}
