/*
**  SETVOL.C - set, change, or kill a disk volume label
**
**  public domain demo by Bob Stout
**  DOS 5 enhancements suggested by Keith Beedle
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <io.h>
#include "sniptype.h"
#include "dirport.h"
#include "snpdskio.h"
#include "dosfiles.h"
#include "mk_fp.h"

#if defined(__TURBOC__)
 #pragma option -a-
#else
 #if defined(__ZTC__)
  #pragma ZTC align 1
 #else /* MSC/QC/WATCOM/METAWARE */
  #pragma pack(1)
 #endif
 struct fcb_ {
         char   fcb_drive;
         char   fcb_name[8];
         char   fcb_ext[3];
         short  fcb_curblk;
         short  fcb_recsize;
         long   fcb_filsize;
         short  fcb_date;
         char   fcb_resv[10];
         char   fcb_currec;
         long   fcb_random;
 };

 struct xfcb_ {
         char           xfcb_flag;
         char           xfcb_resv[5];
         char           xfcb_attr;
         struct fcb_    xfcb_fcb;
 };

 #define fcb      fcb_
 #define xfcb     xfcb_
#endif

#include "dos5boot.h"   /* SNIPPETS file with DOS 5 boot record structure  */

/*
**  Erase an existing volume label
*/

void vol_kill(char *fname)
{
      union REGS regs;
      struct SREGS sregs;
      struct xfcb buf;

      /* Parse the filename into an FCB               */

      segread(&sregs);
      regs.h.ah = 0x29;
      regs.h.al = 0;
      regs.x.si = (unsigned)fname;
      regs.x.di = (unsigned)&buf.xfcb_fcb;
      sregs.es  = sregs.ds;
      intdosx(&regs, &regs, &sregs);

      /* Volume labels require extended FCB's         */

      buf.xfcb_flag = 0xff;
      buf.xfcb_attr = _A_VOLID;

      /* Delete the old label                         */

      regs.h.ah = 0x13;
      regs.x.dx = (unsigned)&buf;
      intdos(&regs, &regs);
}

/*
**  Create a new volume label
*/

void setvol(char *label)
{
      char new_label[13];     /* name + ext + '.' + NUL       */
      struct xfcb buf;
      union REGS regs;
      struct SREGS sregs;
      const char pattern[] = "????????";
      char FAR *dta;

      /*
      **  Change to root directory.
      */
      
      PushDir("\\");

      /* If drive is already labeled, remove it               */

      segread(&sregs);
      regs.h.ah = 0x2f;
      intdosx(&regs, &regs, &sregs);
      dta = MK_FP(sregs.es, regs.x.bx);
      
      buf.xfcb_flag = 0xff;
      buf.xfcb_attr = _A_VOLID;
      buf.xfcb_fcb.fcb_drive = 0;
      memcpy(buf.xfcb_fcb.fcb_name, pattern, 8);
      memcpy(buf.xfcb_fcb.fcb_ext, pattern, 3);

      regs.h.ah = 0x11;
      regs.x.dx = (unsigned)&buf;
      intdos(&regs, &regs);

      if (0 == regs.h.al)
      {
            int i;
            char oldlabel[13], FAR *p, *q;

            for (i = 0, p = dta + 8, q =oldlabel; i < 8; ++i, ++p, ++q)
            {
                  *q = *p;
            }
            *q++ = '.';
            for (i = 0, p = dta + 16; i < 3; ++i, ++p, ++q)
            {
                  *q = *p;
            }
            vol_kill(oldlabel);
      }

      strcpy(new_label, label);
      if (8 < strlen(label))
      {
            new_label[8] = '.';
            strcpy(&new_label[9], &label[8]);
      }

      /* Parse the filename into an FCB               */

      segread(&sregs);
      regs.h.ah = 0x29;
      regs.h.al = 0;
      regs.x.si = (unsigned)new_label;
      regs.x.di = (unsigned)&buf.xfcb_fcb;
      sregs.es  = sregs.ds;
      intdosx(&regs, &regs, &sregs);

      /* Volume labels require extended FCB's         */

      buf.xfcb_flag = 0xff;
      buf.xfcb_attr = _A_VOLID;

      /* Create the new label                         */

      regs.h.ah = 0x16;
      regs.x.dx = (unsigned)&buf;
      intdos(&regs, &regs);

      /* Close the new label                          */

      regs.h.ah = 0x10;
      regs.x.dx = (unsigned)&buf;
      intdos(&regs, &regs);

      /*
      **  For DOS 5.0 replace the boot record too.
      */

      if(_osmajor > 3)
      {
            int index, drive = getdrv();
            B_REC boot_record;

            AbsDiskRead(drive, 1, 0, &boot_record);
            if(0 == strcmp(boot_record.bsOemName, "MSDOS5.0"))
            {
                  index = 0;
                  while (NUL != label[index])
                  {
                        boot_record.bsVolumeLabel[index] = label[index];
                        index++;
                  }
                  for( ; index < 11; index++)
                        boot_record.bsVolumeLabel[index] = 0x20;
                  AbsDiskWrite(drive, 1, 0, &boot_record);
            }
      }
      PopDir();
}

#ifdef TEST

main(int argc, char *argv[])
{
      if (2 > argc)
      {
            puts("\aUsage: SETVOL new_name");
            abort();
      }
      setvol(argv[1]);
      return EXIT_SUCCESS;
}

#endif /* TEST */
