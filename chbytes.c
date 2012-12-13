/*
**  CHBYTES.C - Change bytes in a file
**
**  This program searches a file for a sequence of bytes. If they are
**  found, they are replaced with zeros. It was originally developed for
**  a friend who needed a program to call from Paradox to remove printer
**  control sequences from formatted print files. The requirements were
**  1) since it is called by another program, all status has to be returned
**  in the errorlevel with no screen messages allowed, and 2) The file must
**  remain the same length, so the deleted sequences must only be replaced
**  with ASCII NULs.
**
**  Syntax: CHBYTES filename pattern_1 [pattern_2 [...pattern_N]]
**  where:  Each pattern is a comma-separated list of bytes, each of which
**          may be of the following forms:
**          C    - Any single character will be treated as literal.
**          XXh  - "XX" will be interpreted as a hexadecimal number (both
**                 1- and 2-digit hex numbers are allowed).
**          NNNd - "NNN" will be interpreted as a decimal number (both
**                 1-, 2-, and 3-digit decimal numbers are allowed).
**
**  e.g.    CHBYTES printer.fil 12d 1bh,[,3,x
**          would zero out form feeds and the escape sequence "[3x"
**
**  Returns: 0 - Success
**           1 - No filename
**           2 - No arguments
**           3 - Error opening file
**           4 - Not enough memory
**           5 - Bad argument
**           6 - Error reading file
**           7 - Error writing file
**
**  Public domain by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sniptype.h"

#ifdef __ZTC__
 int _okbigbuf = 0;
#endif

#ifndef max
 #define max(x,y) (((x) >= (y)) ? (x) : (y))
#endif

int bufsize;

struct {
      char pattern[40];                         /* pattern to find      */
      int numbytes;                             /* length of pattern    */
} search[40];

int main (int argc, char *argv[])
{
      FILE *fp  = NULL;
      char *buf = NULL, *getbuf(void);
      fpos_t rpos;
      int i, patterns, max_bytes = 0;
      Boolean_T hex2char(const char *, char *);

      if (2 > argc)                             /* no filename          */
            return 1;
      if (3 > argc)                             /* no argument          */
            return 2;
      if (NULL == (fp = fopen(argv[1], "r+b")))
            return 3;                           /* file open error      */
      if (NULL == (buf = getbuf()))
            return 4;                           /* no memory for buffer */

      patterns = argc - 2;                      /* process arguments    */
      for (i = 2; i < argc; ++i)
      {
            char *p, *ptr;

            if (NULL != (ptr = strtok(argv[i], ",")))
            {
                  p = search[i - 2].pattern;
                  do
                  {
                        search[i - 2].numbytes++;
                        if (1 == strlen(ptr))
                        {
                              *p++ = *ptr;
                              continue;
                        }
                        switch (toupper(LAST_CHAR(ptr)))
                        {
                        case 'D':
                              LAST_CHAR(ptr) = '\0';
                              *p++ = (char)atoi(ptr);
                              break;
                        case 'H':
                              LAST_CHAR(ptr) = '\0';
                              if (Error_ == hex2char(ptr, p++))
                                    return 5;
                              break;
                        default:
                              return 5;
                        }
                  } while (NULL != (ptr = strtok(NULL, ",")));
                  *p = '\0';
                  max_bytes = max(max_bytes, search[i - 2].numbytes);
            }
            else  return 5;
      }

      fgetpos(fp, &rpos);                       /* save where we are    */
      while (1)
      {
            int bytes, n;
            Boolean_T modified;

            if (max_bytes > (bytes = (int)fread(buf, 1, bufsize, fp)))
            {
                  if (0 == bytes && !feof(fp))
                        return 6;               /* something's wrong!   */
                  else  break;                  /* all done!            */
            }
            for (n = 0, modified = False_; n < patterns; ++n)
            {
                  /* check each pattern in turn                         */

                  for (i = 0; i < (bytes - max_bytes + 1); ++i)
                  {
                        int j;

                        if (buf[i] != *(search[n].pattern))
                              continue;
                        if (Success_ != strncmp(&buf[i],
                              search[n].pattern, search[n].numbytes))
                        {
                              continue;
                        }

                        /* found one! replace it in the buffer          */

                        for (j = 0; j < search[n].numbytes; ++j, ++i)
                              buf[i] = '\0';
                        modified = True_;
                  }
            }
            if (modified)                       /* write changes, if any*/
            {
                  fpos_t wpos = rpos;

                  fsetpos(fp, &wpos);
                  if (bytes != (int)fwrite(buf, 1, bytes, fp))
                        return 7;
                  fsetpos(fp, &rpos);
            }
            rpos += bytes - max_bytes + 1;      /* get another buffer   */
            fsetpos(fp, &rpos);
      }
      fclose(fp);
      return Success_;
}

/*
**  Allocate the largest buffer we can
*/

char *getbuf(void)
{
      register char *buffer;

      for (bufsize = 0x4000; bufsize >= 128; bufsize >>= 1)
      {
            if (NULL != (buffer = (char *) malloc(bufsize)))
                  return buffer;
      }
      return NULL;
}

/*
**  Convert ASCII hex char to char
*/

#define xdigit(c) (toupper(c) - (((c) > '9') ? 'A' - 10 : '0'))

Boolean_T hex2char(const char *hex, char *buf)
{
      int ch = 0;
      char *p = (char *)hex;

      while(*p)
      {
            if (!isxdigit(*p))
                  return Error_;
            ch <<= 4;
            ch  += xdigit(*p);
                ++p;
      }
      *buf = (char)ch;
      return Success_;
}
