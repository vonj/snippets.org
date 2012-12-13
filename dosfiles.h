/*
**  SNIPPETS header for functions to work with DOS files & directories
*/

#ifndef DOSFILES__H
#define DOSFILES__H

#include <stdio.h>
#include "dirport.h"
#include "sniptype.h"


Boolean_T   addpath(char *newdir);                    /* Addpath.C      */
void        copy(char *fromDir, char *fname,
                  char *toDir);                       /* Doscopy.C      */
int         getdrv(void);                             /* Drvalid.C      */
Boolean_T   chdrv(int drive);                         /* Drvalid.C      */
Boolean_T   drvalid(int drive);                       /* Drvalid.C      */
Boolean_T   drvrdy(int drive);                        /* Drvalid.C      */
int         favail(void);                             /* Favail.C       */
char      * getdcwd(unsigned int drive);              /* Getdcwd.C      */
int         iscons(FILE *fp);                         /* Iscons.C       */
int         isfopen(FILE *fp);                        /* Isfopen.C      */
int         drive_type(int dr);                       /* Isnetdr.C      */
Boolean_T   isRamDsk(unsigned char drive);            /* Isramdsk.C     */
int         isWprot(int drive);                       /* Iswprot.C      */
int         mkdirs(char *pathname);                   /* Mkdirs.C       */
int         PushDir(char *newdir);                    /* Pushdir.C      */
int         PopDir(void);                             /* Pushdir.C      */
int         isdir(char *dir);                         /* Pushdir.C      */
char FAR  * truename(char FAR *dst, char FAR *src);   /* Truename.C     */
Boolean_T   isCDROMdrive(int drive);                  /* Iscdrom.C      */
int         fdupdate(int fd);                         /* Fupdate.C      */
int         fupdate(FILE *fp);                        /* Fupdate.C      */

/*
**  File: ADDHNDLS.H
*/

#define TABLE_SIZE 255        /* NOTE: *Must* be <= FILES in CONFIG.SYS */

int relocate(void);


/*
**  File: FILES.C
*/

/*
** This is the format for a System File Table header.  SFT's are a linked
** list in which the header points to the next SFT, is followed by the
** number of FILES in this SFT, and ends with the FILES themselves, which
** are not important here.
*/

struct SFT_HEADER {
      struct SFT_HEADER (FAR *next);
      unsigned number;
};

int files(void);


#endif /* DOSFILES__H */
