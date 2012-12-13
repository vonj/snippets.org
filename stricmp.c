/*
**  STRICMP.C - Comapres strings, case-insensitive.
**
**  public domain by Bob Stout
**
**  NOTE: The name of this funtion violates ANSI/ISO 9899:1990 sec. 7.1.3,
**        but this violation seems preferable to either violating sec. 7.13.8
**        or coming up with some hideous mixed-case or underscore infested
**        naming. Also, many SNIPPETS str---() functions duplicate existing
**        functions which are supported by various vendors, so the naming
**        violation may be required for portability.
*/

#include <string.h>
#include <ctype.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

int stricmp(const char *str1, const char *str2)
{
      int retval = 0;

      while (1)
      {
            retval = tolower(*str1++) - tolower(*str2++);

            if (retval)
                  break;

            if (*str1 && *str2)
                  continue;
            else  break;
      }
      return retval;
}

int strnicmp(const char *str1, const char *str2, size_t n)
{
      int i, retval = 0;

      if (n)
      {
            for (i = 0; i < n; ++i)
            {
                  retval = tolower(*str1++) - tolower(*str2++);

                  if (retval)
                        break;

                  if (*str1 && *str2)
                        continue;
                  else  break;
            }
      }
      return retval;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      int len;

      if (4 > argc)
      {
            puts("Usage: STRICMP string1 string2 N");
            puts("Performs case-insensitive compare of strings.");
            puts("Both ful string compares and first N characters.");
            return EXIT_FAILURE;
      }
      else  len = atoi(argv[3]);

      printf("stricmp(\"%s\", \"%s\") returned %d\n", argv[1], argv[2],
             stricmp(argv[1], argv[2]));
      printf("stricmp(\"%s\", \"%s\", %d) returned %d\n", argv[1], argv[2],
             len, strnicmp(argv[1], argv[2], len));
      return EXIT_SUCCESS;
}

#endif /* TEST */
