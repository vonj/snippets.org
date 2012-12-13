/*
**  SNIPPETS header file for MEMREV.C
*/

#ifndef MEMREV__H
#define MEMREV__H

#include <stddef.h>

void memrev(char  *buf, size_t count);
void aswap(char  *buf, size_t head, size_t tail);

#endif /* MEMREV__H */
