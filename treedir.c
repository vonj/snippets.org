/*
**  TREEDIR.C - simple recursive directory lister
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <string.h>
#include "sniptype.h"
#include "dirport.h"

void do_dir(char *path)
{
      char search[67], newpath[67];
      DOSFileData ff;

      strcat(strcpy(search, path), "\\*.*");
      if (Success_ == FIND_FIRST(search, _A_ANY, &ff)) do
      {
            printf("%s\\%s\n", path, ff_name(&ff));
            if (ff_attr(&ff) & _A_SUBDIR && '.' != *ff_name(&ff))
            {
                  strcat(strcat(strcpy(newpath, path), "\\"), ff_name(&ff));
                  do_dir(newpath);
            }
      } while (Success_ == FIND_NEXT(&ff));
      FIND_END(&ff);
}

main()                  /* simple recursive current directory lister    */
{
      do_dir(".");
      return 0;
}
