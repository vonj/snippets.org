/*
**  SPLIT.C - A utility to split large text files into smaller files
**
**  public domain by Bob Stout
**
**  uses FNSPLIT.C from SNIPPETS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extkword.h"
#include "filnames.h"

int main(int argc, char *argv[])
{
      long newsize = 32L * 1024L;
      size_t seq = 0;
      char fname[FILENAME_MAX];
      FILE *from;

      if (2 > argc)
      {
            puts("SPLIT big_file [size_in_K]\n");
            puts("creates files of the same name, "
                  "but with numeric extensions");
            puts("a maximum file size may be specified for new files");
            return EXIT_SUCCESS;
      }
      if (2 < argc)
      {
            newsize   = atol(argv[2]);
            newsize <<= 10;
      }
      if (NULL == (from = fopen(argv[1], "r")))
      {
            printf("\aSPLIT: error - can't open %s\n", argv[1]);
            return EXIT_FAILURE;
      }
      fnSplit(argv[1], NULL, NULL, NULL, NULL, fname, NULL);
      while (!feof(from))
      {
            char newname[FILENAME_MAX], buf[1024];
            FILE *to;
            long bytes;

            sprintf(newname, "%s.%03d", fname, seq++);
            if (NULL == (to = fopen(newname, "w")))
            {
                  printf("\aSPLIT: error - can't write %s\n", newname);
                  return EXIT_FAILURE;
            }
            for (bytes = 0L; !feof(from) && (bytes < newsize); )
            {
                  if (fgets(buf, 1023, from))
                  {
                        fputs(buf, to);
                        bytes += (long)strlen(buf);
                  }
            }
            fclose(to);
            printf("%s written\n", newname);
      }
      return EXIT_SUCCESS;
}
