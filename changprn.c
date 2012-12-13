/*
**  change_prn()
**
**  A function to change the standard printer device for the duration
**  of a program. Valid new device codes are:
**
**      0 - LPT1
**      1 - LPT2
**      2 - LPT3
**      3 - COM1
**      4 - COM2
**      5 - CON
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include "sniprint.h"               /* Includes PrintDevice typedef     */

int change_prn(PrintDevice device)
{
      char *newdev;

      switch (device)
      {
      case LPT1:
            newdev = "LPT1";
            break;
      case LPT2:
            newdev = "LPT2";
            break;
      case LPT3:
            newdev = "LPT3";
            break;
      case COM1:
            newdev = "COM1";
            break;
      case COM2:
            newdev = "COM2";
            break;
      case CON:
            newdev = "CON";
            break;
      default:
            return -1;
      }
      return (NULL == freopen(newdev, "w", stdprn));
}
