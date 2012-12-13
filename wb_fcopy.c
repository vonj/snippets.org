/*
** by: Walter Bright via Usenet C newsgroup
**
** modified by: Bob Stout, Ray Gardner, and David Gersic
**
** There is no point in going to asm to get high speed file copies. Since it
** is inherently disk-bound, there is no sense (unless tiny code size is
** the goal). Here's a C version that you'll find is as fast as any asm code
** for files larger than a few bytes (the trick is to use large disk buffers):
*/

#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include "snipfile.h"

#if !defined(__ZTC__) && !defined(__TURBOC__)
 #include <sys\types.h>
#endif

#include <sys\stat.h>

int file_copy(char *from, char *to)
{
      int fdfrom,fdto;

      fdfrom = open(from,O_RDONLY|O_BINARY,0);
      if (fdfrom < 0)
            return Error_;

      /* Open R/W by owner, R by everyone else        */

      fdto=open(to,O_BINARY|O_CREAT|O_TRUNC|O_RDWR,S_IREAD|S_IWRITE);
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

int fdcopy(int fdfrom, int fdto)
{
      int bufsiz, retval = Error_;

      /* Use the largest buffer we can get    */

      for (bufsiz = 0x4000; bufsiz >= 128; bufsiz >>= 1)
      {
            register char *buffer;

            buffer = (char *) malloc(bufsiz);
            if (buffer)
            {
                  while (1)
                  {
                        register int n;

                        n = read(fdfrom,buffer,bufsiz);
                        if (n == -1)            /* if error             */
                              break;
                        if (n == 0)             /* if end of file       */
                        {
                              retval = Success_;
                              break;
                        }
                        if (n != write(fdto,buffer,(unsigned) n))
                              break;
                  }
                  free(buffer);
                  break;
            }
      }
      return retval;
}
