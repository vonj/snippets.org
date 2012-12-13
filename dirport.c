/*
**  WIN 32 & OS/2 support functions for DIRPORT.H
**
**  Public domain by Jerry Coffin
**
**  15-May-96 David Nugent    Moved OS/2 functions from
**                            dirport.h to here
**                            Fixed for 32-bit OS/2
**                            Allowed compilation under
**                            DOS for easier makefiles
*/

#include "dirport.h"

#if defined(_WIN32) || defined (WIN32)

#if defined(__SC__)
 #include <win32/windows.h>
#else /* Assume Mingw32 */
 #include <windows.h>
#endif

WORD ff_date(DOSFileData *f)
{
      WORD DOS_date;
      WORD DOS_time;

      FileTimeToDosDateTime(&(f->file.ftLastWriteTime), &DOS_date, &DOS_time);

      return DOS_date;
}

WORD ff_time(DOSFileData *f)
{
      WORD DOS_date;
      WORD DOS_time;

      FileTimeToDosDateTime(&(f->file.ftLastWriteTime), &DOS_date, &DOS_time);

      return DOS_time;
}

WORD ff_yr(DOSFileData *f)
{
      SYSTEMTIME t;

      FileTimeToSystemTime(&(f->file.ftLastWriteTime), &t);

      return t.wYear;
}

WORD ff_mo(DOSFileData *f)
{
      SYSTEMTIME t;

      FileTimeToSystemTime(&(f->file.ftLastWriteTime), &t);

      return t.wMonth;
}


WORD ff_day(DOSFileData *f)
{
      SYSTEMTIME t;

      FileTimeToSystemTime(&(f->file.ftLastWriteTime), &t);

      return t.wDay;
}

WORD ff_hr(DOSFileData*f)
{
      SYSTEMTIME t;

      FileTimeToSystemTime(&(f->file.ftLastWriteTime), &t);

      return t.wHour;
}

WORD ff_min(DOSFileData *f)
{
      SYSTEMTIME t;

      FileTimeToSystemTime(&(f->file.ftLastWriteTime), &t);

      return t.wMinute;
}

WORD ff_tsec(DOSFileData *f)
{
      SYSTEMTIME t;

      FileTimeToSystemTime(&(f->file.ftLastWriteTime), &t);

      return t.wMilliseconds / 100;
}

int FIND_FIRST(char *spec, unsigned attrib, DOSFileData *ff)
{
      ff_attr(ff) = attrib | _A_ARCH;

      ff->handle = FindFirstFile(spec, &(ff->file));

      if ( INVALID_HANDLE_VALUE == ff->handle ) {
            return 1;
      }

      while ( 0 != (ff->file.dwFileAttributes & ~ff_attr(ff)))
      {
            if ( FALSE == FindNextFile(ff->handle, &(ff->file))) {
                  return 1;
            }
      }
      return 0;
}

int FIND_NEXT(DOSFileData *ff)
{
      do {
            if ( FALSE == FindNextFile(ff->handle, &(ff->file))) {
                  return 1;
            }

      } while ( 0 != (ff->file.dwFileAttributes & ~ff_attr(ff)));
      return 0;
}

int FIND_END(DOSFileData *ff)
{
    return (int)FindClose(ff->handle);
}

#elif defined(__unix__)

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include "filnames.h"
#include "minmax.h"

static char path[FILENAME_MAX];
static char fname[FILENAME_MAX];

int FIND_FIRST(char *spec, unsigned attrib, DOSFileData *ff)
{
      char cwd[FILENAME_MAX];

      ff->f.d_attr = attrib;
      fnSplit(spec, NULL, NULL, ff->f.d_path, ff->f.d_fname, NULL, NULL);

      if (!strlen(ff->f.d_path))
            strcpy(ff->f.d_path, getcwd(cwd, FILENAME_MAX));
//      printf("FIND_FIRST: path=%s, fname=%s\n", ff->f.d_path, ff->f.d_fname);

      if (NULL == (ff->dirp = opendir(ff->f.d_path)))
      {
//            printf("opendir(%s) failed\n", path);
            return 1;
      }

      return FIND_NEXT(ff);
}

int FIND_NEXT(DOSFileData *ff)
{
      struct dirent *uffblk;
      struct stat stbuf;
      unsigned short fmode;
      char fullname[FILENAME_MAX];
      time_t filetime;
      struct tm *filetm;
      char line[256], *p;
      int retval;
      Boolean_T match;

      if ( NULL != (uffblk = readdir(ff->dirp)))
      {
//            printf("FIND_NEXT: readdir() found %s\n", uffblk->d_name);

            do
            {
                  if (Success_ == strcmp(uffblk->d_name, ".") || Success_ == strcmp(uffblk->d_name, ".."))
                  {
//                        printf("FIND_NEXT: Dot directory \"%s\" found - skipping\n", uffblk->d_name);
                        continue;
                  }
                  strcpy(ff_name(ff), uffblk->d_name);
                  fnMerge(fullname, NULL, NULL, ff->f.d_path, ff_name(ff), NULL, NULL);
//                  printf("FIND_NEXT: fullname=%s\n", fullname);

                  retval = stat(fullname, &stbuf);
                  if (Success_ != retval)
                  {
//                        sprintf(line, "FIND_NEXT: stat(%s) failed:\n", fullname);
                        perror(line);
                        return 1;
                  }
                  ff_size(ff) = stbuf.st_size;
                  fmode = (unsigned short)(stbuf.st_mode & 0177777);
//                  printf("FIND_NEXT: fmode=%ho\n", fmode);
                  ff_attr(ff)        = _A_ARCH;
                  if ('.' == ff_name(ff)[0])
                        ff_attr(ff) |= _A_HIDDEN;     
                  if (S_IXUSR & fmode)
                        ff_attr(ff) |= _A_EXECUTABLE;
                  if (!(S_IWUSR & fmode))
                        ff_attr(ff) |= _A_RDONLY;
                  if (S_ISDIR(fmode))
                        ff_attr(ff) |= _A_SUBDIR;
                  if (!S_ISREG(fmode))
                        ff_attr(ff) |= _A_SYSTEM;

                  filetime = max(stbuf.st_mtime, stbuf.st_ctime);
                  filetm   = localtime(&filetime);
                  ff_yr(ff)    = filetm->tm_year - 80;
                  ff_mo(ff)    = filetm->tm_mon + 1;
                  ff_day(ff)   = filetm->tm_mday;
                  ff_hr(ff)    = filetm->tm_hour;
                  ff_min(ff)   = filetm->tm_min;
                  ff_sec(ff)   = filetm->tm_sec / 2;

//                  if (NULL == (p = strrchr(ffpassed.spec, '/')))
//                        p = ffpassed.spec;
//                  else  ++p;
                  match = xstrcmp(ff->f.d_fname, uffblk->d_name);
//                  printf("xstrcmp(%s,%s) returned %s\n", ff->f.d_fname, uffblk->d_name,
//                         match ? "TRUE" : "FALSE");
                  if (match && ff_attr(ff) & ff->f.d_attr)
                        break;

            } while (NULL != (uffblk = readdir(ff->dirp)));
      }
      else  return 1;

      return 0;
}

int FIND_END(DOSFileData *ff)
{
      return closedir(ff->dirp);
}

#endif      /* O.S.     */
