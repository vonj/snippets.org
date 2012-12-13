/*
**  DVIDPORT.C - Direct video portability functions
*/

#ifdef __ZTC__                            /* Only used with SC/ZTC      */

#include <disp.h>
#include "scrnmacs.h"

void gotoxy(int col, int row)
{
      if (disp_inited)
            disp_move(row, col);
      else  GotoXY(col, row);
}

#endif /* __ZTC__ */
