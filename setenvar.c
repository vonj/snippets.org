/*
**  SETENVAR.C - Program which sets the DOS master environment upon exit
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "sniptype.h"
#include "errors.h"                 /* For cant()                       */
#include "snipkbio.h"               /* For ungetkey() & KB_stuff()      */

main(int argc, char *argv[])
{
      FILE *bfile;

      if (3 > argc)
      {
            puts("\aUsage: SETENVAR envar datum");
            abort();
      }
      bfile = cant("$TMP$.BAT", "w");
      fprintf(bfile, "SET %s=%s\ndel $tmp$.bat\x1a", argv[1], argv[2]);
      fclose(bfile);
      while (kbhit())
            getch();
      KB_stuff("$tmp$\r");
      return 0;
}
