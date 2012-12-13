/*
**  SNIPPETS header file for FSCANBIN.C
*/

#ifndef FSCANBIN__H
#define FSCANBIN__H

#include <stdio.h>
#include "sniptype.h"

#define WORDswap(n)  (*n = (*n << 8) | (*n >> 8))

#define DWORDswap(n)  (\
                        WORDswap(&((WORD *)n)[0]),\
                        WORDswap(&((WORD *)n)[1]),\
                        *n = (*n << 16) | (*n >> 16)\
                        )

#define maxk 32767

int fscanbin (FILE *fp, char *format, ...);

#endif /* FSCANBIN__H */
