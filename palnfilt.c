/*
**  PALNFILT.C - A palindrome filter.
**
**  Reads lines of text and only passes lines which are palindromes, i.e.
**  those which read the same forwards or backwards. Includes options switches
**  to ignore case and/or punctuation, e.g.
**
**  Input               Output      w/ -C       w/ -P       w/ -C -P
**  ---------------     ----------  ----------  ----------  ---------------
**  toot                toot        toot        toot        toot
**  Toot                            Toot                    Toot
**  toot!                                       toot!       toot!
**  Madam, I'm Adam                                         Madam, I'm Adam
**  This is a test.
**  -----------------------------------------------------------------------
**
**  public domain demo by Bob Stout
**
**  Uses GETOPTS.C and CANT.C, also from SNIPPETS
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "getopts.h"
#include "snipfile.h"                     /* For cant()           */

#define LAST_CHAR(s) (((char *)s)[strlen(s) - 1])
#define NUL '\0'

void usage(int exitval);
char *rmpunc(char *str);

Boolean_T fold = False_, punc = False_, help = False_;

struct Option_Tag options[] = {
      {'c', False_, Boolean_Tag, &fold, NULL, NULL, NULL},
      {'p', False_, Boolean_Tag, &punc, NULL, NULL, NULL},
      {'h', False_, Boolean_Tag, &help, NULL, NULL, NULL},
      { 0 , False_, Error_Tag  , NULL,  NULL, NULL, NULL}
};

/*
**  Implemented as a true filer, defaulting to stdin and stdout.
*/

main(int argc, char *argv[])
{
      FILE *infile = stdin, *outfile = stdout;
      char line[2][256];                        /* Nice & roomy   */

      if (Error_ == getopts(argc, argv))
            usage(-1);
      if (help)
            usage(0);
      if (1 < xargc)
            infile = cant(xargv[1], "r");
      if (2 < xargc)
            outfile = cant(xargv[2], "w");
      while (NULL != fgets(line[0], 255, infile))
      {
            char *p1, *p2;
            int OK;

            strcpy(line[1], line[0]);
            if ('\n' == LAST_CHAR(line[1]))
                  LAST_CHAR(line[1]) = NUL;
            if (fold)
                  strupr(line[1]);
            if (punc)
                  rmpunc(line[1]);
            for (p1 = line[1], p2 = &LAST_CHAR(line[1]), OK = 1;
                  p2 > p1; ++p1, --p2)
            {
                  if (*p1 != *p2)
                  {
                        OK = 0;
                        break;
                  }
            }
            if (OK)
                  fputs(line[0], outfile);
      }
      return 0;
}

/*
**  Tell 'em how it works
*/

void usage(int exitval)
{
      puts("Usage: PALNFILT [-cph] [infile] [outfile]");
      puts("where: h = Provide help (this display)");
      puts("       c = Ignore case");
      puts("       p = Ignore punctuation");
      puts("       infile defaults to stdin");
      puts("       outfile defaults to stdout");
      puts("notes: switches may not be concatenated but may be any case");
      exit(exitval);
}

/*
**  Remove all punctuation from a string
*/

char *rmpunc(char *str)
{
      char *obuf, *nbuf;

      for (obuf = str, nbuf = str; *obuf && obuf; ++obuf)
      {
            if (isalpha(*obuf))
                  *nbuf++ = *obuf;
      }
      *nbuf = NUL;
      return str;
}
