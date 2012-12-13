/* MTERM.C - Minimal example PC terminal program.
   Released to public domain by author, David Harmon, June 1992.
   Intended for use with a FOSSIL driver, but will run with BIOS alone.
   I expect you'll want to add something for practical purposes. ;-)
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>            /* kbhit(), getch(), putch(), etc. */
#include <dos.h>              /* int86(), etc. */

#ifdef __ZTC__
 #define cputs(s) fputs((s),stderr)
#endif

int port = 0;                 /* 0 = COM1:, 1 = COM2: etc. */
int local_echo = 0;
int cr_add_lf = 0;
int exiting = 0;

int init_comm(int flags)
{
      union REGS regs;

      regs.h.ah = 0x04;         /* initialize driver (port) */
      regs.x.bx = 0x4f50;
      regs.x.dx = port;
      int86( 0x14, &regs, &regs);

      regs.h.ah = 0x00;         /* set baud rate * port attrs */
      regs.h.al = (unsigned char)flags;
      regs.x.dx = port;
      int86( 0x14, &regs, &regs);
      return regs.h.ah;
}

void send_char(char ch)
{
      union REGS regs;

      regs.h.ah = 0x01;         /* Send char (wait until ready)*/
      regs.h.al = ch;
      regs.x.dx = port;
      int86( 0x14, &regs, &regs);
}

int   input_ready(void)
{
      union REGS regs;

      regs.h.ah = 0x03;         /* Get port status */
      regs.x.dx = port;
      int86( 0x14, &regs, &regs);
      return ((regs.h.ah & 0x01) != 0);   /* input ready */
}

int   get_char(void)
{
      union REGS regs;

      regs.h.ah = 0x02;         /* receive char (wait if necessary)*/
      regs.x.dx = port;
      int86( 0x14, &regs, &regs);
      return regs.h.al;
}

void deinit_comm(void)
{
      union REGS regs;

      regs.h.ah = 0x05;         /* deinitialize port (pseudo close) */
      regs.h.al = 0x00;         /* (lower DTR) */
      regs.x.dx = port;
      int86( 0x14, &regs, &regs);
}

main()
{
      int ch;

      init_comm(0xE3);          /* hard coded 0xB3 = 2400,N,8,1 */
      cputs("MTERM ready!   Press F1 to exit.\r\n");
      while (!exiting)
      {
            if (kbhit())        /* key was hit */
            {
                  ch = getch();     /* Regular ASCII keys are returned as the
                                       ASCII code; function keys, arrows, etc.
                                       as zero followed by a special code
                                       (on next getch.)  */
                  if (ch != 0)
                  {
                        send_char((char)ch);    /* to com port */
                        if (local_echo)
                        {
                              putch(ch);        /* to screen */

                              /* add LF to CR? */

                              if (cr_add_lf && ch == '\r')
                                    putch('\n');
                        }
                  }
                  else
                  {
                        ch = getch();       /* get the special key code */
                        switch (ch)
                        {
                        case 0x3B: /* F1 */
                              exiting = 1;               /* quit now */
                              break;

                        case 0x3C: /* F2 */
                              local_echo = !local_echo;  /* toggle echo */
                              break;

                        case 0x3D: /* F3 */
                              cr_add_lf = !cr_add_lf;    /* toggle LF */
                              break;
                        }
                  }
            } /* end if kbhit */

            if (input_ready())       /* com port */
            {
                  ch = get_char();
                  putch(ch);
                  if (cr_add_lf && ch == '\r')  /* add LF to CR? */
                        putch('\n');
            }
      } /* end while not exiting */
      deinit_comm();
      return 0;
}    
