/*
** FILES.C:  A program to determine the number of file handles
**
** Released in to the Public Domain by Matthew Hunt @ 1:129/135, in the
** hopes that no "programmer" will be so lazy that he|she simple reads
** the CONFIG.SYS file ever again.
**
** Any improvements and modifications are welcome, but I ask that all
** modified versions also be placed into the Public Domain.
**
** Information on the List of Lists and SFT format was provided by
** PC Magazine November 26, 1991, and PC Interrupts by Ralf Brown
** and Jim Kyle.  FILES.C was originally written for Power C.
**
** Modifications for other compiler support by Bob Stout @ 1:106/2000.6
*/

#include <dos.h>
#include "dosfiles.h"
#include "mk_fp.h"

/*
** Walk the SFT linked list, counting file handles as we go
*/

int files(void)
{
      struct SFT_HEADER (FAR *sft);
      unsigned int segment, offset;
      int count=0;
      union REGS regs;
      struct SREGS sregs;

      /* Get ptr to List of Lists in ES:DX */

      regs.x.ax = 0x5200;
      segread(&sregs);
      intdosx(&regs, &regs, &sregs);

      /* Get ssss:oooo to first SFT  */

      segment = *((unsigned FAR *)(MK_FP(sregs.es, regs.x.bx + 6)));
      offset  = *((unsigned FAR *)(MK_FP(sregs.es, regs.x.bx + 4)));

      /* Point our structure to it.  */

      sft = MK_FP(segment, offset);

      do
      {
            count += sft->number;               /* Add the number of FILES */
            sft = sft->next;                    /* Point to next one       */
      } while(FP_OFF(sft->next) != 0x0FFFF);    /* Last one in the chain   */

      /* Add the FILES for the last entry */

      count += sft->number;
      return count;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Number of FILES entries: %d", files());
    return EXIT_SUCCESS;
}

#endif /* TEST */
