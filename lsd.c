/*
** LSD - A simple directory lister
** A public domain C demo program by Bob Stout
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dirport.h"
#include "sniptype.h"

char *sprintfc(char *, long);
char *capitalize(const char *);
int   one_column(char *, char *, long, unsigned, DOSFileDate, DOSFileTime);
int   five_column(char *, char *);
 
/*
**  DOS DIR improved work-alike w/ improved formatting & attribute display
**
**  supports /W switch
*/
 
main(int argc, char *argv[])
{
      int i, files = 0, dirs = 0, argptr = 0, errflag = False_, cols, drive;
      long siz_tot = 0L;
      char *p, *fname, *ext, name[13], buf[67], numbuf[12];
      DOSFileData ff;
#ifndef __ZTC__
      struct diskfree_t df;
#endif
      int (*display)() = one_column;
 
      strcpy(buf, fname = "*.*");
      if(argc != 1)   for (i = 1; i < argc; ++i)
      {
            if ('/' == argv[i][0])
            {
                  if ('W' == toupper(argv[i][1]))
                        display = five_column;
                  else
                  {
                        puts("\aUsage: LSD [/W] [file]");
                        errflag = True_;
                  }
            }
            else if (!argptr)
                  argptr = i;
      }
      if (argptr)
      {
            fname = argv[argptr];
            strcpy(buf, fname);
            if ('\\' == LAST_CHAR(buf) || ':' == LAST_CHAR(buf))
                  strcat(buf, "*.*");
            else
            {
                  if (Success_ == FIND_FIRST(buf, _A_SUBDIR, &ff))
                  {
                        if (ff_attr(&ff) & _A_SUBDIR && '.' != *ff_name(&ff))
                        {
                              if ('\\' != LAST_CHAR(buf))
                                    strcat(buf, "\\");
                              strcat(buf, "*.*");
                        }
                  }
                  else  errflag = True_;
            }
      }
      if (':' == buf[1])
            drive = toupper(*buf) - '@';
      else  drive = 0;
      if (!errflag && !(FIND_FIRST(buf, _A_ANY, &ff))) do
      {
            siz_tot += ff_size(&ff);
            if (ff_attr(&ff) & _A_SUBDIR)
                  ++dirs;
            else  ++files;
            strcpy(name, ff_name(&ff));
            if (NULL != (p = strchr(name, '.')) && p != name)
            {
                  *p  = '\0';
                  ext = ++p;
            }
            else  ext = "";
            cols = (*display)(name, ext, ff_size(&ff),
                  ff_attr(&ff), ff_date(&ff), ff_time(&ff));
      } while (Success_ == FIND_NEXT(&ff));
      else
      {
            fprintf(stderr, "Cannot do directory on '%s'\n", fname);
            exit(-1);
      }
      FIND_END(&ff);
      if (cols)
            fputc('\n', stdout);
      sprintfc(numbuf,siz_tot);
      printf("\n%3d Files totalling %s bytes\n", files, numbuf);
      printf("%3d Director%s\n", dirs, (1 == dirs) ? "y" : "ies");
#ifndef __ZTC__
      _dos_getdiskfree(drive, &df);
      sprintfc(numbuf, (long)df.avail_clusters * df.sectors_per_cluster *
            df.bytes_per_sector);
#else /* if ZTC */
      sprintfc(numbuf, dos_getdiskfreespace(drive));
#endif
      printf("%s bytes free\n", numbuf);
      return 0;
}
 
/*
**  The single column directory entry display function
*/
 
int one_column(char        *name,
               char        *ext,
               long         size,
               unsigned     attribs,
               DOSFileDate  date,
               DOSFileTime  time)
{
      register int i, mask;
      static char *atr = "RHSVDA", szbuf[12];
 
      sprintfc(szbuf, size);
      printf("%-8s %-3s %12s  ", capitalize(name), capitalize(ext), szbuf);

      for (i = 0, mask = 1; i < 6; ++i, mask <<= 1)
            fputc((attribs & mask) ? atr[i] : '.', stdout);

      printf("%4d-%02d-%02d%4d:%02d:%02d\n",
            date.month,
            date.day,
            (date.year + 80) % 100,
            time.hours,
            time.mins,
            time.tsecs * 2);

      return 0;
}
 
/*
**  The five column directory entry display function
*/
 
int five_column(char *name, char *ext)
{
      static int cols = 0;

      printf("%-8s %-3s%s", capitalize(name), capitalize(ext),
            (5 > ++cols) ? "    " : "");
      if (0 == (cols %= 5))
            putchar('\n');
      return (cols);
}
 
/*
**  Display a long int using commas as thousands separators
*/
 
char *sprintfc(char *string, long num)
{
      if (num > 999999L)
      {
            sprintf(string, "%d,%03d,%03d",
                  (int)(num / 1000000L),
                  (int)((num % 1000000L) / 1000L),
                  (int)(num % 1000L));
      }
      else
      {
            if (num > 999L)
            {
                  sprintf(string, "%d,%03d",
                        (int)(num / 1000L),
                        (int)(num % 1000L));
            }
            else  sprintf(string, "%d", (int)num);
      }
      return string;
}
 
/*
**  Capitalize a name or extension in place
*/
 
char *capitalize(const char *string)
{
      int flag = 0;
      char *ptr = (char *)string;
 
      do
      {
            if (isalpha(*ptr))
            {
                  if (flag)
                        *ptr = (char)tolower(*ptr);
                  else
                  {
                        flag = 1;
                        *ptr = (char)toupper(*ptr);
                  }
            }
            else  flag = 0;
      } while (*++ptr);
      return (char *)string;
}
