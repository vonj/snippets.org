/*
**  INDXLOOK.H
**
**  SNIPPETS header file for INDEX.C and LOOKUP.C
**
**  public domain by Mark Corgan
*/

#include "sniptype.h"			/* for True_, False_, Boolean_T	*/
#define MAX_LINE           40       /* maximum line length			*/
#define MAX_KEY            26       /* maximum key size			*/
#define END_REC         ".\n"       /* end of a record			*/

struct index_rec                    /* index record				*/
{
	char key[MAX_KEY];            /* name					*/
	long pos;                     /* position					*/
};

typedef struct index_rec INDEX;
