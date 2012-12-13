/************************************************************************/
/*                                                                      */
/*  FUPDATE.C                                                           */
/*                                                                      */
/*  Functions to flush  disk buffers to disk.                           */
/*                                                                      */
/*  Original Copyright 1990-93 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include "dosfiles.h"

/*
**  fdupdate()
**
**  Update a file by handle reference.
**
**  Arguments: 1 - Handle of file to update
**
**  Returns: Success_ or error code
*/

int fdupdate(int fd)
{
      if (3 > _osmajor || (3 == _osmajor && 3 > _osminor))
      {
            return close(dup(fd));
      }
      else
      {
            union REGS regs;

            regs.h.ah = 0x68;
            regs.x.bx = fd;
            intdos(&regs, &regs);
            if (regs.x.cflag)
                  return regs.x.ax;
            else  return Success_;
      }
}

/*
**  fupdate()
**
**  Update a C buffered file.
**
**  Arguments: 1 - FILE pointer of file to update
**
**  Returns: Success_ or error code
*/

int fupdate(FILE *fp)
{
      fflush(fp);
      return fdupdate(fileno(fp));
}
