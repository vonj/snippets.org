/*
**  POSIXDIR.C - POSIX-style directory processing
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include <errno.h>
#include "sniptype.h"
#include "dirent.h"
#include "filnames.h"         /* For dos2unix() */

#define _NDIRS 20

int DFerr;
DIR _DIRS[_NDIRS];            /* Initialize DIR array to zeros    */

/******************************************************************/
/*                                                                */
/*  opendir()                                                     */
/*                                                                */
/*  Function: Open a directory for reading.                       */
/*                                                                */
/*  Parameters: 1 - Directory name. May include path spec.        */
/*                                                                */
/*  Returns: Pointer to a DIR typedef'ed structure, similar       */
/*           to fopen() returning a FILE pointer.                 */
/*                                                                */
/*           NULL if error, DFerr set as follows:                 */
/*           Success_ - No error                                  */
/*           ENOENT   - Could not locate directory or contents    */
/*           ENOTDIR  - Not a directory                           */
/*           ENOMEM   - Too many directories already open         */
/*                                                                */
/*  Side effects: The dd_size element of the DIR structure        */
/*                will contain a number representing the total    */
/*                number of entries in the directory. The         */
/*                dd_loc element will be set to zero since        */
/*                no elements have, as yet, been read.            */
/*                                                                */
/******************************************************************/

DIR *opendir(char *fname)
{
      int i;
      unsigned n = 0;
      char *p;
      DOSFileData dstruct;

      for (i = 0; i < _NDIRS; ++i)
      {
            if (!_DIRS[i].dd_fd)
                  break;
      }
      if (_NDIRS <= i)
      {
            DFerr = ENOMEM;
            return NULL;
      }

      dos2unix(fname);
      if (':' == fname[1] && 1 < strlen(fname))
            p = &fname[2];
      else  p = fname;
      while ('/' == LAST_CHAR(p) && 1 < strlen(p))
            LAST_CHAR(p) = '\0';
        
      if (strcmp(p, "/") && strlen(p))
      {
            if (Success_ != (FIND_FIRST(fname, _A_ANY, &_DIRS[i].dd_buf)))
            {
                  DFerr = ENOENT;
                  return NULL;
            }
            if (!(_A_SUBDIR & _DIRS[i].dd_buf.attrib))
            {
                  DFerr = ENOTDIR;
                  return NULL;
            }
      }
      strcpy(_DIRS[i].dd_dirname, fname);
      if (!strlen(p))
            strcat(_DIRS[i].dd_dirname, ".");
      if ('/' != LAST_CHAR(_DIRS[i].dd_dirname))
            strcat(_DIRS[i].dd_dirname, "/");
      strcat(strupr(_DIRS[i].dd_dirname), "*.*");
      if (Success_ != FIND_FIRST(_DIRS[i].dd_dirname,
            _A_ANY, &_DIRS[i].dd_buf))
      {
            DFerr = ENOENT;
            return NULL;
      }
      memcpy(&dstruct, &_DIRS[i].dd_buf, sizeof(DOSFileData));
      do
      {
            ++n;
      } while (Success_ == FIND_NEXT(&_DIRS[i].dd_buf));
      memcpy(&_DIRS[i].dd_buf, &dstruct, sizeof(DOSFileData));
      _DIRS[i].dd_size = n;
      _DIRS[i].dd_loc  = 0;
      _DIRS[i].dd_fd   = i + 1;
      DFerr = Success_;
      return &_DIRS[i];
}

/******************************************************************/
/*                                                                */
/*  closedir()                                                    */
/*                                                                */
/*  Function: Close a previously opened directory.                */
/*                                                                */
/*  Parameters: 1 - DIR pointer of directory to close.            */
/*                                                                */
/*  Returns: Success_ or Error_.                                  */
/*                                                                */
/******************************************************************/

int closedir(DIR *dirp)
{
      if (0 == dirp->dd_fd || _NDIRS < dirp->dd_fd)
      {
            DFerr = EBADF;
            return Error_;
      }
      FIND_END(dirp->dd_buf);
      memset(dirp, 0, sizeof(DIR));
      return Success_;
}

/******************************************************************/
/*                                                                */
/*  rewinddir()                                                   */
/*                                                                */
/*  Function: Reset an open DIR to its first entry.               */
/*                                                                */
/*  Parameters: 1 - DIR pointer of directory to rewind.           */
/*                                                                */
/*  Returns: Success_ or Error_.                                  */
/*                                                                */
/******************************************************************/

int rewinddir(DIR *dirp)
{
      if (0 == dirp->dd_fd || _NDIRS < dirp->dd_fd)
      {
            DFerr = EBADF;
            return Error_;
      }
      FIND_FIRST(dirp->dd_dirname, _A_ANY, &(dirp->dd_buf));
      dirp->dd_loc = 0;
      return Success_;
}

/******************************************************************/
/*                                                                */
/*  seekdir()                                                     */
/*                                                                */
/*  Function: Point to a selected entry in a DIR.                 */
/*                                                                */
/*  Parameters: 1 - DIR pointer of directory to rewind.           */
/*              2 - Offset of entry to seek                       */
/*              3 - Origin of offset                              */
/*                                                                */
/*  Returns: A DOSFileData pointer, same as returned by DOS find  */
/*           first/next calls.                                    */
/*                                                                */
/*           NULL if error, DFerr set as follows:                 */
/*           Success_ - No error                                  */
/*           EBADF    - Bad file (DIR) pointer                    */
/*           EACCES   - Illegal origin specification              */
/*           EOF      - Attempt to seek past end of directory     */
/*                                                                */
/******************************************************************/

DOSFileData *seekdir(DIR *dirp, int offset, int origin)
{
      int i, loc;

      if (0 == dirp->dd_fd || _NDIRS < dirp->dd_fd)
      {
            DFerr = EBADF;
            return NULL;
      }
      switch (origin)
      {
      case SEEK_SET:
            loc = offset + 1;
            break;
      case SEEK_CUR:
            loc = dirp->dd_loc + offset;
            break;
      case SEEK_END:
            loc = dirp->dd_size + offset;
            break;
      default:
            DFerr = EACCES;
            return NULL;
      }

      if (loc > (int)dirp->dd_size || 0 >= loc)
      {
            DFerr = EOF;
            return NULL;
      }

      rewinddir(dirp);
      for (i = 0; i < loc; ++i)
            readdir(dirp);

      DFerr = Success_;
      return (&(dirp->dd_buf));
}

/******************************************************************/
/*                                                                */
/*  readdir()                                                     */
/*                                                                */
/*  Function: Reads entries from an open directory.               */
/*                                                                */
/*  Parameters: 1 - DIR pointer of directory to read.             */
/*                                                                */
/*  Returns: A DOSFileData pointer, same as returned by DOS find  */
/*           first/next calls.                                    */
/*                                                                */
/*           NULL if error, DFerr set as follows:                 */
/*           Success_ - No error                                  */
/*           EBADF    - Bad file (DIR) pointer                    */
/*           EOF      - Attempt to read past end of directory     */
/*                                                                */
/*  Side effects: The dd_loc element of the DIR structure         */
/*                will contain a number representing which        */
/*                element of the directory was returned. It may   */
/*                range from 1 to dd_size.                        */
/*                                                                */
/******************************************************************/

DOSFileData *readdir(DIR *dirp)
{
      if (0 == dirp->dd_fd || _NDIRS < dirp->dd_fd)
      {
            DFerr = EBADF;
            return NULL;
      }
      if (0 == dirp->dd_loc || Success_ == FIND_NEXT(&(dirp->dd_buf)))
      {
            dirp->dd_loc += 1;
            DFerr = Success_;
            return (&(dirp->dd_buf));
      }
      else
      {
            DFerr = EOF;
            return NULL;
      }
}
