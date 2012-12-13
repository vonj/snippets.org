/*
**  DISK_SN.C - Retrieve a disk serial number
**
**  public domain demo by Paul Schlyter and Bob Stout
**  OS/2 compatibility added by Mark Kimes
*/

#include <stdio.h>

#ifdef OS2
 #define INCL_DOS

 #include <os2.h>
 #include <stdlib.h>
#else
 #include <dos.h>
#endif

#include "extkword.h"
#include "disk_sn.h"
#include "mk_fp.h"

/*
**  Arguments: 1 - Drive (0 = default, 1 = A:, 2 = B:, etc.)
**
**  Returns: Static string containing the disk serial number, or
**           an empty string if no serial number is available,
**
**  Notes: Drive errors result in an empty string returned, this function
**         does not invoke the critical error handler.
*/

char *get_disk_id(int drive)
{
      static char serial_no[10];
#ifndef OS2
      union REGS r;
      struct SREGS s;
      unsigned sno1, sno2;

      r.x.ax = 0x6900;
      r.h.bl = drive;
      segread(&s);
      intdosx(&r, &r, &s);
      if (r.x.cflag)
            *serial_no = '\0';
      else
      {
            sno2 = *((unsigned FAR *)MK_FP(s.ds, r.x.dx+2));
            sno1 = *((unsigned FAR *)MK_FP(s.ds, r.x.dx+4));
            sprintf(serial_no, "%04X-%04X\n", sno1, sno2);
      }
#else
      struct {
            ULONG serial;
            char  volumelength;
            char  volumelabel[CCHMAXPATH];
      } volser;

      DosError(FERR_DISABLEHARDERR);
      if(0 == DosQueryFSInfo(drive, FSIL_VOLSER, &volser, sizeof(volser)))
            sprintf(serial_no, "%04X-%04X", HIUSHORT(volser.serial),
                  LOUSHORT(volser.serial));
      else  *serial_no = 0;
#endif
      return serial_no;
}

#ifdef TEST

#include <ctype.h>

main(int argc, char *argv[])
{
      char *sn;

      if (2 > argc)
      {
            puts("Usage: DISK_SN drive_letter [...drive_letter]");
            return -1;
      }
      while (--argc)
      {
            int drive;

            drive = toupper(**++argv);
            sn = get_disk_id(drive - '@');
            printf("The serial number of %c: is %s\n", drive, sn);
      }
      return 0;
}

#endif /* TEST */
