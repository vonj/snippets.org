/*
**  Portable, public domain replacements for strupr() & strlwr() by Bob Stout
*/

#include <ctype.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strupr(char *string)
{
      char *s;

      if (string)
      {
            for (s = string; *s; ++s)
                  *s = toupper(*s);
      }
      return string;
} 

char *strlwr(char *string)
{
      char *s;

      if (string)
      {
            for (s = string; *s; ++s)
                  *s = tolower(*s);
      }
      return string;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      if (argc < 2)
      {
            puts("Usage: STRUPR string1 [...string2 [...stringN]]");
            return EXIT_FAILURE;
      }
      while (--argc)
      {
            printf("Original = \"%s\"\n", *++argv);
            printf("strupr() = \"%s\"\n", strupr(*argv));
            printf("strlwr() = \"%s\"\n", strlwr(*argv));
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
