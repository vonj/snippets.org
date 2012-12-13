/*
**  OS/2 Reboot function
**
**  public domain by Mark Kimes
*/

#if defined(__MSDOS__) || defined(MSDOS)
 #error OS2_BOOT.C is NOT intended for DOS programs ...
#endif

#define INCL_DOS

#include <os2.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
      HFILE  hf;
#ifdef __32BIT__
      ULONG  dummy;
      ULONG  rc;
#else
      USHORT dummy;
      USHORT rc;
#endif

      rc = DosOpen("DOS$",&hf,&dummy,0L,FILE_NORMAL,FILE_OPEN,
            OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE |
            OPEN_FLAGS_FAIL_ON_ERROR,NULL);
      if(!rc)
      {
            /* Flush the caches           */

            DosShutdown(1);

            /* Shut down the file system  */

            DosShutdown(1);
            rc = DosShutdown(0);
            if(!rc)
            {
                  /* reboot machine */
                  
#ifdef __32BIT__
                  rc = DosDevIOCtl(hf,0xd5,0xab,NULL,0,NULL,NULL,0,NULL);
#else
                  rc = DosDevIOCtl(NULL,NULL,0xab,0xd5,hf);
#endif
                  DosClose(hf);
            }
      }
      return rc;
}
