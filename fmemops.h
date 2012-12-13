/*
**  FMEMOPS.H - SNIPPETS header file for FMEMOPS.C
*/

#ifndef FMEMOPS__H
#define FMEMOPS__H

#include <stddef.h>
#include "extkword.h"

typedef unsigned char FAR *FarBytePtr;

void FAR * _fmemcpy(void FAR *dest, void FAR *src, size_t count);
void FAR * _fmemmove(void FAR *dest, void FAR *src, size_t count);
void FAR * _fmemset(void FAR *dest, int ch, size_t count);

#endif /* FMEMOPS__H */
