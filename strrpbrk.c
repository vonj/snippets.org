/*
** strrpbrk() - reverse of strpbrk() - Finds the last occurrence of
**              any characters from szChars found in szString.
**
** Donated to SNIPPETS by Phi Nguyen 1995, modified by Bob Stout
*/

#include <stdio.h>
#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strrpbrk(const char *szString, const char *szChars)
{
      const char  *p;
      char        *p0, *p1;

      for (p = szChars, p0 = p1 = NULL; p && *p; ++p)
      {
            p1 = strrchr(szString, *p);
            if (p1 && p1 > p0)
                  p0 = p1;
      }
      return p0;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

main()
{
      char  string[] = "This is a testing string",
            chars[]  = "xyzet",
           *ptr;

      ptr = strrpbrk(string, chars);

      if (ptr)
            printf("One or more of \"%s\" found at \"%s\"\n", chars, ptr);
      else  printf("Can't find any of \"%s\" in \"%s\".\n", chars, string);
      return 0;
}

#endif /* TEST */
