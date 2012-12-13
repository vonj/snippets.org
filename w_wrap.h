/* w_wrap.h */
/* prototypes for the functions in w_wrap.c */

#ifndef W_WRAP__H
#define W_WRAP__H

#include <stddef.h>     /* For size_t     */
#include <stdio.h>      /* For FILE       */

char *word_wrap(char *string, size_t line_len);
void set_tab_size(size_t size);
void center(FILE *file, char *string, size_t width);

#endif /* W_WRAP__H */
