/*
**  Public domain by Paul Schlyter, 27-Apr-1994
**
**  modified for SNIPPETS by Bob Stout
**
**  Pass: 0 for drive A:, 1 for drive B:, 2 for drive C:, etc.
**
**  Returns: True_  if the drive is a CD-ROM
**           False_ if the drive is not a CD-ROM
**           Error_ if MSCDEX not installed
*/

#include "dosfiles.h"

Boolean_T isCDROMdrive(int drive)
{
      union REGS r;

      r.x.ax = 0x1500;              /* First test for presence of MSCDEX */
      r.x.bx = 0;
      int86( 0x2F, &r, &r );
      if ( r.x.bx == 0 )
            return Error_;          /* MSCDEX not there                  */
      r.x.ax = 0x150B;              /* MSCDEX driver check API           */
      r.x.cx = drive - 'A';
      int86( 0x2F, &r, &r );
      return  r.x.ax != 0;          /* Drive is CDROM if AX nonzero      */
}
