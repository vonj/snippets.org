/*
**  OS_ID.C
**
**  Based upon public domain works by David Gibbs & Stephen Lindholm
*/

#define OS_ID_MAIN
#include "snpdosys.h"
#include <dos.h>

struct i_os_ver id_os_ver[TOT_OS];
int id_os_type;
int id_os;

const char *id_os_name[TOT_OS] = {
      "DOS",
      "OS/2 DOS",
      "DESQview",
      "Windows Std",
      "Windows 386"
      };

/*
**  get_os() - Determine OS in use
*/

int get_os (void)
{
      union REGS t_regs;
      int osmajor, osminor;
      unsigned status;

      id_os_type = 0;
      id_os = 0;

      /* test for DOS or OS/2 */

      t_regs.h.ah = 0x30;
      int86(0x21, &t_regs, &t_regs);
      osmajor = t_regs.h.al;
      osminor = t_regs.h.ah;

      if (osmajor < 10)
      {
            id_os_ver[DOS].maj = osmajor;
            id_os_ver[DOS].min = osminor;
            id_os_type = id_os_type | is_DOS;
      }
      else
      {
            /* OS/2 v1.x DOS Box returns 0x0A */

            id_os_type = id_os_type | is_OS2;

            /* OS/2 v2.x DOS Box returns 0x14 */

            id_os_ver[OS2].maj = osmajor/10;
            id_os_ver[OS2].min = osminor;
      }

      /* test for Windows */

      t_regs.x.ax = 0x1600;         /* check enhanced mode operation    */
      int86(0x2F, &t_regs, &t_regs);
      status = t_regs.h.al;

      if ((0x00 == status) || (0x80 == status))
      {
            /*
            ** Can't trust it...
            **  let's check if 3.1 is running in standard mode or what?
            */

            t_regs.x.ax = 0x160A;
            int86( 0x2F, &t_regs, &t_regs );
            if (0 == t_regs.x.ax)
            {
                  id_os_ver[WINS].maj = t_regs.h.bh;
                  id_os_ver[WINS].min = t_regs.h.bl;
                  id_os_type = id_os_type | is_WINS;
            }
      }
      else if ((0x01 == status) || (0xff == status))
      {
            id_os_ver[WINS].maj = 2;
            id_os_ver[WINS].min = 1;
            id_os_type = id_os_type | is_WINS;
      }
      else
      {
            id_os_ver[WINS].maj = t_regs.h.al;
            id_os_ver[WINS].min = t_regs.h.ah;
            id_os_type = id_os_type | is_WINS;
      }

      /* Test for DESQview */

      t_regs.x.cx = 0x4445;                /* load incorrect date */
      t_regs.x.dx = 0x5351;
      t_regs.x.ax = 0x2B01;                /*  DV set up call     */

      intdos(&t_regs, &t_regs);
      if (t_regs.h.al != 0xFF)
      {
            id_os_type = id_os_type | is_DV;
            id_os_ver[DV].maj = t_regs.h.bh;
            id_os_ver[DV].min = t_regs.h.bl;
      }

      if (id_os_type & is_DOS)
            id_os = DOS;
      if (id_os_type & is_WINS)
            id_os = WINS;
      if (id_os_type & is_WIN3)
            id_os = WIN3;
      if (id_os_type & is_DV)
            id_os = DV;
      if (id_os_type & is_OS2)
            id_os = OS2;

      return(id_os);
}

/*
**  Give up a time slice to the OS
*/

void t_slice(int t_os)
{
      union REGS t_regs;
    
      switch (t_os)
      {
      case DOS  :
            break;

      case OS2  :
      case WIN3 :
      case WINS :
            t_regs.x.ax = 0x1680;
            int86(0x2f,&t_regs,&t_regs);
            break;

      case DV   :
            t_regs.x.ax = 0x1000;
            int86(0x15,&t_regs,&t_regs);
            break;
      } /* switch(t_os) */
}


#ifdef TEST

#include <stdio.h>

int main(void)
{
      int ostype = get_os();

      printf("%s version %d.%d\n",
            id_os_name[ostype],
            id_os_ver[ostype].maj,
            id_os_ver[ostype].min);

      return(0);
}

#endif /* TEST */
