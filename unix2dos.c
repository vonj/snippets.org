/*
**  UNIX2DOS.C - Convert Unix-style pathnames to DOS-style
**
**  public domain by Bob Stout
*/

#include "filnames.h"

char *unix2dos(char *path)
{
      char *p;

      for (p = path; *p; ++p)
            if ('/' == *p)
                  *p = '\\';
      return path;
}

char *dos2unix(char *path)
{
      char *p;

      for (p = path; *p; ++p)
            if ('\\' == *p)
                  *p = '/';
      return path;
}
