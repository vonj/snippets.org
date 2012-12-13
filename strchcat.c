/*
**  STRCHCAT.C - Append a character to a string.
**
**  Arguments: 1 - Pointer to string to append to
**             2 - Character to append
**             3 - Maximum size of string buffer
**
**  Returns: Pointer to modified string, or NULL if insufficient space
**
**  Original Copyright 1990-95 by Robert B. Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  NOTE: The name of this funtion violates ANSI/ISO 9899:1990 sec. 7.1.3,
**        but this violation seems preferable to either violating sec. 7.13.8
**        or coming up with some hideous mixed-case or underscore infested
**        naming. Also, many SNIPPETS str---() functions duplicate existing
**        functions which are supported by various vendors, so the naming
**        violation may be required for portability.
*/

#include <stdlib.h>
#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strchcat(char *string, int ch, size_t buflen)
{
      size_t len;

      if (NULL == string || ((len = strlen(string)) + 1) >= buflen)
            return NULL;

      string[len++] = ch;
      string[len]   = '\0';
      return string;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>

main()
{
      char buf1[80] = "This buffer's big enough",
           buf2[] = "This one's not";
      char *ptr;

      printf("strchcat(\"%s\", '!') ", buf1);
      ptr = strchcat(buf1, '!', sizeof(buf1));
      printf("returned %p...", ptr);
      if (NULL != ptr)
            printf("\n...which is \"%s\"", buf1);
      puts("\n");

      printf("strchcat(\"%s\", '!') ", buf2);
      ptr = strchcat(buf2, '!', sizeof(buf2));
      printf("returned %p...", ptr);
      if (NULL != ptr)
            printf("\n...which is \"%s\"", buf2);
      puts("\n");
      return EXIT_SUCCESS;
}

#endif /* TEST */
