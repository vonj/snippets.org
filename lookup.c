/*
**  LOOKUP.C
**
**  This is the lookup function for reading a binary file index to an ASCII
**  file formatted as follows:
**
**  Mark Corgan
**  550 Foothill Rd.
**  Gardnerville, NV 89410
**  (702) 265-2388
**  .
**  Hello World
**  123 Anywhere St.
**  Anytown, CA 12345
**  (123) 456-7890
**  .
**  etc...
**  
**  The period is what  LOOKUP.C looks for to indicate the end
**  of record. Of course, you could have any format you like, so long as the
**  first line is the information you are looking for. Also, there is no
**  limit to the number of lines of infomation after the first line and
**  before the period as fputs() continues until the period. Enjoy!
**
**  by Mark Corgan, 09-May-1993, and donated to the public domain
*/

#include <stdio.h>
#include "errors.h"
#include "indxlook.h"

long bsearch_(FILE *ifp, long first, long last, char *target);

int main(int argc, char *argv[])
{
      FILE *afp, *ifp;
      char line[MAX_LINE];
      INDEX header;
      long pos;

      if (argc != 3)
      {
            puts("Usage: LOOKUP text_file_name index_file_name\n");
            puts("Note: The text file must consist of a number of records "
                 "separated by lines");
            puts("      containing a single period (\".\")");
            return EXIT_FAILURE;
      }
      afp = cant(argv[1], "r");
      ifp = cant(argv[2], "rb");
      if (fread((char *) &header, sizeof(INDEX), 1, ifp) == 0)
            fprintf(stderr, "Can't read header of \"%s\"\n", argv[2]);
      else  while(printf("Name? "), fgets(line, sizeof(line), stdin))
      {
            if ((pos = bsearch_(ifp, 1L, header.pos, line)) == -1)
                  printf("Couldn't find: %s\n", line);
            else if (fseek(afp, pos, 0) != 0)
                  fprintf(stderr, "Can't read record at position %ld\n", pos);
            else while(fgets(line,sizeof(line),afp) && strcmp(line,END_REC)!=0)
                  fputs(line, stdout);
      }
      fclose(ifp);
      fclose(afp);

      exit(0);
}

long bsearch_(FILE *ifp, long first, long last, char *target)
{
      long pos, mid =(first + last) / 2;
      INDEX next;
      int cmp;

      if (mid < first || fseek(ifp, mid * sizeof(INDEX), 0) != 0 ||
          fread((char *) &next, sizeof(INDEX), 1, ifp) == 0)
      {
            pos = -1;
      }
      else  pos = ((cmp = strncmp(target, next.key, MAX_KEY)) == 0) ?
                  next.pos : ((cmp < 0) ? bsearch_(ifp, first, mid - 1, target)
                                        : bsearch_(ifp, mid + 1, last, target));
      return pos;
}
