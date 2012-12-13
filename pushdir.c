/*
**  PushDir() and PopDir()
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dosfiles.h"
#if defined(MSDOS) || defined(__MSDOS__)
 #include "unistd.h"
#else
 #include <unistd.h>
#endif

#define DIR_STACK_SIZE  8
#define MAX_FLEN        67

static int  PushDir_stack_ptr;
static char PushDir_stack[DIR_STACK_SIZE][MAX_FLEN];

/*
**  PushDir()
**
**  Like chdir(), except a drive may be specified and the old directory
**  is saved.
**
**  Arguments: 1 - newdir, the buffer containing the new directory name
**
**  Returns:  -1 - stack overflow
**             0 - error
**             1 - success, still on same drive
**             2 - success, changed drive
**
**  Side effects: Converts name in newdir to upper case and prepends
**                a drive letter.
**
**  CAUTION: Since a drive will be prepended to newdir, its buffer
**           should be at least MAX_FLEN long.
*/

int PushDir(char *newdir)
{
      char pname[MAX_FLEN];
      char drive[3];
      char *target = &pname[2];
      int  new_drv = 0, ercode = 0;
      static int init = 0;

      if (!init)
            PushDir_stack_ptr = init = -1;
      if (DIR_STACK_SIZE <= ++PushDir_stack_ptr)
      {
            ercode = -1;
            goto ErrEx;
      }
      getcwd(PushDir_stack[PushDir_stack_ptr], MAX_FLEN);
      strupr(PushDir_stack[PushDir_stack_ptr]);
      strncpy(drive, PushDir_stack[PushDir_stack_ptr], 2);
      drive[2] = '\0';
      if (':' == newdir[1])
      {     /* If a drive is specified                                  */
            strupr(newdir);
            strcpy(pname, newdir);
            if (strchr(target, ':'))      /* if filename is illegal     */
                  goto ErrEx;
            if (*drive != *newdir)
            {
                  if (Error_ == chdrv(newdir[0] - 'A'))
                  {     /* If the drive is invalid                      */
                        goto ErrEx;
                  }
                  else  new_drv = 1;
            }
      }
      else
      {     /* If a drive isn't specified                               */
            if (!strchr(strupr(newdir), ':'))
            {     /* If legal filename                                  */
                  strcpy(pname, drive);
                  strcat(pname, newdir);
                  strcpy(newdir, pname);
            }
            else
            {     /* If filename is illegal                             */
                  goto ErrEx;
            }
      }

      if (*target)
      {
            if (chdir(target))
            {
                  if (1 == new_drv) /* We already changed drives        */
                        chdrv(*drive - 'A');    /* Go home before exit  */
                  goto ErrEx;
            }
      }
      return (new_drv + 1);
ErrEx:
      --PushDir_stack_ptr;
      return (ercode);
}

/*
**  PopDir()
**
**  Like chdir(), except goes to the drive/directory specified on the
**  top of the PushDir stack.
**
**  Arguments: none
**
**  Returns:  -1 - stack empty
**             0 - error - stack pointer unchanged
**             1 - success, still on same drive
**             2 - success, changed drive
**
**  Side effects: none
**
**  CAUTION: chdir() or chdrv() should not be called between PushDir-
**           PopDir calls.
*/

int PopDir(void)
{
      char I_am_here[MAX_FLEN], target_drv, *target;
      int new_drv = 0;

      if (0 > PushDir_stack_ptr)
            return -1;
      getcwd(I_am_here, MAX_FLEN);
      target = &PushDir_stack[PushDir_stack_ptr][2];
      target_drv = PushDir_stack[PushDir_stack_ptr][0];
      if (I_am_here[0] != target_drv)
      {
            if (Error_ == chdrv(target_drv - 'A'))
                  return 0;
            new_drv = 1;
      }
      if (!chdir(target))
      {
            --PushDir_stack_ptr;
            return (1 + new_drv);
      }
      else  return 0;
}

/*
**  isdir()
**
**  Checks to see if a drive and/or path are a valid directory.
**
**  Arguments: 1 - dir, the buffer containing the new directory name
**
**  Returns: Error_ - push/popdir stack overflow
**           False_ - not a valid directory
**           True_  - valid directory
**
**  Side effects: Converts name in dir to upper case and prepends a
**                drive letter.
**
**  CAUTION: Since a drive will be prepended to newdir, it's buffer
**           should be at least MAX_FLEN long.
*/

int isdir(char *dir)
{
      int ercode;

      if (-1 == (ercode = PushDir(dir)))
            return ercode;
      if (ercode)
            PopDir();
      return TOBOOL(ercode);
}
