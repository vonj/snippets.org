/*
**  xfile.h -- definitions for fast line buffered files
*/

#ifndef __XFILE_H__
#define __XFILE_H__

struct _xfile {
    int   fd;
    int   bufSize;
    char *buf;
    char *nextChar;
    char *lastChar;
};

typedef struct _xfile XFILE;

#include <dos.h>

#if defined(__ZTC__)
 #include <io.h>
 #define DOS_OPEN dos_open
 #define READ     read
 #define CLOSE    close
#elif defined(__TURBOC__)
 #include <io.h>
 #include <fcntl.h>
 #define DOS_OPEN _open
 #define READ     _read
 #define CLOSE    _close
#else /* MSC */
 #include <stdlib.h>
 #include <fcntl.h>
 #define CLOSE    _dos_close
#endif

XFILE *xopen(char const *);
void   xclose(XFILE *);
char  *xgetline(XFILE *);

#endif  /* __XFILE_H__ */
