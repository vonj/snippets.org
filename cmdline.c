/*
**  CMDLINE.C - Demonstrates accessing command line arguments
*/

#include <stdio.h>
#include <stdlib.h>
#include "snip_str.h"         /* For plural_text() and plural_text2()   */

main(int argc, char *argv[])
{
      int i, n = argc - 1;

      printf("You passed %d argument%s on the command line.",
            n, plural_text(n));

      if (argc > 1)
      {
            puts(" They are:");
            for (i = 1; i < argc; ++i)
            {
                  printf("\nArgument #%d:\n  Text: \"%s\"\n  Value: %d\n",
                        i, argv[i], atoi(argv[i]));
            }
      }
      else  putchar('\n');
      return 0;
}
