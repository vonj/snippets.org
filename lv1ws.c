/*
**  Originally published as part of the MicroFirm Function Library
**
**  Copyright 1987-88, Robert B.Stout
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Makes all whitespace single spaces. Passed a string, lv1ws()
**  converts all multiple whitespace characters to single spaces.
*/

#include <ctype.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

void lv1ws(char *str)
{
      char *ibuf, *obuf;
      int i, cnt;

      if (str)
      {
            ibuf = obuf = str;
            i = cnt = 0;

            while(*ibuf)
            {
                  if(isspace(*ibuf) && cnt)
                        ibuf++;
                  else
                  {
                        if (!isspace(*ibuf))
                              cnt = 0;
                        else
                        {
                              *ibuf = ' ';
                              cnt = 1;
                        }
                        obuf[i++] = *ibuf++;
                  }
            }
            obuf[i] = '\0';
      }
}

#if defined(__cplusplus) && __cplusplus
 }
#endif
