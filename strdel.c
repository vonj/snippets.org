/*
**  STRDEL.C - Removes specified characters from a string
**
**  public domain demo by Bob Stout, modifications by Dan Stubbs
**
**  NOTE: The name of this funtion violates ANSI/ISO 9899:1990 sec. 7.1.3,
**        but this violation seems preferable to either violating sec. 7.13.8
**        or coming up with some hideous mixed-case or underscore infested
**        naming. Also, many SNIPPETS str---() functions duplicate existing
**        functions which are supported by various vendors, so the naming
**        violation may be required for portability.
*/

#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strdel(char *str, size_t posn, size_t len)
{
      size_t k;
      char *pos0, *pos1;

      if (str)
      {
            if (posn < strlen(str))
            {
                  pos0 = pos1 = str + posn;
                  k = strlen (pos1);
                  pos1 = (k >= len) ? pos1+len : pos1+k;
                  strMove(pos0, pos1);
            }
      }
      return str;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      int pos, len;

      if (4 > argc)
      {
            puts("Usage: STRDEL string pos len");
            puts("Deletes 'len' characters starting at position 'pos'");
            return EXIT_FAILURE;
      }
      pos = atoi(argv[2]);
      len = atoi(argv[3]);
      printf("strdel(\"%s\", %d, %d) => ", argv[1], pos, len);
      printf("\"%s\"\n", strdel(argv[1], pos, len));
      return EXIT_SUCCESS;
}

#endif /* TEST */
