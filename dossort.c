/*

    cl /AL sortit.c

===========================================================
   sort.c       7-31-91  Robert Mashlan

   This filter is almost compatible with the MS-DOS filter of the same name.

   This filter sorts each line of standard input, disregarding case,
   and sends it to standard output.

   optional parameters:  /R    Output in reverse order
                         /+n   Compare at column n, 1 based

   example usage:  sort < unsorted.txt > sorted.txt

   note: compile in a far data model for maximum capacity ( compact or large )

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINES 10000        /* maximum number of lines to sort */
#define MAXLINE  80           /* maximum line length             */

unsigned col     = 0;  /* column to start sort at ( zero based here ) */
int      reverse = 0;  /* reverse order flag                          */

/*
**  compare function for qsort
*/

int cmp( const void *a, const void *b)
{
      int result;
      const char *_a = *(const char **)a;
      const char *_b = *(const char **)b;

      /* compare at col if other than zero */

      if (col > 0)
      {
            if (strlen(_a) > col)
                  _a += col;
            else  _a = "";
            if (strlen(_b) > col)
                  _b += col;
            else  _b = "";
      }
      result = stricmp(_a,_b);
      return reverse ? -result : result;
}

int main(int argc, char *argv[])
{
      static char *lines[MAXLINES];
      int i, nlines=0, no_match;
      char buf[MAXLINE];

      /* scan for command line options */

      for(i=1;i<argc;i++)
      {
            if(!stricmp(argv[i],"/R"))  /* reverse order option */
                  reverse=1;
            else if (!strnicmp(argv[i],"/+",2))  /* column number option */
            {
                  int n = atoi(argv[i]+1)-1;

                  if (n < 0)
                        n = 0;
                  col = (unsigned)n;
            }
            else
            {
                  fputs("Invalid Parameter",stderr);
                  return 1;
            }
      }
      while(1)
      {
            /* read the line */

            if(fgets(buf,MAXLINE,stdin)==NULL)
                  break;

            /* Check for duplicates here */

            no_match = 1;
            for ( i=0; i< nlines; i++ )
            {
                  if (0 == (no_match = memicmp(buf, lines[i],
                        (unsigned int)12)))
                  {
                        break;
                  }
            }

            if (no_match)                 /* make a duplicate on the heap */
            {
                  if((lines[nlines++]=strdup(buf))==NULL)
                  {
                        fputs("SORT: Insufficient memory",stderr);
                        return 2;
                  }
            }
      }
      /* sort the lot */
      
      qsort((void *)lines,nlines,sizeof(char *),cmp);

      /* display the sorted output */
      
      for (i=0;i<nlines;i++)
            fputs(lines[i],stdout); /* send it to standard output */
      return 0;
}
