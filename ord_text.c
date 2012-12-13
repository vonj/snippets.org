/*
**  Originally published as part of the MicroFirm Function Library
**
**  Copyright 1991, Robert B.Stout
**
**  With modifications suggested by Maynard Hogg
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Function to return ordinal text.
*/

#include "numcnvrt.h"

static char *text[] = {"th", "st", "nd", "rd"};

char *ordinal_text(int number)
{
      if (((number %= 100) > 9 && number < 20) || (number %= 10) > 3)
            number = 0;
      return text[number];
}

#ifdef TEST

#include <stdio.h>

main()
{
      int i;

      for (i = 0; i < 26; ++i)
            printf("%d%s\n", i, ordinal_text(i));
      return 0;
}

#endif /* TEST */
