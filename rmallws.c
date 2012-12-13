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
**  remove all whitespace from a string
*/

#include <stdio.h>
#include <ctype.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *rmallws(char *str)
{
      char *obuf, *nbuf;

      if (str)
      {
            for (obuf = str, nbuf = str; *obuf; ++obuf)
            {
                  if (!isspace(*obuf))
                        *nbuf++ = *obuf;
            }
            *nbuf = NUL;
      }
      return str;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
