/*
**  QUERY.C - Timed query with default for batch files
**
**  Usage: QUERY [Prompt_string] [X] [n]
**         where: X = Default answer, `Y' or `N'
**                n = Seconds to wait before using default answer
**
**  Note:  If any option is used, those preceding it must be specified.
**         For example, to use the time out feature, both the prompt
**         string and default answer must have previously been specified.
**
**  public domain by Bob Stout
*/

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

main(int argc, char *argv[])
{
      int ch = '\0', def_ch = '\0';
      char *prompt = "(y/n) ";
      clock_t start, limit = (clock_t)0;

      if (1 < argc)
      {
            fputs(argv[1], stderr);
            fputc(' ', stderr);
      }
      if (2 < argc)
      {
            def_ch = toupper(*argv[2]);
            if ('Y' == def_ch)
                  prompt[1] = def_ch;
            else if ('N' == def_ch)
                  prompt[3] = def_ch;
            else  def_ch = '\0';
      }
      fputs(prompt, stderr);
      if (3 < argc)
      {
            start = clock();
            limit = (clock_t)(CLK_TCK * atoi(argv[3]));
      }
      while ('Y' != ch && 'N' != ch)
      {
            while (!kbhit())
            {
                  if (limit && (limit <= (clock() - start)))
                  {
                        ch = def_ch;
                        goto BYE;
                  }
            }
            ch = toupper(getch());
            if ('Y' != ch && 'N' != ch && (1 < argc))
                  ch = def_ch;
      };
BYE:  fputc(ch, stderr);
      fputc('\n', stderr);
      return ('Y' == ch);
}
