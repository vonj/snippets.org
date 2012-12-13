/*
** EMS.C
**
** Expanded Memory Functions
**
** Released to the public domain by Cliff Rhodes with no guarantees
** of any kind.
*/

#include <stdio.h>
#include <dos.h>

#include "ems.h"       

/* EMS driver values */
#define EMS_ID      "EMMXXXX0" /* EMS identifier string */
#define EMS_INT     0x67       /* EMS interrupt number */
#define EMS_VERSION 0x32       /* Version 3.2 of EMS */

/* EMS service codes */
#define EMSservice1  0x40    /* Get EMS status */
#define EMSservice2  0x41    /* Get segment address of page 0 */
#define EMSservice3  0x42    /* Get total number of expanded pages */
#define EMSservice4  0x43    /* Get handle and assign pages to it */
#define EMSservice5  0x44    /* Map a page into one of the page frames */
#define EMSservice6  0x45    /* Close EMS handle */
#define EMSservice7  0x46    /* Get the EMS version number */


/*
** Determines if EMS present. If so returns base segment of EMS.
** Returns 0 if EMS not available. The base segment is necessary
** for mapping EMS memory pages into the user address space (see
** EMSmap() below).
*/

unsigned int EMSbaseaddress(void)
{
      static const char tag[] = EMS_ID;

      char far      *p;
      int           i;
      union REGS    regs;
      struct SREGS  segs;

      regs.h.ah = 0x35;
      regs.h.al = EMS_INT;
      int86x(0x21, &regs, &regs, &segs);

      p = (char far *) (MK_FP(segs.es, 10)); /* EMS_ID must be at offset 10 */

      for(i = 0; i < 8; i++)
      {
            if(tag[i] != p[i])
                  return 0;                  /* If EMS_ID not found, return */
      }

      regs.h.ah = EMSservice2;               /* Get page frame segment */
      int86(EMS_INT, &regs, &regs);

      return regs.h.ah ? 0 : (unsigned int) regs.x.bx;
}

/*
** Returns current EMS version, -1 if not found or obsolete
*/

int EMSversion(void)
{
      union REGS regs;

      regs.h.ah = EMSservice7;
      int86(EMS_INT, &regs, &regs);

      if(!regs.h.ah && regs.h.al >= EMS_VERSION)  /* Make sure at least 3.2 */
            return regs.x.ax;

      return -1;
}

/*
** Returns 0 if EMS system OK, -1 if not
*/

int  EMSstatus(void)
{
      union REGS regs;

      regs.h.ah = EMSservice1;
      int86(EMS_INT, &regs, &regs);

      return regs.h.ah ? -1 : 0;
}

/*
** Returns number of free EMS pages (each page is 16k), -1 if error
*/

int EMSpages(void)
{
      union REGS  regs;

      regs.h.ah = EMSservice3;
      int86(EMS_INT, &regs, &regs);

      return regs.h.ah ? -1 : (int) regs.x.bx;
}

/*
** Returns handle to block of size pages or -1 if error.
** NOTE: always free any handles when you are done!.
*/

int EMSalloc(int pages)
{
      union REGS regs;

      regs.h.ah = EMSservice4;
      regs.x.bx = pages;
      int86(EMS_INT, &regs, &regs);

      return regs.h.ah ? -1 : (int) regs.x.dx;
}

/*
** Frees handle block, returns 0 if successful, -1 if error
*/

int EMSfree(int handle)
{
      union REGS regs;

      regs.h.ah = EMSservice6;
      regs.x.dx = handle;
      int86(EMS_INT, &regs, &regs);

      return regs.h.ah ? -1 : 0;
}

/*
** Maps page of handle into bank. Returns 0 if successful, -1 if error.
** Each handle controls 1 or more 16k pages of EMS memory.
** There are four banks 0-3. bank 0 starts at the segment returned by
** EMSbaseaddress(), bank 1 starts at that segment with offset 16k, etc.
*/

int  EMSmap(int bank, int handle, int page)
{
      union REGS regs;

      regs.h.ah = EMSservice5;
      regs.h.al = bank;
      regs.x.bx = page;
      regs.x.dx = handle;
      int86(EMS_INT, &regs, &regs);

      return regs.h.ah ? -1 : 0;
}
