/* printq.c 12-22-91 Robert Mashlan, Public Domain

   A small program that utilizes the prnspool module,
   which is an interface to the DOS program PRINT.COM

*/

#include "prnspool.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv )
{
      char far *files;
      int i;
      int addfiles = 1;

      if (!printspool_installed())
      {
            printf("print.com not installed\n");
            return 0;
      }
      for (i = 1; i < argc; i++)
      {
            if (stricmp(argv[i],"/T") == 0)
                  printspool_cancel();    /* cancel all files in queue */
            else if (stricmp(argv[i],"/C") == 0)
                  addfiles = 0;           /* cancel all listed files */
            else if (stricmp(argv[i],"/P") == 0)
                  addfiles = 1;           /* add all listed files */
            else
            /* here the specified file should really have the full pathname */
            {
                  if (addfiles)
                        printspool_submit(argv[i]);
                  else  printspool_remove(argv[i]);
                  if (printspool_errno)
                        puts(printspool_errlist[printspool_errno]);
            }
      }
      printf("files currently in queue:\n");
      for (files = printspool_getqueue(); *files; files += 64)
            printf("\t%Fs\n", files);
      printspool_endhold();
      return 0;
}
