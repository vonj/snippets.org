/*
**  Originally published as part of the MicroFirm Function Library
**
**  Copyright 1986, S.E. Margison
**  Copyright 1989, Robert B.Stout
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  remove trailing whitespace from a string
*/

#include <string.h>
#include <ctype.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *rmtrail(char *str)
{
      int i;

      if (str && 0 != (i = strlen(str)))
      {
            while (--i >= 0)
            {
                  if (!isspace(str[i]))
                        break;
            }
            str[++i] = NUL;
      }
      return str;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
