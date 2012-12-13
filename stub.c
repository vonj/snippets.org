/*
**  STUB.C - Utility to truncate files
**
**      STUB is used with MAKE utilities which lack the ability to timestamp
**      library object modules to reduce disk space requirements. After
**      compiling and building your library or executable, run "STUB *.OBJ"
**      to truncate all object files to zero length. The files' time and
**      date stamps will remain unchanged. By doing this, your make utility
**      will still know which modules are out of date even though the files
**      themselves have all been truncated to zero length. STUB also supports
**      the standard response file format so you can use your linker response
**      files to direct the files to be truncated.
**
**  public domain by Bob Stout
**
**  Notes: To expand command line arguments with wildcards,
**         TC/TC++/BC++  - Link in WILDARGS.OBJ.
**         MSC/QC        - Link in SETARGV.OBJ.
**         ZTC/C++       - Link in _MAINx.OBJ, where 'x' is the memory model.
**         Watcom C/C++  - Compile & link with WILDARGV.C
**
**         Allows file list(s) using standard "@file_list_name" convention.
*/

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "sniptype.h"

int fd;

void truncate(char *);

int main(int argc, char **argv)
{
      int i;

      if (2 > argc)
      {
            puts("Usage: STUB filespec [...filespec]");
            puts("where: filespec = fully-specified file name, or");
            puts("       filespec = wildcard-specified file name, or");
            puts("       filespec = response file name, e.g. \"@FILE.LST\"");
            return 1;
      }

      for (i = 1; i < argc; ++i)    /* Scan for simple file specs       */
      {
            if ('@' == *argv[i])
                  continue;
            else  truncate(argv[i]);
      }
      for (i = 1; i < argc; ++i)    /* Scan for response file specs     */
      {
            if ('@' == *argv[i])
            {
                  FILE *fp;
                  char buf[256], *ptr = &argv[i][1];

                  if (NULL == (fp = fopen(ptr, "r")))
                  {
                        printf("\aSTUB: Error opening %s\n", ptr);
                        return -1;
                  }
                  while (NULL != fgets(buf, 255, fp))
                  {
                        LAST_CHAR(buf) = '\0';  /* Strip '\n'           */
                        truncate(buf);
                  }
                  fclose(fp);
            }
      }
      return 0;
}

/*
**  The actual truncation function
*/

#ifdef __ZTC__
 #define GETFTIME dos_getftime
 #define SETFTIME dos_setftime
#else
 #define GETFTIME _dos_getftime
 #define SETFTIME _dos_setftime
#endif

#ifdef __WATCOMC__
 typedef unsigned short FTIME_T;
#else
 typedef unsigned int FTIME_T;
#endif

void truncate(char *fname)
{
#ifdef __TURBOC__
            struct ftime Ftime;
#else
            unsigned short date, time;
#endif

            fd = open(fname, O_WRONLY);

#ifdef __TURBOC__                         /* Save the time/date         */
            getftime(fd, &Ftime);
#else
            GETFTIME(fd, (FTIME_T *)&date, (FTIME_T *)&time);
#endif

            chsize(fd, 0L);               /* Truncate the file          */

#ifdef __TURBOC__                         /* Restore the time/date      */
            setftime(fd, &Ftime);
#else
            SETFTIME(fd, (FTIME_T)date, (FTIME_T)time);
#endif
            close(fd);
}
