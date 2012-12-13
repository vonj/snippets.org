/*
**  DIRENT.H - Posix compliant header
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#ifndef DIRENT_H
#define DIRENT_H

#include <stdio.h>                        /* For FILENAME_MAX     */
#include <dos.h>
#include "sniptype.h"
#include "dirport.h"

#undef DIR
#define DIR DIR_

/*
**  Posix directory structure
*/

typedef struct {
      int               dd_fd;
      unsigned          dd_loc,
                        dd_size;
      DOSFileData       dd_buf;
      char              dd_dirname[FILENAME_MAX];
} DIR;

DIR            *opendir(char *);
int             closedir(DIR *),
                rewinddir(DIR *);
struct dirent  *readdir(DIR *),
               *seekdir(DIR *, int, int);
#define         telldir(dd) dd->loc

/*
**  Other useful functions from DIRMASK.C and XSTRCMP.C
*/

int       dirmask(DOSFileData *, char *, char *, unsigned, unsigned);
Boolean_T xstrcmp (const char *, const char *);
Boolean_T xstricmp (const char *, const char *);

extern int DFerr;

extern DIR _DIRS[];

#endif /* DIRENT_H */
