/*
**  SNIPPETS header file for INITVARS.C
*/

#ifndef INITVARS__H
#define INITVARS__H

#include <stdio.h>

#define LNSZ 256

int init_globals(FILE *fp, char *names, char *types, ...);

#endif /* INITVARS__H */
