/* SNIPTREE

   Written by Tom Torfs (2:292/516@fidonet.org)
   I hereby donate this code to the public domain

   Builds custom directory structure for SNIPPETS by reading SNIPPETS.NDX

   Requires POSIX-function mkdir(), rest should be ANSI C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif

int main(void)
{
      FILE *fp;
      char buf[100];
      char curpath[80];
      char oldfile[80],newfile[80];
      int i;
      int skiparea;

      puts("SNIPTREE by Tom Torfs");

      if ((fp=fopen("SNIPPETS.NDX","r"))==NULL)
      {
            puts("ERROR: can't open SNIPPETS.NDX for reading");
            return 1;
      }

      while (1)
      {
            do
            {
                  fgets(buf,100,fp);
                  if (feof(fp)) goto finished;
            }
            while (memcmp(buf,"|=========",10))
                  ;
            fgets(buf,100,fp);
            if (feof(fp)) goto finished;
            if (!memcmp(buf,"|=========",10))
            {
                  fgets(buf,100,fp); /* empty */
                  fgets(buf,100,fp); /* section name */
                  fgets(buf,100,fp); /* empty */
                  fgets(buf,100,fp); /* |======================= */
                  fgets(buf,100,fp); /* |======================= */
                  continue;
            }
            strtok(buf,"\n");
            printf("\nFile area: %s\n",buf+2);
            if (!memicmp(buf+2,"Files deleted",13))
            {
                  skiparea = 1;
                  puts("--> SKIPPING");
            }
            else
            {
                  skiparea = 0;
                  printf("Subdirectory (empty=don't move): ");
                  fgets(curpath,80,stdin);
                  i = strlen(curpath);
                  while (i>0 && (curpath[i-1]=='\n' || curpath[i-1]=='\\'
                                 || curpath[i-1]=='/'))
                        i--;
                  curpath[i] = '\0';
                  if (i>0)
                        mkdir(curpath);
            }
            fgets(buf,100,fp); /* |======================= */
            fgets(buf,100,fp); /* | File  O/S  Description */
            fgets(buf,100,fp); /* | ----  ---  ----------- */
            while (1)
            {
                  fgets(buf,100,fp);
                  if (feof(fp)) goto finished;
                  if (buf[0]=='\n')
                        break;
                  if (skiparea)
                        continue;
                  strtok(buf+2," ");
                  strcpy(oldfile,buf+2);
                  if (curpath[0]=='\0' || !stricmp(oldfile,"SNIPPETS.NDX"))
                        printf("%s (not moved)\n",oldfile);
                  else
                  {
                        sprintf(newfile,"%s\\%s",curpath,oldfile);
                        printf("%s -> %s\n",oldfile,newfile);
                        rename(oldfile,newfile);
                  }
            }
      }
finished:

      puts("\nFinished.");

      fclose(fp);

      return 0;
}
