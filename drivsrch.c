/*
**  DRIVSRCH.C - public domain by Marty Connelly, Victoria, BC 1992
**
**  Modified by Bob Stout
**  
**  Routine checks how many valid disk drives are available on machine,
**  both physical and logical drives
*/

/*
**  Includes drive letters assigned with DOS SUBST command
**
**  Networked drives are left as an exercise as I don't have access
**  to them to check.
**
**  The routine uses undocumented DOS interrupt 32H.
**
**  Compatible with MSC 5 and 6, ZTC++, BC++, other DOS compilers
**
**  DS:BX contains the address of the Disk Parameter Block (DPB) for a
**  requested drive. If the drive letter at offset 0 of the DPB doesn't
**  match the requested drive, then the drive has been SUBST'ed.
*/

#include <stdio.h>
#include <dos.h>
#include "extkword.h"
#include "mk_fp.h"

main()
{
      int i;
      int unsigned result;
      int drivestatus[26];
      unsigned char FAR *DPB;
      union REGS regs;
      struct SREGS sregs;


      /* routine checks for all valid drive possibilities from A to Z  */

      /*
      **    if removable media drive ie. floppy drive A: has a latch door
      **    open you will get "Abort Retry" panic message
      */

      for (i = 0; i < 26; i++)
      {
            /* drive number (0=default, 1=A, 2=B,etc.)*/

            regs.h.dl = (unsigned char)(i + 1);
            segread(&sregs);

            regs.h.ah=0x32;         /* DOS interrupt 32H */
                                    /* was undocumented for DOS release 3.2 */

            intdosx(&regs,&regs, &sregs);

            result=regs.h.al;
            DPB = MK_FP(sregs.ds, regs.x.bx);

            /*
            **  result =0  then valid drive
            **         =255 or ff hex then invalid or non-existent drive
            */

            if (0 == result && *DPB != (unsigned char)i)
                  drivestatus[i] = 1;
            else  drivestatus[i]=result;
      }

      for (i = 0; i < 26; i = i + 2)
      {
            printf("drive %c: status code =%3d drive %c: status code =%3d\n",
                  'A' + i,drivestatus[i],'B' + i,drivestatus[i+1]);
      }
      return 0;
}
