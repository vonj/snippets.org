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
**  Make an ascii hexadecimal string into an integer.
*/

#include <stdio.h>
#include <ctype.h>
#include "numcnvrt.h"

unsigned int hstr_i(char *cptr)
{
      unsigned int i, j = 0;

      while (cptr && *cptr && isxdigit(*cptr))
      {
            i = *cptr++ - '0';
            if (9 < i)
                  i -= 7;
            j <<= 4;
            j |= (i & 0x0f);
      }
      return(j);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
      char *arg;
      unsigned int x;

      while (--argc)
      {
            x = hstr_i(arg = *++argv);
            printf("Hex %s = %d\n", arg, x, x);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
