/*
**  STRDELCH.C - Removes specified character(s) from a string
**
**  public domain demo by Bob Stout
**
**  NOTE: The name of this funtion violates ANSI/ISO 9899:1990 sec. 7.1.3,
**        but this violation seems preferable to either violating sec. 7.13.8
**        or coming up with some hideous mixed-case or underscore infested
**        naming. Also, many SNIPPETS str---() functions duplicate existing
**        functions which are supported by various vendors, so the naming
**        violation may be required for portability.
*/

#include <stdio.h>
#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strdelch(char *string, const char *lose)
{
      if (!string || !*string)
            return NULL;
      if (lose)
      {
            char *s;

            for (s = string; *s; ++s)
            {
                  if (strchr(lose, *s))
                  {
                        strMove(s, s + 1);
                        --s;
                  }
            }
      }
      return string;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

main(int argc, char *argv[])
{
      char *strng, *delstrng;

      if (3 > argc--)
      {
            puts("Usage: STRDELCH char(s)_to_delete string_1 [...string_N]");
            return -1;
      }
      else  delstrng = *(++argv);
      while (--argc)
      {
            strng = *(++argv);
            printf("strdelch(\"%s\", \"%s\") => ", delstrng, strng);
            printf("\"%s\"\n", strdelch(strng, delstrng));
      }
      return 0;
}

#endif /* TEST */
