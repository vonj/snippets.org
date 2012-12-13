/*
** BINCOMP -- binary compare
** by Raymond Gardner -- Englewood CO -- 8/92 -- public domain
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "minmax.h"

#define bufsize             8
#define empty_legend        " .."

unsigned char f1buf[bufsize+1], f2buf[bufsize+1];
long prevn;

void putempty(unsigned n)
{
      while (n--)
            printf(empty_legend);
}

void putbl(unsigned n)
{
      while (n--)
            printf(" ");
}

void showbufs(long n, unsigned m, unsigned char *b1, unsigned char *b2)
{
      unsigned i;

      if (n != prevn + bufsize)
            printf("\n");
      prevn = n;
      printf("%08lX ", n);

      if (b1 && b2)
      {
            for (i = 0; i < m; i++)
                  printf(" %02X", b1[i]);
            for (i = m; i < 8; i++)
                  printf("   ");
            putchar(' ');
            for (i = 0; i < m; i++)
            {
                  if (isprint(b1[i]))
                        putchar(b1[i]);
                  else  putchar(' ');
            }
            for (i = m; i < 8; i++)
                  putchar(' ');
            printf(" |");
            for (i = 0; i < m; i++)
            {
                  if (b1[i] != b2[i])
                        printf(" %02X", b2[i]);
                  else  printf(empty_legend);
            }
            for (i = m; i < 8; i++)
                  printf("   ");
            putchar(' ');
            for (i = 0; i < m; i++)
            {
                  if (b1[i] != b2[i] && isprint(b2[i]))
                        putchar(b2[i]);
                  else  putchar(' ');
            }
      }
      else if (b1)
      {
            for (i = 0; i < m; i++)
                  printf(" %02X", b1[i]);
            for (i = m; i < 8; i++)
                  printf("   ");
            putchar(' ');
            for (i = 0; i < m; i++)
            {
                  if (isprint(b1[i]))
                        putchar(b1[i]);
                  else  putchar(' ');
            }
            for (i = m; i < 8; i++)
                  putchar(' ');
            printf(" |");
      }
      else
      {
            putbl(33);
            printf(" |");
            for (i = 0; i < m; i++)
                  printf(" %02X", b2[i]);
            for (i = m; i < 8; i++)
                  printf("   ");
            putchar(' ');
            for (i = 0; i < m; i++)
            {
                  if (isprint(b2[i]))
                        putchar(b2[i]);
                  else  putchar(' ');
            }
      }
      printf("\n");
}

long fsize(FILE *fp)
{
      long pos, size;

      pos = ftell(fp);
      fseek(fp, 0L, SEEK_END);
      size = ftell(fp);
      fseek(fp, pos, SEEK_SET);
      return size;
}

void bincomp(FILE *f1, FILE *f2)
{
      unsigned m;
      long f1len, f2len, k, n;

      prevn = -1;
      f1len = fsize(f1);
      f2len = fsize(f2);
      printf("%ld %ld\n", f1len, f2len);
      k = min(f1len, f2len);
      n = 0;
      while (n < k)
      {
            m = (unsigned)min(k - n, (long)bufsize);
            fread(f1buf, 1, m, f1);
            fread(f2buf, 1, m, f2);
            if (memcmp(f1buf, f2buf, m) != 0)
                  showbufs(n, m, f1buf, f2buf);
            n += m;
      }
      while (n < f1len)
      {
            m = (unsigned)min(f1len - n, (long)bufsize);
            fread(f1buf, 1, m, f1);
            showbufs(n, m, f1buf, NULL);
            n += m;
      }
      while (n < f2len)
      {
            m = (unsigned)min(f2len - n, (long)bufsize);
            fread(f2buf, 1, m, f2);
            showbufs(n, m, NULL, f2buf);
            n += m;
      }
}

int main(int argc, char **argv)
{
      FILE *f1, *f2;

      if (argc < 3)
      {
            puts("Usage: bincomp f1 f2");
            exit(0);
      }
      printf("%s vs. %s\n", argv[1], argv[2]);
      f1 = fopen(argv[1], "rb");
      f2 = fopen(argv[2], "rb");
      if (f1 && f2)
            bincomp(f1, f2);
      else  puts("can't open file(s)");
      return 0;
}
