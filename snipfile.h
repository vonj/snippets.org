/*
**  Header file for portable file functions
*/

#ifndef SNIPFILE__H
#define SNIPFILE__H

#include <stdio.h>
#include "sniptype.h"

#define flushall() fflush(NULL)

long   flength(char *fname);                          /* Ansiflen.C     */
FILE * cant(char *fname, char *fmode);                /* Ferrorf.C      */
int    fcompare(const char *fnam1, const char *fnam2);/* Fcompare.C     */
long   fcopy(char *dest, char *source);               /* Fcopy.C        */
long   ffsearch(FILE *fp, const char *pattern,
             const size_t size, int N);               /* Srchfile.C     */
long   rfsearch(FILE *fp, const char *pattern,
             const size_t size, int N);               /* Srchfile.C     */
void   show_text_file(char *txt);                     /* Textmod.C      */
int    file_copy(char *from, char *to);               /* Wb_Fcopy.C     */
int    fdcopy(int fdfrom, int fdto);                  /* Wb_Fcopy.C     */
int    file_append(char *from, char *to);             /* Wb_Fapnd.C     */

Boolean_T exists(char *name);                         /* Existsx.C      */
char     *dexists(char *name, char *envar);           /* Existsx.C      */
char     *pexists(char *name);                        /* Existsx.C      */
char     *gexists(char *name, char *envar);           /* Existsx.C      */
FILE     *fopenp(char *name, char *mode);             /* Fopenx.C       */
FILE     *fopend(char *name, char *mode, char *envar);/* Fopenx.C       */
FILE     *fopeng(char *name, char *mode, char *envar);/* Fopenx.C       */


#endif /* SNIPFILE__H */
