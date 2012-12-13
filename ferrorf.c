/*
**  FERRORF.C - Functions for formatted error messages
*/

/*
**  ferrorf()
**
**  Prints error message with printf() formatting syntax, then a colon,
**  then a message corresponding to the value of errno, then a newline.
**  Output is to filehandle.
**
**  Public Domain by Mark R. Devlin, free usage is permitted.
*/
 
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "errors.h"
 
int ferrorf(FILE *filehandle, const char *format, ...)
{
      int vfp, fp;
      va_list vargs;
 
      vfp = fp = 0;
      va_start(vargs, format);
      vfp = vfprintf(filehandle, format, vargs);
      va_end(vargs);
      fp = fprintf(filehandle, ": %s\n", sys_errlist[errno]);
      return ((vfp==EOF || fp==EOF) ? EOF : (vfp+fp));
}

/*
**  cant() - An fopen() replacement with error trapping
**
**  Call just as you would fopen(), but make sure your exit functions are
**  registered with atexit().
**
**  public domain by Bob Stout
*/

FILE *cant(char *fname, char *fmode)
{
      FILE *fp;

      if (NULL == (fp = fopen(fname, fmode)))
      {
            ferrorf(stderr, "Can't open %s", fname);
            exit(EXIT_FAILURE);
      }
      return fp;
}

#ifdef TEST

main()
{
      char badname[] = "????????.???";
      FILE *fp;

      fp = cant(badname, "r");
      return 0;
}

#endif /* TEST */
