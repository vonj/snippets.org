/*
**  STRIPEOF.C
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#define BUFSIZE 16384

int main(int argc, char *argv[])
{
      char *buf;

      if (2 > argc)
      {
            puts("Usage: STRIPEOF filename1 [...filenameN]");
            return EXIT_FAILURE;
      }
      if (NULL == (buf = malloc(BUFSIZE)))
      {
            puts("STRIPEOF internal failure");
            return EXIT_FAILURE;
      }
      while (--argc)
      {
            int fd;
            size_t bytes;
            int found = 0;
            long zpos = 0L;

            if (-1 == (fd = open(*(++argv), O_RDWR | O_BINARY)))
            {
                  printf("Couldn't open %s\n", *argv);
                  return EXIT_FAILURE;
            }
            while (0 < (bytes = read(fd, buf, BUFSIZE)))
            {
                  int i;

                  for (i = 0; i < (int)bytes; ++i)
                  {
                        if (('Z' - 64) == buf[i])
                        {
                              found = 1;
                              zpos += i;
                              break;
                        }
                  }
                  if (found)
                        break;
                  zpos += bytes;
            }
            if (found)
                  chsize(fd, zpos);
      }
      return EXIT_SUCCESS;
}
