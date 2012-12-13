/*
** arccrc16.c -- calculate 16-bit CRC for file(s)
** rev. Feb. 1992
** public domain by Raymond Gardner
**                  Englewood, Colorado
**
** This program uses the same CRC calculation as ARC (SEA) and LHA (Yoshi)
*/

#include <stdio.h>
#include "crc.h"

#define bufsiz (16*1024)

static WORD crc_table[256];

/*
** I determined this function empirically, by examining the
** table for patterns, and programming via trial-and-error.
** Don't ask me how it works or if it can be generalized for
** other CRC polynomials.
*/

void init_crc_table(void)
{
      int i, j;
      WORD k;

      for (i = 0; i < 256; i++)
      {
            k = 0xC0C1;
            for (j = 1; j < 256; j <<= 1)
            {
                  if (i & j)
                        crc_table[i] ^= k;
                  k = (k << 1) ^ 0x4003;
            }
      }
}

/*
** crc_calc() -- calculate cumulative crc-16 for buffer
*/

WORD crc_calc(WORD crc, char *buf, unsigned nbytes)
{
      unsigned char *p, *lim;

      p = (unsigned char *)buf;
      lim = p + nbytes;
      while (p < lim)
      {
            crc = (crc >> 8 ) ^ crc_table[(crc & 0xFF) ^ *p++];
      }
      return crc;
}

void do_file(char *fn)
{
      static char buf[bufsiz];
      FILE *f;
      int k;
      WORD crc;

      f = fopen(fn, "rb");
      if (f == NULL)
      {
            printf("%s: can't open file\n", fn);
            return;
      }
      crc = 0;
      while ((k = fread(buf, 1, bufsiz, f)) != 0)
            crc = crc_calc(crc, buf, k);
      fclose(f);
      printf("%-14s %04X\n", fn, crc);
}

#ifdef TEST

int main(int argc, char **argv)
{
      int i;

      if (argc < 2)
      {
            fprintf(stderr, "Usage: crc filename [filename...]\n");
            return EXIT_FAILURE;
      }
      init_crc_table();
      for (i = 1; i < argc; i++)
            do_file(argv[i]);
      return EXIT_SUCCESS;
}

#endif /* TEST */
