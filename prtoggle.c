/*
**  prtoggle()
**
**  Tee's all standard output to the printer.
**
**  Parameters: None
**
**  Returns:  0 if operation was successful.
**           -1 if stdout or stdin is redirected.
**
**  Side effects: Flushes the keyboard buffer
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include "sniptype.h"
#include "sniprint.h"
#include "snipkbio.h"

int prtoggle(void)
{
      if (!isatty(fileno(stdin)) || !isatty(fileno(stdout)))
            return -1;
      while (kbhit())           /* Flush the keyboard buffer            */
            getch();
      ungetkey('P' - 64);       /* Stuff a Ctrl-P into the buffer       */
      system("");               /* Let COMMAND.COM do the work          */
      return 0;
}
