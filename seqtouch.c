/*
**  SEQTOUCH.C - Touch files in a directory with sequential time stamps.
**
**  Public domain by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#ifdef __TURBOC__
 #include <dir.h>
#else
 #include <direct.h>
#endif
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif
#include "ftime.h"
#include "datetime.h"
#include "getopts.h"
#include "errors.h"
#include "snipfile.h"
#include "dosfiles.h"
#include "snipkbio.h"
#include "dirport.h"

/*
**  Options area - uses getopts() from SNIPPETS.
*/

char fname[FILENAME_MAX] = "";
char pname[FILENAME_MAX] = "";
char datestr[40];
char timestr[20];

Boolean_T help    = False_;
Boolean_T noquery = False_;

struct Option_Tag options[] = {
      {'F',  False_, String_Tag,  fname,    NULL, NULL, NULL},
      {'P',  False_, String_Tag,  pname,    NULL, NULL, NULL},
      {'D',  False_, String_Tag,  datestr,  NULL, NULL, NULL},
      {'T',  False_, String_Tag,  timestr,  NULL, NULL, NULL},
      {'H',  False_, Boolean_Tag, &help,    NULL, NULL, NULL},
      {'N',  False_, Boolean_Tag, &noquery, NULL, NULL, NULL},
      {'\0', False_, Error_Tag,   NULL,     NULL, NULL, NULL}
};

void loadtm(struct tm *ftm, struct ftime *ftimep)
{
      ftimep->ft_tsec  = ftm->tm_sec / 2;
      ftimep->ft_min   = ftm->tm_min;
      ftimep->ft_hour  = ftm->tm_hour;
      ftimep->ft_day   = ftm->tm_mday;
      ftimep->ft_month = ftm->tm_mon + 1;
      ftimep->ft_year  = ftm->tm_year - 80;
}

void usage(int errlvl)
{
      puts("SEQTOUCH - A utility to process a directory so its files "
           "have sequentially");
      puts("           increasing time/date stamps\n");
      puts("Usage: SEQTOUCH [-N] [-Ddir] [-Ffile] [-Dmm-dd-yy] "
           "[-Thh:mm:ss]\n");
      puts("Where: <dir> is the directory to process");
      puts("       <file> is an existing file whose time/date stamp "
           "to use");
      puts("       <mm-dd-yy> is a date in month-day-year format");
      puts("       <hh:mm:ss> is a time in hours:minutes:seconds "
           "format\n");
      puts("       <-N> suppresses verification of arguments\n");
      puts("All are optional. Specifying SEQTOUCH with no arguments "
           "causes all files in");
      puts("the current directory to be dated to the current date. "
           "The timestamp of the");
      puts("first file will be set to the current date and time and "
           "each subsequent file");
      puts("will have its time/date stamp set to 2 seconds later "
           "than the preceding file.\n");
      puts("Specifying the -T and/or -D options sets the time and/or date "
           "of the first");
      puts("file in the directory to the speified time and/or date.\n");
      puts("Specifying a file with the -F option causes the timestamp of "
           "first file in ");
      puts("the directory to be set to match the specified file.\n");
      puts("If the -N option is not specified, you will br prompted with "
           "the specific");
      puts("directory, time, and date (and file, if specified) to be used.");
      puts("The option switches are not case sensitive, i.e. -f works just "
           "like -F.");
      exit(errlvl);
}

int main(int argc, char *argv[])
{
      struct ftime ftimep;
      struct tm ftm;
      time_t ft;
      DOSFileData ff;
      char str[50];
      char path[FILENAME_MAX];
      char file[FILENAME_MAX];
      size_t plen;

      if (Error_ == getopts(argc, argv))
            usage(EXIT_FAILURE);

      if (help)
            usage(EXIT_SUCCESS);

      if (NUL != pname[0])
      {
            if (!isdir(pname))
                  ErrExit("%s is not a directory", pname);
      }
      else  getcwd(pname, FILENAME_MAX);

      if (NUL != fname[0])
      {
            FILE *fp;

            fp = cant(fname, "r");
            if (Success_ != getftime(fileno(fp), &ftimep))
                  ErrExit("Can't read timestamp of %s", fname);
            ftime2tm(&ftimep, &ftm);
            ft = ftime2time(&ftimep);
      }
      else
      {
            time(&ft);
            ftm = *localtime(&ft);

            if (NUL != datestr[0])
            {
                  unsigned yy, mm, dd;

                  if (Success_ != parse_date(datestr, &yy, &mm, &dd, USA))
                        ErrExit("Invalid date - %s", datestr);
                  ftm.tm_year = yy - 1900;
                  ftm.tm_mon  = mm - 1;
                  ftm.tm_mday = dd;
            }

            if (NUL != timestr[0])
            {
                  unsigned  hh, mm, ss;

                  if (Success_ != parse_time(timestr, &hh, &mm, &ss))
                        ErrExit("Invalid time - %s", timestr);
                  ftm.tm_hour = hh;
                  ftm.tm_min  = mm;
                  ftm.tm_sec  = ss;
            }

            loadtm(&ftm, &ftimep);
      }

      printf("\nDirectory %s\n", pname);
      if (NUL == fname[0])
            puts("No file specified\n");
      else  printf("Using file %s\n\n", fname);
      strftime(str, 50, "%A, %d-%b-%Y, %X", &ftm);
      printf("Files will be timstamped sequentially beginning with\n  %s\n",
             str);
      if (!noquery)
      {
            if (False_ == getYN("OK?", 'N', 5))
            {
                  fputs("\nSEQTOUCH aborted by user\n", stderr);
                  return EXIT_SUCCESS;
            }
      }

      strcpy(path, pname);
      if ('\\' != LAST_CHAR(path))
            strcat(path, "\\");
      plen = strlen(path);
      strcpy(file, path);

      strcat(path, "*.*");
      if (Success_ == FIND_FIRST(path, _A_NORMAL, &ff)) do
      {
            FILE *fp;

            file[plen] = NUL;
            strcat(file, ff_name(&ff));

            fprintf(stderr, "Touching %s\n", file);

            fp = cant(file, "r+");
            setftime(fileno(fp), &ftimep);
            fclose(fp);

            ftm.tm_sec += 2;
            ft = mktime(&ftm);            /* Used to normalize time     */
            loadtm(&ftm, &ftimep);
      } while (Success_ == FIND_NEXT(&ff));

      return EXIT_SUCCESS;
}
