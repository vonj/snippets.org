/*
**  DRVS.C - public domain by David Gersic, DeKalb, Il 1993
**
**  Routine checks how many valid disk drives are available on machine,
**  both physical and logical drives.
**
**  Includes drive letters assigned with DOS SUBST command and network
**  drives for Novell Netware (and probably other networks).
**
**  Compiled Under MSC 6 LARGE memory Model
**  Should be compatible with other DOS compilers
**
*/

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>

main()
{
      union REGS in, out;
      int i;

      /* Novell's shell TSRs allow up to 32 drive 'letters' to be created */

      char drives[]={' ','a','b','c','d','e','f','g','h','i','j',
                     'k','l','m','n','o','p','q','r','s','t','u',
                     'v','w','x','y','z','[','\\',']','^','_','`'};

      in.x.ax=0x4409;   /* IOCTL function - Check if block device remote  */
      for(i = 1; i < 32; i++)
      {
            in.h.bl=(unsigned char)i;     /* 1==a:, 2==b:, etc.           */
            intdos(&in,&out);
            if(!out.x.cflag)        /* carry flag set on error            */
            {                       /* bit 15 == subst, bit 12 == 'remote'*/
                  printf("drive %c: is %s\n",
                        drives[i],out.x.dx & (1<<15) ? "subst" :
                        out.x.dx & (1<<12) ? "network" : "local");
            }
      }
      return(0);
}
