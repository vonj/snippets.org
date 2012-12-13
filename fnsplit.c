/************************************************************************/
/*                                                                      */
/*  FNSPLIT.C                                                           */
/*                                                                      */
/*  Contains routines for parsing file/path names.                      */
/*                                                                      */
/*  Original Copyright 1989-96 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "filnames.h"

/******************************************************************/
/*                                                                */
/*  has_wild()                                                    */
/*                                                                */
/*  Checks a string for wildcard characters ('?' and '*')         */
/*                                                                */
/*  Arguments 1 - String to check                                 */
/*                                                                */
/*  Returns: True_ if string contains wildcards, else False_      */
/*                                                                */
/*  Side Effects: None                                            */
/*                                                                */
/******************************************************************/

Boolean_T has_wild(char *pname)
{
      if (NULL != strchr(pname, '*') || NULL != strchr(pname, '?'))
            return True_;
      else  return False_;
}

/******************************************************************/
/*                                                                */
/*  fnSplit()                                                     */
/*                                                                */
/*  Splits file specifications into component parts. Similar to   */
/*  compiler-specific fnsplit() or _splitpath().                  */
/*                                                                */
/*  Arguments 1 - Original file specification                     */
/*            2 - Buffer to receive drive spec                    */
/*            3 - Buffer to receive drive/path spec               */
/*            4 - Buffer to receive path spec                     */
/*            5 - Buffer to receive name.ext spec                 */
/*            6 - Buffer to receive name spec                     */
/*            7 - Buffer to receive ext spec                      */
/*                                                                */
/*  Returns: Bit map as follows (see defines in RBS.H):           */
/*           Extension_ - File spec included extension            */
/*           Filename_  - File spec did not end in '\'            */
/*           Directory_ - File spec included a path               */
/*           Drive_     - File spec included a drive spec         */
/*           Wildname_  - File name included wildcards (*.?)      */
/*           Wildpath_  - File path included wildcards (*.?)      */
/*                                                                */
/*  Side Effects: Calls dos2unix() to convert '\' to '/'          */
/*                                                                */
/*  Notes: Passing NULL in arguments 2-7 causes fnsplit() to      */
/*         not save the corresponding portion of the path.        */
/*                                                                */
/******************************************************************/

int fnSplit(char *spec,             /* Original file spec         */
            char *drive,            /* Drive spec                 */
            char *pname,            /* Path w/ drive spec         */
            char *path,             /* Path spec                  */
            char *fname,            /* File name + extension      */
            char *name,             /* File name                  */
            char *ext)              /* File extension             */
{
      int ret_code = 0;
      char *d = spec, *p, *e;

      dos2unix(spec);

      if (':' == spec[1])
      {
            if (drive)
                  strncpy(drive, spec, 2);
            drive[2] = NUL;
            d += 2;
            ret_code |= Drive_;
      }
      else
      {
            if (drive)
                  *drive = NUL;
      }

      if (NULL != (p = strrchr(d, '/')))
      {
            char ch;

            ch = *(++p);
            *p = NUL;
            if (path)
                  strcpy(path, d);
            if (pname)
                  strcpy(pname, spec);
            if (has_wild(d))
                  ret_code |= Wildpath_;
            *p = ch;
            ret_code |= Directory_;
      }
      else
      {
            if (path)
                  *path = NUL;
            if (pname)
            {
                  if (drive)
                        strcpy(pname, drive);
                  else  *pname = NUL;
            }
            p = d;

            if ('.' == *p)
            {
                  size_t dot_length;

                  ret_code |= Directory_;
                  for (dot_length = 0; '.' == p[dot_length]; ++dot_length)
                        ;
                  if (path)
                  {
                        strncat(path, p, dot_length);
                        strcat(path, "\\");
                  }
                  if (pname)
                  {
                        strncat(pname, p, dot_length);
                        strcat(pname, "\\");
                  }
                  if (fname)
                        *fname = NUL;
                  if (name)
                        *name  = NUL;
                  if (ext)
                        *ext   = NUL;
                  return ret_code;
            }
      }
      if (fname)
            strcpy (fname, p);
      if (has_wild(p))
            ret_code |= Wildname_;
      if (*p)
            ret_code |= Filename_;
      if (NULL != (e = strrchr(p, '.')))
      {
            *e = NUL;
            if (name)
                  strcpy(name, p);
            *e = '.';
            if (ext)
                  strcpy(ext, e);
            ret_code |= Extension_;
      }
      else
      {
            if (name)
                  strcpy(name,p);
            if (ext)
                  *ext = NUL;
      }
      return ret_code;
}

/******************************************************************/
/*                                                                */
/*  fnMerge()                                                     */
/*                                                                */
/*  Creates file specification from component parts. Similar to   */
/*  compiler-specific fnmerge() or _makepath().                   */
/*                                                                */
/*  Arguments 1 - Buffer to receive file specification            */
/*            2 - drive specification                             */
/*            3 - drive/path specification                        */
/*            4 - path specification                              */
/*            5 - name.ext specification                          */
/*            6 - name specification                              */
/*            7 - ext specification                               */
/*                                                                */
/*  Returns: Reassembled name                                     */
/*                                                                */
/*  Side Effects: None                                            */
/*                                                                */
/******************************************************************/

char *fnMerge(char *spec,                 /* File spec buffer     */
              char *drive,                /* Drive spec           */
              char *pname,                /* Path w/ drive spec   */
              char *path,                 /* Path spec            */
              char *fname,                /* File name + extension*/
              char *name,                 /* File name            */
              char *ext)                  /* File extension       */
{
      *spec = NUL;

      if (pname && *pname)
            strcpy(spec, pname);
      else
      {
            if (drive && *drive)
                  strcpy(spec, drive);
            if (path && *path)
                  strcpy(spec, path);
      }

      dos2unix(spec);

      if (*spec && '/' != LAST_CHAR(spec) && (':' != LAST_CHAR(spec)))
            strcat(spec, "/");

      if (fname && *fname)
            strcat(spec, fname);
      else
      {
            if (name && *name)
                  strcat(spec, name);
            else  return spec;
            if (ext && *ext)
            {
                  if ('.' != *ext)
                        strcat(spec, ".");
                  strcat(spec, ext);
            }
      }
//      return strupr(spec);
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      char pname[FILENAME_MAX], drive[3], path[FILENAME_MAX];
      char fname[13], name[9], ext[4], fullname[FILENAME_MAX];

      if (2 > argc)
      {
            puts("\aUsage: FNSTST [d:][path{\\|/}][file][.ext]");
            abort();
      }
      printf("fnSplit(%s) returned \n", argv[1]);
      printf("%02Xh\n",
            fnSplit(argv[1], drive, pname, path, fname, name, ext));
      printf("drive: %s\n", drive);
      printf("pname: %s\n", pname);
      printf("path : %s\n", path);
      printf("fname: %s\n", fname);
      printf("name : %s\n", name);
      printf("ext  : %s\n", ext);

      printf("\nCalling fnMerge() to reassemble everything returned %s\n",
             fnMerge(fullname, drive, pname, path, fname, name, ext));
}

#endif /* TEST */
