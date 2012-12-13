/*
** by: Walter Bright via Usenet C newsgroup
**
** modified by: Bob Stout based on a recommendation by Ray Gardner
**
** modified by: David Gersic to deal with binary files
**
** There is no point in going to asm to get high speed file copies. Since it
** is inherently disk-bound, there is no sense (unless tiny code size is
** the goal). Here's a C version that you'll find is as fast as any asm code
** for files larger than a few bytes (the trick is to use large disk buffers):
*/

#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include "snipfile.h"               /* Contains prototype for fdcopy()  */

#if !defined(__ZTC__) && !defined(__TURBOC__)
 #include <sys\types.h>
#endif

#include <sys\stat.h>

int file_append(char *from, char *to)
{
      int fdfrom,fdto;

      fdfrom = open(from,O_RDONLY|O_BINARY,0);
      if (fdfrom < 0)
            return 1;

      /* Open R/W by owner, R by everyone else        */

      fdto=open(to,O_BINARY|O_CREAT|O_APPEND|O_RDWR,S_IREAD|S_IWRITE);
      if (fdto >= 0)
      {
            if (Success_ == fdcopy(fdfrom, fdto))
            {
                  close(fdto);
                  close(fdfrom);
                  return Success_;
            }
            else
            {
                  close(fdto);
                  remove(to);             /* delete any partial file  */
            }
      }
      close(fdfrom);
      return Error_;
}
