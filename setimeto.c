/*
**  SETIMETO.C - Set the timestamp of one file to match another.
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "ftime.h"

int main(int argc, char *argv[])
{
      int fd0, fd1;

      struct ftime Ftime;

      if (3 > argc)
      {
            puts("Usage: SETIMETO filename_w/original_stamp "
                  "filename_w/stamp_to_set");
            return EXIT_FAILURE;
      }

      if (-1 == (fd0 = open(argv[1], O_RDONLY)))
      {
            printf("Unable to open %s\n", argv[1]);
            return EXIT_FAILURE;
      }

      getftime(fd0, &Ftime);              /* Save the time/date         */

      if (-1 == (fd1 = open(argv[2], O_WRONLY)))
      {
            printf("Unable to open %s\n", argv[2]);
            return EXIT_FAILURE;
      }

      setftime(fd1, &Ftime);              /* Set the time/date          */

      close(fd0);
      close(fd1);
      return EXIT_SUCCESS;
}
