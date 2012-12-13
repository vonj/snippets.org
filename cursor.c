/*****************************************************************/
/**   CURSOR()                                                  **/
/**   ARGUMENTS: A char variable identifying what to do with    **/
/**              the cursor.                                    **/
/**   RETURN: none                                              **/
/**                                                             **/
/**   DESCRIPTION:  This function receives a character which    **/
/**                 tells it to do one of several things.       **/
/**                 Turn the cursor on or off, change its size, **/
/**                 save the cursor size & position, restore the**/
/**                 size, restore the position, or restore both.**/
/**                                                             **/
/**   COMMANDS:   'S' - Save cursor size and position           **/
/**               'R' - Restore cursor size and position        **/
/**               'P' - Restore cursor position                 **/
/**               'Z' - Restore cursor size                     **/
/**               'H' - Hide cursor (cursor off)                **/
/**               'O' - Cursor on                               **/
/**               'N' - Cursor normal (same  as 'O')            **/
/**               'B' - Cursor big                              **/
/**                                                             **/
/**   BY Bill Wilkie, 1988; Bob Stout, 1995                     **/
/*****************************************************************/

#include <dos.h>
#include <ctype.h>
#include "cursor.h"

static unsigned position;       /* global to hold cursor position*/
static unsigned size;           /* global to hold cursor size    */

void cursor(int tmp)
{
      union REGS inregs,outregs;                /* cpu registers */

      switch (toupper(tmp))
      {
      case 'S' :                             /* SAVE CURSOR DATA */
            inregs.h.ah = 3;    /* read cursor position and size */
            inregs.h.bh = 0;                   /* from page zero */
            int86(0x10,&inregs,&outregs);
            position = outregs.x.dx;           /* store position */
            size = outregs.x.cx;                   /* store size */
            break;

      case 'P' :                      /* RESTORE CURSOR POSITION */
            inregs.h.ah = 2;              /* set cursor position */
            inregs.h.bh = 0;                     /* on page zero */
            inregs.x.dx = position;      /* at this old position */
            int86(0x10,&inregs,&outregs);
            break;

      case 'R' :               /* RESTORE CURSOR SIZE & POSITION */
            inregs.h.ah = 2;              /* set cursor position */
            inregs.h.bh = 0;                     /* on page zero */
            inregs.x.dx = position;      /* at this old position */
            int86(0x10,&inregs,&outregs);
            /* fall through to... */

      case 'Z' :                          /* RESTORE CURSOR SIZE */
            inregs.h.ah = 1;                  /* set cursor size */
            inregs.x.cx = size;                /* to saved value */
            int86(0x10,&inregs,&outregs);
            break;

      case 'H' :                                   /* CURSOR OFF */
            inregs.h.ah = 1;                  /* set cursor size */
            inregs.h.ch = 0x20;      /* set bit turns cursor off */
            int86(0x10,&inregs,&outregs);
            break;

      case 'O' :                                    /* CURSOR ON */
      case 'N' :                                /* CURSOR NORMAL */
            inregs.h.ah = 1;                  /* set cursor size */
            inregs.h.ch = 6;                /* cursor start line */
            inregs.h.cl = 7;                  /* cursor end line */
            int86(0x10,&inregs,&outregs);
            break;

      case 'B' :                                   /* CURSOR BIG */
            inregs.h.ah = 1;                  /* set cursor size */
            inregs.h.ch = 4;                /* cursor start line */
            inregs.h.cl = 7;                  /* cursor end line */
            int86(0x10,&inregs,&outregs);
            break;
      }
}
