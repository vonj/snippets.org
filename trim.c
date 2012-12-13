/*
**  TRIM.C - Remove leading, trailing, & excess embedded spaces
**
**  public domain by Bob Stout & Michael Dehlwes
*/

#include <ctype.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *trim (char *str)
{
      char *ibuf, *obuf;

      if (str)
      {
            for (ibuf = obuf = str; *ibuf; )
            {
                  while (*ibuf && (isspace (*ibuf)))
                        ibuf++;
                  if (*ibuf && (obuf != str))
                        *(obuf++) = ' ';
                  while (*ibuf && (!isspace (*ibuf)))
                        *(obuf++) = *(ibuf++);
            }
            *obuf = NUL;
      }
      return (str);
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
      if (argc == 2)
      {
            printf ("trim(\"%s\")\n", argv[1]);
            printf ("returned \"%s\"\n", trim (argv[1]));
      }
      else
      {
            fprintf (stderr, "To test this function, call TRIM\n");
            fprintf (stderr, "with an argument enclosed in quotes.\n");
            fprintf (stderr, "   Example:\n");
            fprintf (stderr, "   C:\\>trim \"  test   test   \"\n");
            fprintf (stderr, "   trim(\"  test   test   \"\n");
            fprintf (stderr, "   returned \"test test\"\n\n");
            fprintf (stderr, "   C:\\>_\n");
            return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
