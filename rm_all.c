/*
**  Remove all files and (optionally) subdirectories
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <ctype.h>
#include "sniptype.h"
#include "dirport.h"
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif

#define MAX_PATH 80

#if (defined(_MSC_VER) && (_MSC_VER >= 700)) || (defined(__SC__))
 /* Make FP_xxx macros lvalues as in older versions */
 #undef FP_SEG
 #undef FP_OFF
 #define FP_SEG(fp)    ((unsigned)((unsigned long)(fp) >> 16))
 #define FP_OFF(fp)    ((unsigned)(fp && 0xffff))
#endif

/* Select one of the following - remove() is ANSI       */

#define rmfunc remove
/* #define rmfunc unlink */

#define show(s) fputs((s), stderr)

Boolean_T recurse = False_, gobble = False_, ignore = False_;

char *mask = "*.*";

/*
**  Clean all files from a directory
*/

void clean_dir(char *path)
{
      char rmpath[MAX_PATH], *rmfile;
      DOSFileData fbuf;
      unsigned attrib = (ignore) ? _A_ANY : 0;

      strcpy(rmpath, path);
      if ('\\' != LAST_CHAR(rmpath))
            strcat(rmpath, "\\");
      rmfile = &rmpath[strlen(rmpath)];
      strcpy(rmfile, mask);
      if (0 == FIND_FIRST(rmpath, attrib, &fbuf)) do
      {
            strcpy(rmfile, ff_name(&fbuf));
            if (ignore)
            {
                  union REGS regs;
                  struct SREGS sregs;

                  regs.x.ax = 0x4300;
                  regs.x.dx = FP_OFF((char FAR *)rmpath);
                  segread(&sregs);
                  sregs.ds  = FP_SEG((char FAR *)rmpath);
                  intdosx(&regs, &regs, &sregs);
                  if (!regs.x.cflag)
                  {
                        regs.x.ax  = 0x4301;
                        regs.x.cx &= ~(_A_RDONLY | _A_HIDDEN | _A_SYSTEM);
                        intdosx(&regs, &regs, &sregs);
                        if (regs.x.cflag)
                              printf("unable to delete %s\n", rmpath);
                  }
            }
            rmfunc(rmpath);
            printf("deleting %s\n", rmpath);
      } while (0 == FIND_NEXT(&fbuf));
}

/*
**  Process directories
*/

void do_dir(char *path)
{
      char search[MAX_PATH], new[MAX_PATH];
      DOSFileData ff;

      strcpy(search, path);
      if ('\\' != LAST_CHAR(search))
            strcat(search, "\\");
      strcat(search, "*.*");
      if (Success_ == FIND_FIRST(search, _A_ANY, &ff)) do
      {
            if (ff_attr(&ff) & _A_SUBDIR && '.' != *ff_name(&ff))
            {
                  strcpy(new, path);
                  if ('\\' != LAST_CHAR(new))
                        strcat(new, "\\");
                  strcat(new, ff_name(&ff));
                  do_dir(new);
            }
      } while (Success_ == FIND_NEXT(&ff));
      clean_dir(path);
      if (gobble)
            rmdir(path);
}

/*
**  Tell 'em they messed up
*/

void usage(Boolean_T errstat)
{
      if (errstat)
            fputc('\a', stderr);
      show("Usage: RM_ALL directory [...directory] [-eFNAME.EXT] [-rgi?]\n");
      show("switches: -eFNAME.EXT  Remove only files matching mask "
            "(default is \"-e*.*\")\n");
      show("          -r           Recurse subdirectories\n");
      show("          -g           Gobble (delete) empty subdirectories\n");
      show("          -i           Ignore special file attributes "
            "(CAUTION!)\n");
      show("          -?           Display help (this message)\n");
      exit(errstat);
}

/*
**  RM_ALL - Deletes all files and (optionally) subdirectories
*/

int main(int argc, char *argv[])
{
      int i, j;
      Boolean_T found_dir = False_;
      void (*clean_func)(char *) = clean_dir;

      for (i = 1; i < argc; ++i)          /* Check for switches         */
      {
            if (NULL == strchr("-/", *argv[i]))
                  continue;               /* Assume it's a filename     */
            for (j = 1; argv[i][j] ; ++j) /* Traverse nested switches   */
            {
                  switch (toupper(argv[i][j]))
                  {
                  case 'R':
                        clean_func = do_dir;
                        break;

                  case 'G':
                        gobble = True_;
                        break;

                  case 'I':
                        ignore = True_;
                        break;

                  case '?':
                        puts("***help***");
                        usage(False_);
                        break;

                  case 'E':
                        if (0 == strlen(&argv[i][++j]))
                        {
                              puts("***no file***");
                              usage(Error_);                /* Oops     */
                        }
                        mask = strupr(&argv[i][j]);
                        j += strlen(&argv[i][j]) - 1; /* End of switch  */
                        break;

                  default:
                        puts("***default***");
                        usage(Error_);
                  }
            }
      }
      for (i = 1; i < argc; ++i)          /* Scan filenames             */
      {
            if (strchr("/-", *argv[i]))
                  continue;
            found_dir = True_;
            clean_func(argv[i]);
      }
      if (!found_dir)
      {
            puts("***not found***");
            usage(True_);
      }
      return 0;
}
