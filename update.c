/*
** update_file() - 1) Renames szFname.bak (if existed) to tmpnam
**                 2) Renames szFname to szFname.bak
**                 3) Renames szOther to szFname.
**                    If szOther == NULL, skip this step (REN2BAK only).
**                 4) Removes old bak (tmpnam) file.
**
** public domain by Phi Nguyen
*/

#include "update.h"

Boolean_T update_file(char *szFname, char *szOther)
{
      char  szBakname[FILENAME_MAX],
            szTbakname[L_tmpnam],
            *p;

      if (NULL == szFname)
            return Error_;

      strcpy(szBakname, szFname);
      p = strrchr(szBakname, '.');
      if (p && !strpbrk(p, "\\/"))
            *p = NUL;
      strcat(szBakname, ".bak");

      rename(szBakname, tmpnam(szTbakname));
      if (rename(szFname, szBakname))
      {
            rename(szTbakname, szBakname);
            return Error_;
      }

      if (NULL != szOther)
      {
            if (rename(szOther, szFname))
            {
                  rename(szBakname, szFname);
                  rename(szTbakname, szBakname);
                  return Error_;
            }
      }

      remove(szTbakname);
      return Success_;
}

#ifdef TEST

#include <stdlib.h>

main(int argc, char *argv[])
{
      int i;

      if (1 == argc)
      {
            puts("Usage: Ren2Bak filename ...");
            return EXIT_FAILURE;
      }

      for (i = 1; i < argc; i++)
      {
            if (Error_ == ren2bak(argv[i]))
            {
                  printf("Can't rename file '%s'.\n", argv[i]);
                  return EXIT_FAILURE;
            }
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
