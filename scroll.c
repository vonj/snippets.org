/*--------------------------[ scroll ]--------------------------*/
/*      Scroll the active page up or down a number of lines     */
/*              Public domain code by Jeff Dunlop:              */
/*--------------------------------------------------------------*/
/* input:                                                       */
/*      dx = direction                                          */
/*      num_lines = number of lines to scroll, 0 = clear coords */
/*      attr = attribute of blank line(s)                       */
/*      y1, x1, y2, x2 = corner coordinates of scroll window    */
/* local:                                                       */
/*      regs = register union for ISR                           */
/*--------------------------------------------------------------*/
 
#include <dos.h>
#include "scrnmacs.h"
 
void scroll(int direction,
            int num_lines,
            int vattrib,
            int ulrow,
            int ulcomumn,
            int lrrow,
            int lrcolumn)
{
      union REGS regs;
 
      /*
            BH = attribute to be used on blank line
            CH = row of upper left corner of scroll window
            CL = column of upper left corner of scroll window
            DH = row of lower right corner of scroll window
            DL = column of lower right corner of scroll window
      */
 
      regs.h.al = (unsigned char)num_lines;
      regs.h.bh = (unsigned char)vattrib;
      regs.h.ch = (unsigned char)ulrow;
      regs.h.cl = (unsigned char)ulcomumn;
      regs.h.dh = (unsigned char)lrrow;
      regs.h.dl = (unsigned char)lrcolumn;
 
      if (direction == SCROLL_UP)
            regs.h.ah = 0x06;
      else  regs.h.ah = 0x07;
 
      int86(0x10, &regs, &regs);
}
