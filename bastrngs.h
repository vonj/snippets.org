/*
**  BASIC-like string operations
*/

#ifndef BASTRNGS__H
#define BASTRNGS__H

#include <stdlib.h>                             /* For size_t     */

char *stralloc(size_t length);
void  str_free(char *string);
char *left(char *string, size_t N);
char *right(char *string, size_t N);
char *mid(char *string, size_t M, size_t N);
char *string_add(char *string, ...);
char *string(int ch, size_t N);

unsigned int instr(const unsigned int start_pos,
                   const char        *string,
                   const char        *findstr);

#endif /* BASTRNGS__H */
