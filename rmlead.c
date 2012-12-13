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
**  remove leading whitespace from a string
*/

#include <ctype.h>
#include <string.h>
#include "snip_str.h"                     /* Contains strMove() macro   */

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *rmlead(char *str)
{
      char *obuf;

      if (str)
      {
            for (obuf = str; *obuf && isspace(*obuf); ++obuf)
                  ;
            if (str != obuf)
                  strMove(str, obuf);
      }
      return str;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
