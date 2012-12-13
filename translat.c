/*
** Public Domain by Jerry Coffin.
**
** Interprets a string in a manner similar to the way the compiler
** does string literals in a program.  All escape sequences are
** longer than their translated equivalent, so the string is
** translated in place and either remains the same length or
** becomes shorter.
*/

#include <string.h>
#include <stdio.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *translate(char *string)
{
      char *here=string;
      size_t len=strlen(string);
      int num;
      int numlen;

      while (NULL!=(here=strchr(here,'\\')))
      {
            numlen=1;
            switch (here[1])
            {
            case '\\':
                  break;

            case 'r':
                  *here = '\r';
                  break;

            case 'n':
                  *here = '\n';
                  break;

            case 't':
                  *here = '\t';
                  break;

            case 'v':
                  *here = '\v';
                  break;

            case 'a':
                  *here = '\a';
                  break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                  numlen = sscanf(here,"%o",&num);
                  *here = (char)num;
                  break;

            case 'x':
                  numlen = sscanf(here,"%x",&num);
                  *here = (char) num;
                  break;
            }
            num = here - string + numlen;
            here++;
            memmove(here,here+numlen,len-num );
      }
      return string;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
