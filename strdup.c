/*
**  Portable, public domain strdup() by Bob Stout
*/

#include <stdlib.h>
#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strdup(const char *string)
{
      char *newstring;

      if (string)
      {
            if (NULL != (newstring = malloc(strlen(string) + 1)))
                  strcpy(newstring, string);
            return newstring;
      }
      else  return NULL;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
