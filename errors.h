/*
**  Header file for SNIPPETS error handlers
*/

#ifndef ERRORS__H
#define ERRORS__H

#include <stdio.h>

int  ferrorf(FILE *filehandle, const char *format, ...); /* Ferrorf.C   */
FILE * cant(char *fname, char *fmode);                   /* Ferrorf.C   */
void ErrExit(char *fmt, ...);                            /* Err_Exit.C  */

#endif /* ERRORS__H */
