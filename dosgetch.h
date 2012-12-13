/*
   Header for DOSGETCH.ASM
   Donated to the public domain 96-11-12 by Tom Torfs (2:292/516)
*/

#ifndef DOSGETCH__H
#define DOSGETCH__H

#include "extkword.h"

extern int CDECL dosgetch(void);
extern int CDECL doswaitch(void);

#define doswaitkey() if (doswaitch()==0) doswaitch()

#endif /* DOSGETCH__H */
