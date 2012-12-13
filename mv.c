/*
** mv.c -- move or rename files or directories  
** updated for multiple files, 5 jul 92, rlm    
** placed in the public domain via C_ECHO by the author, Ray McVay
**
** modified by Bob Stout, 28 Mar 93
** modified by Bob Stout,  4 Jun 93
**
** uses file_copy from SNIPPETS file WB_FCOPY.C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "dirport.h"
#include "snipfile.h"
#include "dosfiles.h"
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif

/*
**  Tell 'em they messed up
*/

void help(char *s)
{
      puts("usage: mv <oldname [...]> <newname|newdir>");
      printf("error: %s\n", s);
}

/*
**  Simple directory test
*/

isdir(char *path)
{
      DOSFileData f;

      /* "Raw" drive specs are always directories     */

      if (':' == path[1] && '\0' == path[2])
            return 1;

      return (Success_ == FIND_FIRST(path, _A_SUBDIR, &f) &&
            (ff_attr(&f) & _A_SUBDIR));
}

/*
**  Use rename or copy and delete
*/

int mv(char *src, char *dest)
{
      int errcount = 0;
      char buf[FILENAME_MAX];
      const char *generr = "ERROR: mv - couldn't %s %s %s\n";

      if (':' == dest[1] && *dest != *getcwd(buf, FILENAME_MAX))
      {
            if (file_copy(src, dest))
            {
                  printf(generr, "move", src, dest);
                  ++errcount;
            }
            else if (remove(src))
            {
                  printf(generr, "delete", src, "");
                  ++errcount;
            }
      }
      else
      {
            if (rename(src, dest))
            {
                  printf(generr, "rename", src, dest);
                  ++errcount;
            }
      }
      return errcount;
}

/*
**  Enter here
*/

int main(int argc, char **argv)
{
      int src, errcount = 0;
      char target[FILENAME_MAX];

      puts("mv 1.3 (4 jun 93) - Ray L. McVay/Bob Stout");
      if (argc < 3)
            help("Not enough parameters");

      /*
      **  Handle cases where target is a directory
      */

      else if (isdir(argv[argc -1]))
      {
            for (src = 1; src < argc - 1; src++)
            {
                  char termch;

                  strcpy(target, argv[argc - 1]);
                  termch = target[strlen(target) - 1];
                  if ('\\' != termch && ':' != termch)
                        strcat(target, "\\");

                  if (strrchr(argv[src], '\\'))
                        strcat(target, strrchr(argv[src], '\\') + 1);
                  else if (argv[src][1] == ':')
                        strcat(target, argv[src] + 2);
                  else  strcat(target, argv[src]);

                  errcount += mv(argv[src], target);
            }
      }

      /*
      **  Nothing left except 2 explicit file names
      */

      else if (argc == 3)
            errcount += mv(argv[1], argv[2]);

      return errcount;
}
