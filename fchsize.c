/*
** Routines for truncating DOS files associated by streams.
**
** Author   : Eric Coolman, Simple Minded Software
**            modified for SNIPPETS by Bob Stout
**
** Released to public domain on June 23, 1994.  USE AT OWN RISK.
**
** Notes:     fChSize works the same with FILE pointers as CHSIZE() does for
**            file handles.
**
**            All routines return the same values as CHSIZE().
*/


#include <stdio.h>                  /* fileno(), ftell(), FILE *        */
#include <io.h>                     /* chsize()                         */
#include "errors.h"                 /* cant()                           */

#if defined(__ZTC__) && !defined(__SC__)
 #include "sniptype.h"              /* Error_, Success_                 */

 int chsize(int fd, long posn)
 {
       char dummy;

       if (-1L == lseek(fd, posn, SEEK_SET))
             return Error_;
       if (-1 == write(fd, &dummy, 0))
             return Error_;
       else    return Success_;
 }
#endif

/*
** Expand or reduce the size of a file
*/

int fChSize(FILE *stream, long size)
{
      return(chsize(fileno(stream), size));
}

/*
** Truncates the file at the current offset of the FILE pointer
*/

int fTrunc(FILE *stream)
{
      return(fChSize(stream, ftell(stream)));
}

/*
** Clears contents of stream (zero bytes it)
*/

int fStub(FILE *stream)
{
      return fChSize(stream, 0L);
}


#ifdef TEST

#include <stdlib.h>     /* atol()         */
#include <ctype.h>      /* toupper()      */

void usage(void)
{
      puts("Usage: FCHSIZE { C | S } filename [new_length]\n");
      puts("Where: C  means change the file size, "
           "new size given by new_length");
      puts("       S  means truncate the file to zero bytes");
      exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
      FILE *testfile;
      long posn;

      if (argc < 3)
            usage();

      testfile = cant(argv[2], "r+b");

      switch (toupper(argv[1][0]))
      {
      case 'C':
            posn    = atol(argv[3]);
            printf("fChSize(%s, %ld) returned %d\n", argv[1], posn,
                   fChSize(testfile, posn));
            break;

      case 'S':
            printf("fStub(%s) returned %d\n", argv[1], fStub(testfile));
            break;

      default:
            usage();
      }

      fclose(testfile);
      return EXIT_SUCCESS;
}

#endif /* TEST */
