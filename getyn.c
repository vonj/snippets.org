/************************************************************************/
/*                                                                      */
/*  GETYN.C                                                             */
/*                                                                      */
/*  Original Copyright 1993-94 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "sniptype.h"
#include "snipkbio.h"

/************************************************************************/
/*                                                                      */
/*  getYN()                                                             */
/*                                                                      */
/*  Prompt a user for a Yes/No response with default and timeout        */
/*  features.                                                           */
/*                                                                      */
/*  Parameters: 1 - Prompt string                                       */
/*              2 - Default response, 'Y' or 'N', '\0' for none         */
/*              3 - Timeout before default assumed - 0 for none         */
/*                                                                      */
/*  Returns: True_ or False_                                            */
/*                                                                      */
/*  NOTE: All output is via stderr                                      */
/*                                                                      */
/************************************************************************/

Boolean_T getYN(char *prompt, int def_ch, unsigned timeout)
{
      int ch;
      char *yn = " (y/n) ";
      clock_t start, limit = (clock_t)0;

      fputs(prompt, stderr);
      
      def_ch = toupper(def_ch);
      if ('Y' == def_ch)
            yn[2] = def_ch;
      else if ('N' == def_ch)
            yn[4] = def_ch;
      else  def_ch = '\0';

      fputs(yn, stderr);

      if (0 != def_ch && 0 < timeout)
      {
            start = clock();
            limit = (clock_t)(CLK_TCK * timeout);
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
            if (def_ch && 'Y' != ch && 'N' != ch)
                  ch = def_ch;
      }

BYE:  fputc(ch, stderr);
      fputc('\n', stderr);
      return ('Y' == ch);
}
