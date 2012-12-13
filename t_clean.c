/*
**  Remove all files older than a given date from the current directory
**  and (optionally) all subdirectories
**
**  public domain by Bob Stout
**
**  uses SCALDATE.C & FTIME.C from SNIPPETS
**  also uses TODAY.C & FDATE.C from SNIPPETS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <ctype.h>
#include <time.h>
#include "sniptype.h"
#include "dirport.h"
#include "ftime.h"
#include "scaldate.h"
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif

#if (defined(_MSC_VER) && (_MSC_VER >= 700)) || (defined(__SC__))
 /* Make FP_xxx macros lvalues as in older versions */
 #undef FP_SEG
 #undef FP_OFF
 #define FP_SEG(fp)    ((unsigned)((unsigned long)(fp) >> 16))
 #define FP_OFF(fp)    ((unsigned)(fp && 0xffff))
#endif

#define show(s) fputs((s), stderr)

/*
**  Switches and globals
*/

Boolean_T recurse = False_, gobble = False_;
Boolean_T ignore = False_, noaction = False_;

long kill_date = 0L;
FILE *log = NULL;
long file_date;

/*
**  Local prototypes
*/

static void usage(Boolean_T errstat);
static void clean_dir(char *path);
static void do_dir(char *path);
static void RMfile(char *fname);
static void RMdir(char *dname);
static int  log_printf(char *str, char *arg);

/*
**  T_CLEAN - Deletes all files and (optionally) subdirectories
*/

int main(int argc, char *argv[])
{
      int i, j, span;
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

                  case 'N':
                        noaction = True_;
                        break;

                  case '?':
                        fputs("*** Help ***\n", stderr);
                        usage(False_);
                        break;

                  case 'L':
                        if (0 == strlen(&argv[i][++j]))
                              log = stdout;
                        else
                        {
                              log = fopen(strupr(&argv[i][j]), "w");
                              if (NULL == log)
                              {
                                    fprintf(stderr, "*** Unable to open "
                                          "logfile, %s\n", &argv[i][j]);
                                    return Error_;
                              }
                        }
                        j += strlen(&argv[i][j]) - 1; /* End of switch  */
                        break;

                  case 'T':
                        if (0 == strlen(&argv[i][++j]))
                        {
                              fputs("*** No time specified ***\n", stderr);
                              usage(Error_);                /* Oops     */
                        }
                        span = atoi(&argv[i][j]);
                        kill_date = today() - (long)(span);
                        j += strlen(&argv[i][j]) - 1; /* End of switch  */
                        break;

                  default:
                        fprintf(stderr, "*** Bad switch - %c ***",
                              argv[i][j]);
                        usage(Error_);
                  }
            }
      }
      if (0L == kill_date)
            kill_date = today() - 14L;

      for (i = 1; i < argc; ++i)          /* Scan filenames             */
      {
            if (strchr("/-", *argv[i]))
                  continue;
            found_dir = True_;
            clean_func(argv[i]);
      }
      if (!found_dir)
      {
            fputs("*** No directory specified ***\n", stderr);
            usage(True_);
      }
      return EXIT_SUCCESS;
}

/*
**  Tell 'em they messed up
*/

static void usage(Boolean_T errstat)
{
      if (errstat)
            fputc('\a', stderr);
      show("Usage: T_CLEAN directory [...directory] [-tN] [-l[LOGFILE]] "
            "[-rgin?]\n");
      show("switches: -tN          Remove only files older than N days\n"
            "                       (default = 14 days)\n");
      show("          -l[LOGFILE]  Create named log file or (default) log to "
            "the console\n");
      show("          -r           Recurse subdirectories\n");
      show("          -g           Gobble (delete) empty subdirectories\n");
      show("          -i           Ignore special file attributes "
            "(CAUTION!)\n");
      show("          -n           No Action, display/log proposed "
            "deletions\n");
      show("          -?           Display help (this message)\n");
      exit(errstat);
}

/*
**  Clean all files from a directory
*/

static void clean_dir(char *path)
{
      char rmpath[FILENAME_MAX], *rmfile;
      DOSFileData fbuf;
      unsigned attrib = (ignore) ? _A_ANY : 0;

      strcpy(rmpath, path);
      if ('\\' != LAST_CHAR(rmpath))
            strcat(rmpath, "\\");
      rmfile = &rmpath[strlen(rmpath)];
      strcpy(rmfile, "*.*");
      if (0 == FIND_FIRST(rmpath, attrib, &fbuf)) do
      {
            strcpy(rmfile, ff_name(&fbuf));
            if (0 != getdatef(rmpath, &file_date))
            {
                  log_printf("Unable to get date of %s\n", rmpath);
                  continue;
            }
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
                              log_printf("Unable to ignore attributes "
                                    "of %s\n", rmpath);
                  }
            }
            if (kill_date > file_date)
                  RMfile(rmpath);
      } while (0 == FIND_NEXT(&fbuf));
}

/*
**  Process directories
*/

static void do_dir(char *path)
{
      char search[FILENAME_MAX], new[FILENAME_MAX];
      DOSFileData ff;

      strcpy(search, path);
      if ('\\' != LAST_CHAR(search))
            strcat(search, "\\");
      strcat(search, "*.*");
      if (Success_ == FIND_FIRST(search, _A_ANY, &ff)) do
      {
            if (ff.attrib & _A_SUBDIR && '.' != *ff_name(&ff))
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
            RMdir(path);
}

/*
**  File removal function - includes noaction switch and logging
*/

static void RMfile(char *fname)
{
      unsigned yr, mo, dy;

      if (!noaction)
            remove(fname);
      if (log)
      {
            scalar_to_ymd(file_date, &yr, &mo, &dy);
            fprintf(log, "%02d/%02d/%04d: %s\n", mo, dy, yr, fname);
      }
      else  fprintf(stderr, "Deleting %s\n", fname);
}

/*
**  Directory removal function - includes noaction switch and logging
*/

static void RMdir(char *dname)
{
      if (!noaction)
            rmdir(dname);
      if (log)
            fprintf(log, "Attempting to remove directory %s\n", dname);
}

/*
**  Error logging printf()
*/

static int log_printf(char *str, char *arg)
{
      FILE *out;

      if (log)
            out = log;
      else  out = stderr;

      return fprintf(out, str, arg);
}
