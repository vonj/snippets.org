/*
   File wc.c - a sample word count program
   Written and submitted to public domain by Jay Elkes
   April, 1992
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main (int argc, char *argv[])
{
      FILE *infileptr;
      char infile[80];

      long int nl = 0;
      long int nc = 0;
      long int nw = 0;

      int state = 0;
      const int  NEWLINE = '\n';
      int  c;

/*  The program name itself is the first command line argument so we
    ignore it (argv[0]) when showing user entered parameters. */

      switch (argc - 1)
      {
      case (0):
            printf("no parameters\n");
            return 12;
      case (1):
            break;
      default:
            printf("too many parameters\n");
            return 12;
      }

      strcpy(infile,argv[1]);

      infileptr = fopen(infile,"rb");
      if (infileptr == NULL)
      {
            printf("Cannot open %s\n",infile);
            return 12;
      }

      while ((c = getc(infileptr)) != EOF)
      {
            ++nc;
            if (c == NEWLINE)
                  ++nl;
            if (isspace(c))
                  state = 0;
            else if (state == 0)
            {
                  state = 1;
                  ++nw;
            }
      }

      /* Final Housekeeping */

      printf("%ld Lines, %ld Words, %ld Characters", nl, nw, nc);
      return 0;
}
