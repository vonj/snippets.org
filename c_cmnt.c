/* Extract comments from a C program source file.
**
** This program acts as a filter to copy comments in a C source
** file to the output.  Each comment includes the starting and
** ending delimiters and is followed by a newline.
**
** Three #ifdef options are defined:
**   INHIBIT_TRIGRAPHS prevents recognition of trigraphs, which
**      can affect detection of escaped characters,
**      i.e., ??/" is an escaped quote.
**   TRANSLATE_TRIGRAPHS causes the output to have trigraphs
**      converted to the normal C characters.
**   CPP_MODE causes "//" to start a comment.
** The default for these symbols is undefined, resulting in
** operation on strict ANSI source, except as noted below.
**
** What makes this program interesting is that comment detection
** should be inhibited within strings and character constants.
**
** Note: The name of a header following #include can, under ANSI,
** contain any sequence of characters, except \n and the closing
** > or ".  This program doesn't inhibit comment, string, or character
** constant detection within the header name, as an ANSI parser must.
**
** Written by and contributed to the public domain by
** Thad Smith III,   Boulder, CO,  October 1990.
*/

#include <stdio.h>

#ifndef INHIBIT_TRIGRAPHS        /* default: recognize trigraphs */
 #define getnc()  getnsc(1)       /* get char with trigraph xlate */
 #ifdef TRANSLATE_TRIGRAPHS
  #define getcmtc() getnsc(1)     /* get comment char w/ t.g. xlate */
 #else
  #define getcmtc() getnsc(0)     /* default: no comment t.g. xlate */
 #endif

/*
**  get next source character or EOF
*/

int getnsc(int cvtg)             /* boolean: convert trigraphs */
{
      static int c, nc, nnc;     /* next 3 characters */

      /* shift in next source character */

      c = nc;  nc = nnc;   nnc = getchar();

      /* perform trigraph substitution */

      if (cvtg && c == '?' && nc == '?')
      {
            switch (nnc)
            {
            case '=' :
                  c = '#' ;
                  break;
            case '(' :
                  c = '[' ;
                  break;
            case '/' :
                  c = '\\';
                  break;
            case ')' :
                  c = ']' ;
                  break;
            case '\'':
                  c = '^' ;
                  break;
            case '<' :
                  c = '{' ;
                  break;
            case '!' :
                  c = '|' ;
                  break;
            case '>' :
                  c = '}' ;
                  break;
            case '-' :
                  c = '~' ;
                  break;
            default  :
                  return c;        /* no substitution */
            }
            nc = getchar();   nnc = getchar();
      }
      return c;
}

#else       /* don't process trigraphs */

 #define getnc()   getchar()
 #define getcmtc() getchar()
#endif

int main(void)
{
      int pc;  /* previous character      */
      int c;   /* current input character */

#ifndef INHIBIT_TRIGRAPHS
      getnc();                                        /* prime the pump */
      getnc();
#endif
      c = getnc();                                    /* get first char */

      for (;;)                                  /* in non-comment area */
      {
            switch (c)
            {
            case '/':                     /* possible start of comment */
                  if ((c= getnc()) == '*')      /* process comment */
                  {
                        putchar('/');
                        putchar('*');

                        /* copy comment to stdout */

                        for (pc = 0; (c = getcmtc()) != EOF &&
                              (putchar(c) != '/' || pc != '*'); pc=c)
                                    ;
                              putchar('\n');
#ifdef CPP_MODE
                  }
                  else if (c == '/')      /*  '//' comment */
                  {
                        putchar('/');
                        putchar('/');
                        while ((c = getcmtc()) != EOF && putchar(c) != '\n')
                              ;
#endif
                  }
                  else  continue;         /* test current char */
                  break;

            case '\"':                                /* start of string */
            case '\'':    /* start of (possibly multi-byte) char constant */
                  pc = c;                             /* save delimiter */
                  do                      /* scan through character constant,
                                          ** discarding escape chars
                                          */
                  {
                        while ((c = getnc()) == '\\')
                              getnc();
                  } while (c != pc && c != EOF);
                  break;
            }
            if (c == EOF)
                  return 0;
            else  c = getnc();
      }
}
