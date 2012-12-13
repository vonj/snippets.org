/*
** A series of routines to provide access to MicroSoft (and compatible)
** mice.  Consult your mouse documentation for detailed information regarding
** each mouse driver function.
**
** by Bob Jarvis w/ modifications by Bob Stout & Bruce Wedding
*/

#include <dos.h>
#include "mouse.h"

int mouse_present = 0;  /* globally visible */

#define DOS_INT 0x21

#define MOUSE(workregs) int86(MSMOUSE,&workregs,&workregs)
#define MOUSEX(workregs,sregs) int86x(DOS_INT,&workregs,&workregs,&sregs)

/*
** Uses driver function 0 to initialize the mouse software to its default
** settings.  If no mouse is present it returns 0.  If a mouse is present, it
** returns -1, and places the value of the mouse type (2 = MicroSoft,
** 3 = Mouse Systems, other values are possible) in *mousetype.  Also
** initializes the global variable mouse_present (0 = no mouse, !0 = mouse
** is available).
*/

int ms_reset(int *mousetype)
{
      union REGS workregs;
      struct SREGS sregs;

      /* check the vector     */

      segread (&sregs);
      workregs.h.ah = 0x35;     /* DOS get vector */
      workregs.h.al = 0x33;     /* mouse vector   */
      intdosx(&workregs, &workregs, &sregs);

      /* ES:BX now contains the pointer to the interrupt handler */

      if (sregs.es == 0 && workregs.x.bx == 0)
            return mouse_present = 0;

      workregs.x.ax = 0;
      MOUSE(workregs);
      *mousetype = workregs.x.bx;
      mouse_present = workregs.x.ax;
      return(mouse_present);
}

/*
** Makes the mouse cursor visible.
*/

void ms_show_cursor(void)
{
      union REGS workregs;

      workregs.x.ax = 1;
      MOUSE(workregs);
}

/*
** Hides the mouse cursor.  Should be called before changing any portion of
** the screen under the mouse cursor.
*/

void ms_hide_cursor(void)
{
      union REGS workregs;

      workregs.x.ax = 2;
      MOUSE(workregs);
}

/*
** Obtains information about the mouse position and button status.
** Places the current horizontal and vertical positions in *horizpos and
** *vertpos, respectively.  Returns the mouse button status, which is
** mapped at the bit level as follows:
**    Bit 0 - left button    \
**    Bit 1 - right button    >-- 0 = button up, 1 = button down
**    Bit 2 - middle button  /
*/

int ms_get_mouse_pos(int *horizpos, int *vertpos) /* Returns button status */
{
      union REGS workregs;

      workregs.x.ax = 3;
      MOUSE(workregs);
      *horizpos = workregs.x.cx;
      *vertpos  = workregs.x.dx;
      return(workregs.x.bx);
}

/*
** Moves the mouse cursor to a new position.
*/

void ms_set_mouse_pos(int horizpos, int vertpos)
{
      union REGS workregs;

      workregs.x.ax = 4;
      workregs.x.cx = horizpos;
      workregs.x.dx = vertpos;
      MOUSE(workregs);
}

/*
** Obtains information about the last time the specified button
** (0 = left, 1 = right, 2 = middle) was pressed.  Returns the current
** button status (same format as return from ms_get_mouse_pos() above).
*/

int ms_button_press_status(int  button,
                           int *press_count,
                           int *column,
                           int *row)
{
      union REGS workregs;

      workregs.x.ax = 5;
      workregs.x.bx = button;
      MOUSE(workregs);
      *press_count = workregs.x.bx;
      *column = workregs.x.cx;
      *row = workregs.x.dx;
      return(workregs.x.ax);
}

/*
** Similar to above but obtains information about the last release of the
** specified button.
*/

int ms_button_release_status(int  button,
                             int *release_count,
                             int *column,
                             int *row)
{
      union REGS workregs;

      workregs.x.ax = 6;
      workregs.x.bx = button;
      MOUSE(workregs);
      *release_count = workregs.x.bx;
      *column = workregs.x.cx;
      *row = workregs.x.dx;
      return(workregs.x.ax);
}

/*
** Forces the mouse cursor to remain within the range specified.
*/

void ms_restrict_horiz(int min, int max)
{
      union REGS workregs;

      workregs.x.ax = 7;
      workregs.x.cx = min;
      workregs.x.dx = max;
      MOUSE(workregs);
}

/*
** Forces the mouse cursor to remain within the range specified.
*/

void ms_restrict_vert(int min, int max)
{
      union REGS workregs;

      workregs.x.ax = 8;
      workregs.x.cx = min;
      workregs.x.dx = max;
      MOUSE(workregs);
}

void ms_define_window(int left, int top, int right, int bottom)
{
      ms_restrict_horiz(left,right);
      ms_restrict_vert(top,bottom);
}

/*
** Allows the user to set the graphics cursor to a new shape.  Check your
** mouse reference manual for full information about the use of this function.
*/

void ms_set_graphics_cursor(int      horiz_hotspot,
                            int      vert_hotspot,
                            unsigned seg_shape_tables,
                            unsigned offset_shape_tables)
{
      union REGS workregs;
      struct SREGS segregs;

      workregs.x.ax = 9;
      workregs.x.bx = horiz_hotspot;
      workregs.x.cx = vert_hotspot;
      workregs.x.dx = offset_shape_tables;
      segregs.es  = seg_shape_tables;
      MOUSEX(workregs, segregs);
}

/*
** Selects either the software or hardware cursor and sets the start and stop
** scan lines (for the hardware cursor) or the screen and cursor masks (for
** the software cursor).  Consult your mouse reference for more information.
*/

void ms_set_text_cursor(int type, int screen_mask, int cursor_mask)
{
      union REGS workregs;

      workregs.x.ax = 10;
      workregs.x.bx = type;
      workregs.x.cx = screen_mask;
      workregs.x.dx = cursor_mask;
      MOUSE(workregs);
}

/*
** Obtains the horizontal and vertical raw motion counts since the last
** request.
*/

void ms_read_motion_counters(int *horiz, int *vert)
{
      union REGS workregs;

      workregs.x.ax = 11;
      MOUSE(workregs);
      *horiz = workregs.x.cx;
      *vert  = workregs.x.dx;
}

/*
** Sets up a subroutine to be called when a given event occurs.
** NOTE: Use with extreme care.  The function whose address is provided MUST
**    terminate with a far return (i.e. must be compiled using large model).
**    Also, no DOS or BIOS services may be used, as the user-defined function
**    is (in effect) an extension to an interrupt service routine.
*/

void ms_set_event_subroutine(int      mask,
                             unsigned seg_routine,
                             unsigned offset_routine)
{
      union REGS workregs;
      struct SREGS segregs;

      workregs.x.ax = 12;
      workregs.x.cx = mask;
      workregs.x.dx = offset_routine;
      segregs.es  = seg_routine;
      MOUSEX(workregs, segregs);
}

/*
** Turns light pen emulation mode on.
*/

void ms_light_pen_on(void)
{
      union REGS workregs;

      workregs.x.ax = 13;
      MOUSE(workregs);
}

/*
** turns light pen emulation mode off.
*/

void ms_light_pen_off(void)
{
      union REGS workregs;

      workregs.x.ax = 14;
      MOUSE(workregs);
}

/*
** Sets the sensitivity of the mouse.  Defaults are 8 and 16 for horizontal
** and vertical sensitivity (respectively).
*/

void ms_set_sensitivity(int horiz, int vert)
{
      union REGS workregs;

      workregs.x.ax = 15;
      workregs.x.cx = horiz;
      workregs.x.dx = vert;
      MOUSE(workregs);
}

/*
** Sets up a region of the screen inside of which the mouse cursor will
** automatically be 'hidden'.
*/

void ms_protect_area(int left, int top, int right, int bottom)
{
      union REGS workregs;

      workregs.x.ax = 16;
      workregs.x.cx = left;
      workregs.x.dx = top;
      workregs.x.si = right;
      workregs.x.di = bottom;
      MOUSE(workregs);
}

/*
* Similar to ms_set_graphics_cursor() but allows a larger cursor.  Consult
** your mouse documentation for information on how to use this function.
*/

int ms_set_large_graphics_cursor(int      width,
                                 int      height,
                                 int      horiz_hotspot,
                                 int      vert_hotspot,
                                 unsigned seg_shape_tables,
                                 unsigned offset_shape_tables)
{
      union REGS workregs;
      struct SREGS segregs;

      workregs.x.ax = 18;
      workregs.x.bx = (width << 8) + horiz_hotspot;
      workregs.x.cx = (height << 8) + vert_hotspot;
      workregs.x.dx = offset_shape_tables;
      segregs.es  = seg_shape_tables;
      MOUSEX(workregs, segregs);
      if(workregs.x.ax == (unsigned)-1)
            return(workregs.x.ax); /* Return -1 if function 18 supported */
      else  return(0);             /* else return 0                      */
}

/*
** Sets the threshold value for doubling cursor motion.  Default value is 64.
*/

void ms_set_doublespeed_threshold(int speed)
{
      union REGS workregs;

      workregs.x.ax = 19;
      workregs.x.dx = speed;
      MOUSE(workregs);
}
