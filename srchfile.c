/*
**  SRCHFILE.C - Functions for searching files
**
**  public domain by Bob Stout
**
**  Note: Although this snippet demonstrates some useful techniques, even
**        the fast text searching algorithm used can't provide particularly
**        good performance. Left as an exercise for the user is to perform
**        explicit buffering using fread() rather than fgets() as is used
**        here. See CHBYTES.C in SNIPPETS for how to perform searches in
**        user-managed buffers.
*/

#include <stdlib.h>
#include <string.h>
#include "sniptype.h"
#include "snipfile.h"

/*
**  Allocate a big buffer, use it to buffer a specified stream
*/

static size_t fsetup(FILE *fp, size_t minbuf)
{
      register size_t bufsize;
      register char *buffer;

      /* Allocate the largest buffer we can */

      for (bufsize = 0x4000; bufsize >= minbuf; bufsize >>= 1)
      {
            if (NULL != (buffer = (char *) malloc(bufsize)))
                  break;
      }
      if (NULL == buffer)
            return 0;

      /* Use the buffer to buffer the file */

      if (Success_ == setvbuf(fp, buffer, _IOFBF, bufsize))
            return bufsize;
      else  return 0;
}

/*
**  Search a file for a pattern match (forward)
**
**  Arguments: FILE pointer
**             pattern to search for
**             size of pattern
**             find Nth occurrence
**
**  Returns: -1L if pattern not found
**           -2L in case of error
*/

long ffsearch(FILE *fp, const char *pattern, const size_t size, int N)
{
      long pos = -2L, tempos = 0L;
      char *sbuf, *p;
      size_t skip;
      int ch = 0;

      /* Allocate a search buffer */

      if (NULL == (sbuf = (char *)malloc(size - 1)))
            goto FDONE;

      /* Buffer the file and position us within it */

      if (0 == fsetup(fp, size))
            goto FDONE;
      pos = -1L;
      fseek(fp, 0L, SEEK_SET);

      /* Set up for smart searching */

      if (1 < strlen(pattern) && NULL != (p = strchr(pattern + 1, *pattern)))
            skip = p - (char *)pattern;
      else  skip = strlen(pattern);

      /* Look for the pattern */

      while (EOF != ch)
      {
            if (EOF == (ch = fgetc(fp)))
                  break;
            if ((int)*pattern == ch)
            {
                  tempos = ftell(fp);
                  if (size - 1 > fread(sbuf, sizeof(char), size - 1, fp))
                        goto FDONE;
                  if (Success_ == memcmp(sbuf, &pattern[1], size - 1))
                  {
                        if (0 == --N)
                        {
                              pos = tempos - 1L;
                              goto FDONE;
                        }
                  }
                  fseek(fp, tempos + skip, SEEK_SET);
            }
      }

      /* Clean up and leave */

FDONE:
      free(sbuf);
      return pos;
}

/*
**  Search a file for a pattern match (backwards)
**
**  Arguments: FILE pointer
**             pattern to search for
**             size of pattern
**             find Nth occurrence
**
**  Returns: -1L if pattern not found
**           -2L in case of error
*/

long rfsearch(FILE *fp, const char *pattern, const size_t size, int N)
{
      long pos = -2L, tempos;
      char *sbuf, *p;
      size_t skip;
      int ch = 0;

      /* Allocate a search buffer */

      if (NULL == (sbuf = (char *)malloc(size - 1)))
            goto RDONE;

      /* Buffer the file and position us within it */

      if (0 == fsetup(fp, size))
            goto RDONE;
      pos = -1L;
      fseek(fp, -1L, SEEK_END);
      tempos = ftell(fp) - strlen(pattern);

      /* Set up for smart searching */

      if (1 < strlen(pattern) && NULL != (p = strrchr(pattern + 1, *pattern)))
            skip = strlen(pattern) - (p - (char *)pattern);
      else  skip = strlen(pattern);

      /* Look for the pattern */

      while (0L <= tempos)
      {
            fseek(fp, tempos, SEEK_SET);
            if (EOF == (ch = fgetc(fp)))
                  break;
            if ((int)*pattern == ch)
            {
                  if (size - 1 <= fread(sbuf, sizeof(char), size - 1, fp))
                  {
                        if (Success_ == memcmp(sbuf, &pattern[1], size - 1))
                        {
                              if (0 == --N)
                              {
                                    pos = tempos;
                                    goto RDONE;
                              }
                        }
                  }
                  tempos -= skip;
            }
            else  --tempos;
      }

      /* Clean up and leave */

RDONE:
      free(sbuf);
      return pos;
}

#ifdef TEST

int main(int argc, char *argv[])
{
      long pos;
      int N = 1;
      size_t size = strlen(argv[1]);
      char buf[256], *fname = "SRCHFILE.C";
      FILE *fp;

      if (2 > argc)
      {
            puts("Usage: SRCHFILE string [N] [file]");
            puts("where: N = find Nth occurrence");
            puts("       If file is specified, N must be given");
            return EXIT_FAILURE;
      }

      if (2 < argc)
            N = atoi(argv[2]);

      if (3 < argc)
            fname = strupr(argv[3]);

      fp = cant(fname, "r");
      printf("ffsearch(%s, %s) returned %ld\n", fname, argv[1],
            pos = ffsearch(fp, argv[1], size, N));
      fseek(fp, pos, SEEK_SET);
      fgets(buf, 256, fp);
      printf("...which contains \"%s\"\n\n", buf);
      fclose(fp);

      fp = cant(fname, "rb");
      printf("rfsearch(%s, %s) returned %ld\n", fname, argv[1],
            pos = rfsearch(fp, argv[1], size, N));
      fseek(fp, pos, SEEK_SET);
      fgets(buf, 256, fp);
      printf("...which contains \"%s\"\n\n", buf);
      fclose(fp);
      return EXIT_SUCCESS;
}

#endif /* TEST */
