/*
**  HEXDUMP.C - Dump a file.
**
**  Originally written By Paul Edwards
**  Released to the public domain
**
**  Modified for SNIPPETS by Bob Stout
**
**  Uses ERR_EXIT.C and FERROF.C, also in SNIPPETS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "errors.h"

static void dodump(FILE *fp, long start, long count);

main(int argc, char **argv)
{
      FILE *fp;
      long start, count, length;

      if (argc < 2)
            ErrExit("Usage: HEXDUMP file_name [start] [length]");
      if (argc > 2)
            sscanf(argv[2], "%li", &start);
      else  start = 0L;
      if (argc > 3)
            sscanf(argv[3], "%li", &count);
      else  count = -1L;

      fp = cant(argv[1], "rb");

      fseek(fp, 0L, SEEK_END);
      length = ftell(fp);
      if (start > length)
      {
            ErrExit("Can't find position %ld in a %ld byte file",
                  start, length);
      }
      if (fseek(fp, start, SEEK_SET))
            ErrExit("Unable to find position %ld", start);

      dodump(fp, start, count);
      return (EXIT_SUCCESS);
}

static void dodump(FILE *fp, long start, long count)
{
      int c, pos1, pos2, posn = (int)(start % 16L);
      long x = 0L;
      char prtln[80];

      while (((c = fgetc(fp)) != EOF) && (x != count))
      {
            if (0 == (posn % 16) || 0 == x)
            {
                  memset(prtln,' ',sizeof prtln);
                  sprintf(prtln,"%0.6X:", start + x);
                  prtln[7] = ' ';
                  pos1 = 8 + (int)(3 * posn);
                  if (posn > 3)
                        ++pos1;
                  if (posn > 7)
                        ++pos1;
                  if (posn > 11)
                        ++pos1;
                  pos2 = 60 + (int)(posn);
            }
            sprintf(prtln + pos1, "%0.2X ", c);
            if (isprint(c))
                  sprintf(prtln + pos2, "%c", c);
            else  sprintf(prtln + pos2, ".");
            pos1 += 3;
            *(prtln+pos1) = ' ';
            pos2++;
            if (posn % 4 == 3)
                  *(prtln + pos1++) = ' ';
            if (posn % 16 == 15)
            {
                  printf("%s\n", prtln);
                  posn = 0;
            }
            else  ++posn;
            ++x;
      }
      if (posn % 16)
            printf("%s\n", prtln);
      return;
}
