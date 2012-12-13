/*
**  GETVOL.C - Retrieve a disk volume label
**             (proof you don't need FCBs to do it!)
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>

#include "dirport.h"
#include "sniptype.h"

char *getvol(char drive)
{
      char search[] = "A:\\*.*";
      static DOSFileData ff;

      *search = drive;
      if (Success_ == FIND_FIRST(search, _A_VOLID, &ff))
      {
            if (8 < strlen(ff_name(&ff)))       /* Eliminate period     */
                  strcpy(&(ff_name(&ff))[8], &(ff_name(&ff))[9]);
            return ff_name(&ff);
      }
      else  return NULL;
}

#ifdef TEST

int main(int argc, char *argv[])
{
      char *label;

      if (2 > argc)
      {
            puts("\aUsage: GETVOL d[:]");
            puts("where: d = drive letter (e.g. A, B, C, etc.");
            return -1;
      }
      if (NULL == (label = getvol(*argv[1])))
            printf("Unable to read a label on drive %c:\n", *argv[1]);
      else  printf("The volume label of drive %c: is \"%s\"\n",
            *argv[1], label);
      return 0;
}

#endif /* TEST */
