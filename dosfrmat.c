/*
**  FORMAT.C - Use DOS FORMAT to format a diskette
**
**  Original Copyright 1992 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/
 
#include <stdio.h>
#include <stdlib.h>
#include "snpdosys.h"
 
/*
**  format
**
**  Formats a specified floppy disk with optional switches.
**
**  Parameters: 1 - Drive letter ('A', 'B', ...) to format
**              2 - Formatting switches in FORMAT.COM format, e.g. "/4"
**              3 - Volume label
**
**  Returns: Success_ or Error_
*/
 
int format(char drive, char *switches, char *vlabel)
{
      char command[128], fname[13];
      FILE *tmpfile;
 
      tmpnam(fname);
      if (NULL == (tmpfile = fopen(fname, "w")))
            return Error_;                       /* Can't open temp file */
      fprintf(tmpfile, "\n%s\nN\n", vlabel);
      fclose(tmpfile);
 
      sprintf(command, "format %c: /V %s < %s > NUL", drive, switches, fname);
 
      system(command);
 
      remove(fname);
 
      return Success_;
}
 
#ifdef TEST
 
main()
{
      int retval = format((char)'a', "/4", "dummy_test");
 
      printf("format() returned %d\n", retval);
      return 0;
}
 
#endif /* TEST */
