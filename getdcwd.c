/*
** GETDCWD.C - returns the current working directory for a specific drive
**
** public domain by Bob Jarvis, modified by Bob Stout
** OS/2 compatibility mods by Ed Blackman
*/

#include "extkword.h"

#if defined(__OS2__)
 #define INCL_DOSFILEMGR    /* DosQueryCurrentDisk/Dir   */
 #define INCL_DOSMISC       /* DosError, DosQuerySysInfo */
 #include <os2.h>
 #define GetDrive(d) os2_getdrive(&d)

 void os2_getdrive(unsigned int *drive)
 {
       ULONG dummy;

       DosQueryCurrentDisk((ULONG *)&drive, &dummy);
 }

#elif defined(__ZTC__)
 #define GetDrive(d) dos_getdrive(&d)
#elif defined(__TURBOC__)
 #define GetDrive(d) ((d) = getdisk() + 1)
#else /* assume MSC */
 #define GetDrive(d) _dos_getdrive(&d)
#endif

#ifndef __OS2__
 #include <dos.h>
#endif      /* !__OS2__ */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "dosfiles.h"

char *getdcwd(unsigned int drive)   /* 0 = current, 1 = A, 2 = B, etc */
{
      char *retptr, FAR *fretptr;
      unsigned long pathMaxLen = FILENAME_MAX + 4;

#ifdef __OS2__
      APIRET rc;
#else
      union REGS regs;
      struct SREGS sregs;
#endif


#ifdef __OS2__
/*
** it's bad practice in OS/2 to use hard coded values for things like
** the maximum length of paths and such.  Query the system instead.
*/
      rc = DosQuerySysInfo(QSV_MAX_PATH_LENGTH, QSV_MAX_PATH_LENGTH,
            &pathMaxLen, sizeof(pathMaxLen));
      if(0 != rc)
            return NULL;
#endif

      retptr = calloc((size_t)pathMaxLen, sizeof(char));
      if(retptr == NULL)
            return NULL;
      else  fretptr = (char FAR *)retptr;

      if(drive == 0)    /* figure out which drive is current */
            GetDrive(drive);

      *retptr = (char)((drive-1) + 'A');
      *(retptr+1) = ':';
      *(retptr+2) = '\\';

#ifdef __OS2__
      pathMaxLen -= 3;
      DosError(FERR_DISABLEHARDERR); /* disable "drive not ready" popups */

      rc = DosQueryCurrentDir(drive, retptr + 3, &pathMaxLen);

      DosError(FERR_ENABLEHARDERR);  /* re-enable popups */
#else
      segread(&sregs);
      regs.h.ah = 0x47;
      regs.h.dl = (unsigned char)drive;
      sregs.ds  = FP_SEG(fretptr);
      regs.x.si = FP_OFF(fretptr) + 3;

      intdosx(&regs, &regs, &sregs);
#endif

#ifdef __OS2__
      if (0 != rc)             /* drive number invalid or other error */
#else
      if (15 == regs.x.ax)     /* drive number invalid */
#endif
      {
            free(retptr);
            return NULL;
      }
      else  return retptr;
}

#ifdef TEST

main(int argc, char *argv[])
{
      char *curpath;
      unsigned int n;

      if(argc > 1)
            n = (tolower(*argv[1]) - 'a') + 1;
      else  n = 0;
      
      printf("curpath = '%s'\n", curpath = getdcwd(n));
      if (curpath)
            free(curpath);
      return 0;
}

#endif /* TEST */
