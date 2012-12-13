/*
** Public domain from Bob Jarvis
*/

#include <stdio.h>
#include <ctype.h>
#include "phonetic.h"

char *soundex(char *instr, char *outstr)
{                   /* ABCDEFGHIJKLMNOPQRSTUVWXYZ */
        char *table = "01230120022455012623010202";
        char *outptr = outstr;
        int count = 0;

        while(!isalpha(instr[0]) && instr[0])
                ++instr;

        if(!instr[0])     /* Hey!  Where'd the string go? */
                return(NULL);

        if(toupper(instr[0]) == 'P' && toupper(instr[1]) == 'H')
        {
                instr[0] = 'F';
                instr[1] = 'A';
        }

        *outptr++ = (char)toupper(*instr++);

        while(*instr && count < 5)
        {
                if(isalpha(*instr) && *instr != *(instr-1))
                {
                        *outptr = table[toupper(instr[0]) - 'A'];
                        if(*outptr != '0')
                        {
                                ++outptr;
                                ++count;
                        }
                }
                ++instr;
        }

        *outptr = '\0';
        return(outstr);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      char code[6];

      if (argc != 2)
      {
            puts("Usage: SOUNDEX string");
            return EXIT_FAILURE;
      }

      printf("soundex(\"%s\") returned %s\n",
            argv[1], soundex(argv[1], code));

      return EXIT_SUCCESS;
}

#endif /* TEST */
