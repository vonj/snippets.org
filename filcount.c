/*
**  FILCOUNT.C - counts directories and /or files in a directory
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <string.h>
#include "sniptype.h"
#include "dirport.h"

unsigned DirCount = 0, FileCount = 0;

/*
**  Arguments: 1 - directory to search
**             2 - search subdirectories: True_ or False_
*/

void do_dir(char *path, int recurse_flag)
{
      char search[67], new[67];
      DOSFileData ff;

      strcpy(search, path);
      if ('\\' != LAST_CHAR(search))
            strcat(search, "\\");
      strcat(search, "*.*");
      if (Success_ == FIND_FIRST(search, _A_ANY, &ff)) do
      {
            if ('.' == *ff_name(&ff))
                  continue;
            if (ff_attr(&ff) & _A_SUBDIR)
            {
                  DirCount++;
                  if (recurse_flag)
                  {
                        strcpy(new, path);
                        if  ('\\' != LAST_CHAR(new))
                              strcat(new, "\\");
                        strcat(new, ff_name(&ff));
                        do_dir(new, recurse_flag);
                  }
            }
            else  FileCount++;
      } while (Success_ == FIND_NEXT(&ff));
}

/*
**  Simple recursive file/directory counter
**
**  Usage: FILCOUNT [path_name] [{Y | N}]
**
**  Notes: 1. If a path name isn't specified, the current directory is assumed
**         2. Default recursion flag is False_
**         3. Path must be specified in order to specify the recursion flag
*/

main(int argc, char *argv[])
{
      char *Dir =".";
      Boolean_T recurse = False_;

      if (1 < argc)
            Dir = argv[1];
      if (2 < argc)
            recurse = (NULL != strchr("Yy", *argv[2]));
      do_dir(Dir, recurse);
      printf("Counted: %d Directories and %d Files\n",
            DirCount, FileCount);
      return 0;
}
