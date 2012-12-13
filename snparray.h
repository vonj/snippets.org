/*
**  Header file for SNIPPETS array allocation functions
*/

#ifndef SNPARRAY__H
#define SNPARRAY__H

/*
**  AMALLOC.C
*/

void *amalloc( int esiz, void *initval, int dims, ... );

/*
**  MDALLOC.C
*/

void *mdalloc(int ndim, int width, ...);
void mdfree(void *tip, int ndim);

#endif /* SNPARRAY__H */
