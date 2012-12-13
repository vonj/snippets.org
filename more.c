/*
**  MORE.C - A DOS/Unix more work-alike
**
**  public domain by Bob Stout
*/

#include <stdio.h>
#include "more.h"

main()
{
      char buf[512];

      while(NULL != fgets(buf, 512, stdin))
      {
            if (Key_ESC == more_proc(buf))
                  break;
      }
      return 0;
}
