/*
**  POSIX_LS.C - Directory lister using POSIX style processing
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  This subset version is functionally identical to the
**  version originally published by the author in Tech Specialist
**  magazine and is hereby donated to the public domain.
*/

#include <stdio.h>
#include "dirent.h"
#include "sniptype.h"

void dumpdir(char *dirname, char *mask)
{
      DIR *dirp;
      DOSFileData *dstruct;

      dirp = opendir(dirname);
      if (!dirp)
      {
            printf("Opening %s returned NULL\n\n", dirname);
            return;
      }
      printf("Dir %s has %d entries\n", dirname, dirp->dd_size);
      do
      {
            if (NULL != (dstruct = readdir(dirp)))
            {
                  if (Success_ == dirmask(dstruct, mask, NULL, _A_ANY, 0))
                        printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
            }
            else  puts("EOF\n");
      } while (dstruct);
      printf("seekdir( -1) returned %p\n", seekdir(dirp, -1, SEEK_SET));
      printf("seekdir(999) returned %p\n", seekdir(dirp, 999, SEEK_SET));
      printf("seekdir(0, SEEK_SET) returned %p\n", dstruct = seekdir(dirp,
            0, SEEK_SET));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(1, SEEK_SET) returned %p\n", dstruct = seekdir(dirp,
            1, SEEK_SET));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(4, SEEK_SET) returned %p\n", dstruct = seekdir(dirp,
            4, SEEK_SET));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(4, SEEK_CUR) returned %p\n", dstruct = seekdir(dirp,
            4, SEEK_CUR));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(-1, SEEK_CUR) returned %p\n", dstruct = seekdir(dirp,
            -1, SEEK_CUR));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(1, SEEK_CUR) returned %p\n", dstruct = seekdir(dirp,
            1, SEEK_CUR));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(0, SEEK_END) returned %p\n", dstruct = seekdir(dirp,
            0, SEEK_END));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(-1, SEEK_END) returned %p\n", dstruct = seekdir(dirp,
            -1, SEEK_END));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      printf("seekdir(-4, SEEK_END) returned %p\n", dstruct = seekdir(dirp,
            -4, SEEK_END));
      printf("%3d - %s\n", dirp->dd_loc, ff_name(dstruct));
      closedir(dirp);
}

main(int argc, char *argv[])
{
      char *mask = NULL, *dirname;

      if (1 < argc)
            dirname = argv[1];
      else  dirname = ".";

      if (2 < argc)
            mask = argv[2];
      printf("Calling dumpdir(%s, %s)\n\n", dirname, mask);
      dumpdir(dirname, mask);
      return 0;
}
