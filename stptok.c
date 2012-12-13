/*
**  stptok() -- public domain by Ray Gardner, modified by Bob Stout
**
**   You pass this function a string to parse, a buffer to receive the
**   "token" that gets scanned, the length of the buffer, and a string of
**   "break" characters that stop the scan.  It will copy the string into
**   the buffer up to any of the break characters, or until the buffer is
**   full, and will always leave the buffer null-terminated.  It will
**   return a pointer to the first non-breaking character after the one
**   that stopped the scan.
*/

#include <string.h>
#include <stdlib.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *stptok(const char *s, char *tok, size_t toklen, char *brk)
{
      char *lim, *b;

      if (!*s)
            return NULL;

      lim = tok + toklen - 1;
      while ( *s && tok < lim )
      {
            for ( b = brk; *b; b++ )
            {
                  if ( *s == *b )
                  {
                        *tok = 0;
                        for (++s, b = brk; *s && *b; ++b)
                        {
                              if (*s == *b)
                              {
                                    ++s;
                                    b = brk;
                              }
                        }
                        return (char *)s;
                  }
            }
            *tok++ = *s++;
      }
      *tok = 0;
      return (char *)s;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      char *ptr, buf[256];

      if (3 > argc)
      {
            puts("Usage: STPTOK \"string\" \"token_string\"");
            return EXIT_FAILURE;
      }
      else  ptr = argv[1];
      do
      {
            ptr = stptok(ptr, buf, sizeof(buf), argv[2]);
            printf("stptok(\"%s\", \"%s\")\n  buf: \"%s\"\n  "
                  "returned: \"%s\"\n", argv[1], argv[2], buf, ptr);
      } while (ptr && *ptr);
      return EXIT_SUCCESS;
}

#endif /* TEST */
