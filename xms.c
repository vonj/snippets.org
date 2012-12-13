/*
** XMS.C
**
** Routines to use Extended Memory from a DOS program.
** NOTE: Uses inline assembly language.
**
** Released to the public domain by Cliff Rhodes with no guarantees
** of any kind.
*/

#include <dos.h>

#include "xms.h"

#define XMS_INT  0x002f     /* DOS Multiplex interrupt */

static int (_far * XMSDriver)(void);

static int initFlag = 0;

/*
** Verify that an Extended Memory Manager is installed.
**
** Returns 1 if manager found, 0 if not.
**
** NOTE: This function should be called before any other XMS function!
*/

int XMSinit(void)
{
      union REGS regs;
      struct SREGS sregs;

      regs.x.ax = 0x4300;             /* Verify XMS manager present */
      int86(XMS_INT, &regs, &regs);
      if(regs.h.al == 0)
            return 0;

      regs.x.ax = 0x4310;             /* Get XMS manager entry point */
      int86x(XMS_INT, &regs, &regs, &sregs);

      /* Save entry point */

      XMSDriver = (int (_far *)(void)) MK_FP(sregs.es, regs.x.bx);

      return (initFlag = 1);
}

/*
** Return version number of XMS driver, or 0 if not available.
*/

int XMSversion(void)
{
      if(!initFlag)
            return 0;

      _asm mov ax, 0;

      return XMSDriver();
}

/*
** Returns number of bytes available in largest free block.
*/

long  XMScoreleft(void)
{
      if(!initFlag)
            return 0L;

      _asm mov ax, 0x0800;

      return 1024L * (long) XMSDriver();
}

/*
** Attempts to allocate size bytes of extended memory.
**
** Returns handle if successful, 0 if not.
**
** NOTE: Actual size allocated will be the smallest multiple of 1024
**       that is larger than size.
*/

unsigned int XMSalloc(long size)
{
      /* Get the number of 1024 byte units required by size. */

      int rval = (int) (size / 1024L);

      if(size % 1024L)
            rval++;    /* Add a block for any excess */

      if(!initFlag)
            return 0;

      _asm
      {
            mov dx, rval;
            mov ax, 0x0900;
      }

      if(XMSDriver())
            _asm mov rval, dx;
      else  rval = 0;

      return rval;
}

/*
** Attempts to free extended memory referred to by handle. Returns 1
** if successful, 0 if not.
*/

int XMSfree(unsigned int handle)
{
      if(!initFlag)
            return 0;

      _asm
      {
            mov dx, handle;
            mov ax, 0x0a00;
      }

      return XMSDriver();
}

typedef struct {
   long         nbytes;     /* Number of bytes to move */
   unsigned int shandle;    /* Handle of source memory */
   long         soffset;    /* Offset of source in handle's memory area */
   unsigned int dhandle;    /* Handle of destination memory */
   long         doffset;    /* Offset of destination in memory */
} XMSRequestBlock;

static XMSRequestBlock bd;

static long XMSMove(long n)
{
      long rval;
      unsigned int segm, offs;
      XMSRequestBlock _far *fptr = (XMSRequestBlock _far *) &bd;

      if(!initFlag)
            return 0L;

      bd.nbytes = n;

      offs = FP_OFF(fptr);
      segm = FP_SEG(fptr);

      _asm
      {
            push ds;      /* Save DS */
            mov ds, segm;
            mov si, offs;
            mov ax, 0x0b00;
      }

      rval = (XMSDriver() == 0) ? 0L : n;

      _asm pop ds;    /* Restore DS since we changed it to make this call */

      return rval;
}

/*
** Attempts to copy n bytes from srchandle to desthandle memory areas.
** Returns number of bytes copied, or 0 on error.
*/

long XMSmemcpy(unsigned int desthandle, long destoff,
               unsigned int srchandle, long srcoff, long n)
{
      bd.shandle = srchandle;
      bd.soffset = srcoff;
      bd.dhandle = desthandle;
      bd.doffset = destoff;

      return XMSMove(n);
}

/*
** Attempts to copy n bytes from DOS src buffer to desthandle memory area.
** Returns number of bytes copied, or 0 on error.
*/

int DOStoXMSmove(unsigned int desthandle, long destoff,
                 const char *src, int n)
{
      bd.shandle = 0;
      bd.soffset = (long) ((char _far *) src);
      bd.dhandle = desthandle;
      bd.doffset = destoff;

      return (int) XMSMove((long) n);
}

/*
** Attempts to copy n bytes to DOS dest buffer from srchandle memory area.
** Returns number of bytes copied, or 0 on error.
*/

int XMStoDOSmove(char *dest, unsigned int srchandle, long srcoff, int n)
{
      bd.shandle = srchandle;
      bd.soffset = srcoff;
      bd.dhandle = 0;
      bd.doffset = (long) ((char _far *) dest);

      return (int) XMSMove((long) n);
}
