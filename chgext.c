/*
**  chgext.c - Change a file's extension
**
**  public domain by Bob Stout
**
**  Arguments: 1 - Pathname
**             2 - Old extension (NULL if don't care)
**             3 - New extension
**
**  Returns: Pathname or NULL if failed
**
**  Note: Pathname buffer must be long enough to append new extension
**
**  Side effect: Converts Unix style pathnames to DOS style
*/

#include <stdio.h>
#include <string.h>
#include "filnames.h"

char *chgext(char *path, char *oldext, char *newext)
{
      char *p;

      /* Convert to DOS-style path name */

      for (p = path; *p; ++p)
            if ('/' == *p)
                  *p = '\\';

      /* Find extension or point to end for appending */

      if (NULL == (p = strrchr(path, '.')) || NULL != strchr(p, '\\'))
            p = strcpy(&path[strlen(path)], ".");
      ++p;

      /* Check for old extension */

      if (oldext && strcmp(p, oldext))
            return NULL;

      /* Add new extension */

      while ('.' == *newext)
            ++newext;
      strncpy(p, newext, 3);

      /*
      ** Added to insure string is properly terminated. Without this, if
      ** the new extension is longer than the old, we lose the terminator.
      */

      p[strlen(newext)] = '\0';

      return path;
}

#ifdef TEST

main(int argc, char *argv[])
{
      char *retval, *old_ext = NULL, path[128];

      if (2 > argc)
      {
            puts("Usage: CHGEXT path [old_ext]");
            puts("\nChanges extension to \".TST\"");
            puts("Old extension optional");
            return -1;
      }
      strcpy(path, strupr(argv[1]));
      if (2 < argc)
            old_ext = strupr(argv[2]);
      if (NULL == (retval = chgext(path, old_ext, ".TSTstuff")))
            puts("chgext() failed");
      else  printf("chgext(%s, %s, TST)\n...returned...\n%s\n", argv[1],
            old_ext ? old_ext : "NULL", path);
      return (NULL == retval);
}

#endif /* TEST */
