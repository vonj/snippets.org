/*
**  FILNAMES.H - Header file for SNIPPETS DOS file name processing functions.
*/

#ifndef FILNAMES__H
#define FILNAMES__H

#include "sniptype.h"

char *fln_fix(char *path);
int   flnorm(char *in_name, char *out_name);
char *unix2dos(char *path);
char *dos2unix(char *path);
char *chgext(char *path, char *oldext, char *newext);

Boolean_T valid_fname (const char *fname, Boolean_T wild_check);

/*
**  FNSPLIT.C prototypes and definitions
*/

#define Extension_ 1
#define Filename_  2
#define Directory_ 4
#define Drive_     8
#define Wildname_  16
#define Wildpath_  32

Boolean_T has_wild(char *pname);
int       fnSplit(char *spec, char *drive, char *pname, char *path,
                  char *fname, char *name, char *ext);
char     *fnMerge(char *spec, char *drive, char *pname, char *path,     
                  char *fname, char *name, char *ext) ;

/*
**  Macros for backwards compatibility with previous SNIPPETS, also more
**  compatible with Borland psplit(), Microsoft _splitpath(), et al.
*/

#define psplit(str,drv,dir,nam,ext) fnSplit(str,drv,NULL,dir,NULL,nam,ext)
#define pmerge(str,drv,dir,nam,ext) fnMerge(str,drv,NULL,dir,NULL,nam,ext)

#endif /* FILNAMES__H */
