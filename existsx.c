/************************************************************************/
/*                                                                      */
/*  EXISTSX.C                                                           */
/*                                                                      */
/*  POSIX-compliant functions to verify the existance of files in the   */
/*  PATH or in other environment variables.                             */
/*                                                                      */
/*  Original Copyright 1990-93 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif
#include "snipfile.h"
#include "snip_str.h"

/*
**  exists()
**
**  See if a file exists.
**
**  Parameters: 1 - File name
**
**  Returns: True_ if found, else False_.
**
*/

Boolean_T exists(char *name)
{
      return(Success_ == access(name, F_OK));
}

/*
**  dexists()
**
**  See if a file exists in an environment variable.
**
**  Parameters: 1 - File name
**              2 - Environment variable name
**
**  Returns: The pathname of the file which was found or NULL if the file
**           was not found.
**
**  Side effects: The returned path name is a static string. Subesequent
**                calls to any of these [d/p/g]exists() functions will
**                destroy the previous contents.
**
**  Notes: Uses stptok() and dos2unix(), also in SNIPPETS. May require
**         strupr() from SNIPPETS if it's not in your standard library.
*/

char *dexists(char *name, char *envar)
{
      char *path, *ptr;
      static char newname[FILENAME_MAX];
      int i, j;
      char chr;

      if (exists(name))
            return(name);

      if (NULL == (path = getenv(strupr(envar))))
            return(NULL);                       /* no environment var.  */

      dos2unix(path);
      
      do
      {
            path = stptok(path, newname, FILENAME_MAX, ";");
            if (LAST_CHAR(newname) != '/')
                  strcat(newname, "/");
            strcat(newname, name);
            if (exists(newname))
                  return(newname);
      } while (path && *path);

      return NULL;
}

/*
**  pexists()
**
**  See if a file exists in the PATH.
**
**  Parameters: 1 - File name
**
**  Returns: The pathname of the file which was found or NULL if the file
**           was not found.
**
**  Side effects: The returned path name is a static string. Subesequent
**                calls to any of these [d/p/g]exists() functions will
**                destroy the previous contents.
*/

char *pexists(char *name)
{
      return dexists(name, "PATH");
}

/*
**  gexists()
**
**  See if a file exists in the PATH or in an environment variable.
**
**  Parameters: 1 - File name
**              2 - Environment variable name
**
**  Returns: The pathname of the file which was found or NULL if the file
**           was not found.
**
**  Side effects: The returned path name is a static string. Subesequent
**                calls to any of these [d/p/g]exists() functions will
**                destroy the previous contents.
*/

char *gexists(char *name, char *envar)
{
      char *tmp;

      if (exists(name))
            return(name);
      if (NULL != (tmp = dexists(name, envar)))
            return(tmp);
      return(pexists(name));
}

#ifdef TEST

main(int argc, char *argv[])
{
      char *fname;

      switch (argc)
      {
      case 2:
            fname = pexists(argv[1]);
            if (fname)
                  printf("%s found in PATH\n", fname);
            else  printf("%s not found in PATH\n", argv[1]);
            break;
      case 3:
            fname = gexists(argv[1], argv[2]);
            if (fname)
                  printf("%s found in PATH or %s\n", fname, argv[2]);
            else  printf("%s not found in PATH or %s\n",
                        argv[1], argv[2]);
            break;
      default:
            puts("\aUsage: EXISTSX fname [envar]");
            return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;
}

#endif /* TEST */
