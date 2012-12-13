/*
**  To4Dos.C - Utility to convert SNIPPETS.NDX to 4DOS DESCRIPT.ION file.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snipfile.h"

main()
{
      FILE *ndx, *desc;
      char line1[81], line2[81], *ptr;
      int i;

      ndx  = cant("snippets.ndx", "r");
      desc = cant("descript.ion", "w");

      while (!feof(ndx))
      {
            if (NULL != (fgets(line1, 81, ndx)))
            {
                  if ('|' == *line1 || 3 > strlen(line1) ||
                        isspace(line1[2]) || '-' == *line1)
                  {
                        continue;
                  }
                  for (ptr = line1 + 2; ' ' != *ptr; ++ptr)
                        fputc(*ptr, desc);
                  fputs(" <", desc);
                  if (isspace(line1[19]))
                  {
                        for (i = 15; i < 18; ++i)
                              fputc(line1[i], desc);
                        fprintf(desc, ">%s", line1 + 19);
                  }
                  else
                  {
                        for (i = 15; i < 18; ++i)
                              fputc(line2[i], desc);
                        fprintf(desc, ">%s", line2 + 19);
                  }
                  strcpy(line2, line1);
            }
      }
      return EXIT_SUCCESS;
}
