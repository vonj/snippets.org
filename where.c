/*
**  WHERE.C:  will search all DIRs on the given drive for specified file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#if defined(__ZTC__)
 #include <direct.h>
 #define GetDrive(d) dos_getdrive(&d)
 #define SetDrive(d) {unsigned x;dos_setdrive(d,&x);}
#elif defined(__TURBOC__)
 #include <dir.h>
 #define GetDrive(d) ((d) = getdisk() + 1)
 #define SetDrive(d) (setdisk(d - 1))
#else /* assume MSC */
 #include <direct.h>
 #define GetDrive(d) _dos_getdrive(&d)
 #define SetDrive(d) {unsigned x;_dos_setdrive(d,&x);}
#endif

#include "dirport.h"

int count=0;

main(int argc, char *argv[])
{
      char *curdir,
            sought[80],
           *temp;
      int   newdrive, p;
      void  searchdir(char *dir, char *ptrn);
      unsigned curdrive;

      /*  Find out where we are */

      curdir=getcwd(NULL,80);
      GetDrive(curdrive);

      /*  Find out what we're looking for  */

      if(argc>1)
            strcpy(sought,argv[1]);
      else
      {
            printf("\n\nPattern to search for: ");
            gets(sought);
      }

      /*  Get designator for another drive if specified  */

      if(sought[1]==':')
      {
            newdrive=(toupper(sought[0]))-64;       /* convert  */
            SetDrive(newdrive);
            p = (sought[2]=='\\') ? 3:2;
            strcpy(sought, &(sought[p]));
      }

      /*  Add wildcard prefix/suffix if necessary  */

      if(sought[0]=='.')
      {
            temp=strcat("*",sought);        /*  set prefix  */
            strcpy(sought,temp);
      }
      if(!strchr(sought,'.'))
            strcpy(sought,"*.*");           /*  set suffix  */

      /*  Perform search for pattern starting in root  */

      searchdir("\\",sought);
      printf("\nNumber of matches: %d",count);

      /*  Restore Original Drive and Directory  */

      SetDrive(curdrive);
      chdir(curdir);
      return EXIT_SUCCESS;
}

/*------------------------------------------------------------------------- */

void searchdir(char *path, char *ptrn)
{
      DOSFileData   *f;
      char          *wholepath;
      unsigned       rtn;

      chdir(path);                        /*  change to new path  */
      wholepath = getcwd(NULL, 80);       /*  get full path name  */
      f = malloc(sizeof(*f));

      /*  Search for filename matches in this directory  */

      rtn = FIND_FIRST(ptrn, _A_ANY, f);
      while (rtn == 0)
      {
            if (!(ff_attr(f) & _A_SUBDIR ))
                  printf("%s\\%s\n",wholepath,ff_name(f));
            else  printf("%s\\%s <DIR>\n", wholepath, ff_name(f));
            ++count;

            rtn = FIND_NEXT(f);           /* find next match      */
      }  /* end while loop  */

      /*  Now search any subdirectories under this directory  */

      rtn = FIND_FIRST("*.*", _A_SUBDIR, f);
      while (rtn == 0)
      {
            if ((ff_attr(f) & _A_SUBDIR) && (ff_name(f)[0] != '.'))
            {
                  searchdir(ff_name(f), ptrn);  /* recursive call */
                  chdir(wholepath);
            }
            rtn = FIND_NEXT(f);           /* search next dir      */
      }

      free(wholepath);
      free(f);
}
