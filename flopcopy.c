/*
**  FLOPCOPY.C
**
**  Copy a floppy to a hard disk directory with directory recursion
**  Public domain, uses functions from SNIPPETS.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sniptype.h"
#include "dirport.h"
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif
#include "snipfile.h"

void do_dir(char *, char *);

/*
**  Copy a floppy to an HD subdirectory
*/

int main(int argc, char *argv[])
{
      char fdrv[4] = "A:\\", target[FILENAME_MAX];

      if (3 > argc)
      {
            puts("Usage: FLOPCOPY drive_letter subdir");
            puts("where: drive_letter is \"A\" or \"B\" (colon optional)");
            puts("       subdir is drive:dir target, e.g. \"C:\\FLOPSTUF\"");
            return EXIT_FAILURE;
      }
      *fdrv = *argv[1];
      strcpy(target, argv[2]);
      if ('\\' != LAST_CHAR(target))
            strcat(target, "\\");
      
      do_dir(fdrv, target);
      return EXIT_SUCCESS;
}

/*
**  Process a directory (SNIPPETS: Treedir.C, modified)
*/

void do_dir(char *from, char *to)
{
      char search[FILENAME_MAX], new[FILENAME_MAX], newto[FILENAME_MAX];
      DOSFileData ff;

      strcat(strcpy(search, from), "*.*");
      if (Success_ == FIND_FIRST(to, _A_ANY, &ff))
      {
            if (0 == (ff_attr(&ff) & _A_SUBDIR))
            {
                  printf("*** %s Exists and is not a directory!\n", to);
                  return;
            }
      }
      else
      {
            strcpy(newto, to);
            if ('\\' == LAST_CHAR(newto))
                  LAST_CHAR(newto) = NUL;
            mkdir(newto);
      }
      if (Success_ == FIND_FIRST(search, _A_ANY, &ff)) do
      {
            if (ff_attr(&ff) & _A_SUBDIR && '.' != *ff_name(&ff))
            {
                  strcat(strcat(strcpy(new, from), ff_name(&ff)), "\\");
                  strcat(strcat(strcpy(newto, to), ff_name(&ff)), "\\");
                  do_dir(new, newto);
            }
            else
            {
                  char file1[FILENAME_MAX], file2[FILENAME_MAX];

                  if ((ff_attr(&ff) & (_A_SUBDIR | _A_VOLID)) ||
                        '.' == *ff_name(&ff))
                  {
                        continue;
                  }
                  strcat(strcpy(file1, from), ff_name(&ff));
                  strcat(strcpy(file2, to), ff_name(&ff));
                  if (Success_ != file_copy(file1, file2))
                        printf("*** Unable to copy %s to %s\n", file1, file2);
                  else  printf("Copied %s to %s\n", file1, file2);
            }
      } while (Success_ == FIND_NEXT(&ff));
}
