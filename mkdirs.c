/*
**  MKDIRS.C - Function to build multi-level directories in a single call
**
**  Original Copyright 1993-95 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Also uses PUSHDIR.C from SNIPPETS.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//#include <io.h>
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif
#include "sniptype.h"
#include "filnames.h"

int mkdirs(char *pathname)
{
      int retval;
      char path[FILENAME_MAX];

#if defined(__unix__)
      umask(0);
#endif

      strcpy (path, pathname);            /* isdir() may expand this    */
      dos2unix(path);

      while (strlen(path) && '/' == LAST_CHAR(path))
            LAST_CHAR(path) = NUL;

      while (0 != (retval = mkdir(path
#if defined(__unix__)
                                 , 0755
#endif
                                 )))
      {
            char subpath[FILENAME_MAX] = "", *delim;

//            if (EACCES == errno)
//            {
//                  if (isdir(path))
//                        return 0;
//                  else  return retval;
//            }
            if (NULL == (delim = strrchr(path, '/')))
                  return retval;
            strncat(subpath, path, delim - path);     /* Appends NUL    */
            if (Success_ != mkdirs(subpath))
                  break;
      }
      return retval;
}

#ifdef TEST

main(int argc, char *argv[])
{
      if (2 > argc)
      {
            puts("Usage: MKDIRS pathname [...pathname]");
            return -1;
      }
      while (--argc)
      {
            ++argv;
            printf("mkdirs(%s) returned %d\n", *argv, mkdirs(*argv));
      }
      return 0;
}

#endif /* TEST */
