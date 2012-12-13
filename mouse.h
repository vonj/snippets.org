/*      module:         mouse.h
 *      programmer:     Ray L. McVay
 *      started:        26oct86
 *      updated:        26oct86
 *
 *      Some handy mouse interface functions.
 */

#ifndef MOUSE__H
#define MOUSE__H

#include "extkword.h"

#define MSMOUSE 0x33

extern int mouse_present;

int  ms_reset(int *);
void ms_show_cursor(void);
void ms_hide_cursor(void);
int  ms_get_mouse_pos(int *, int *);
void ms_set_mouse_pos(int, int);
int  ms_button_press_status(int, int *, int *, int *);
int  ms_button_release_status(int, int *, int *, int *);
void ms_restrict_horiz(int, int);
void ms_restrict_horiz(int, int);
void ms_define_window(int, int, int, int);
void ms_set_graphics_cursor(int, int, unsigned, unsigned);
void ms_set_text_cursor(int, int, int);
void ms_read_motion_counters(int *, int *);
void ms_set_event_subroutine(int, unsigned, unsigned);
void ms_light_pen_on(void);
void ms_light_pen_off(void);
void ms_set_sensitivity(int, int);
void ms_protect_area(int, int, int, int);
int  ms_set_large_graphics_cursor(int, int, int, int, unsigned, unsigned);
void ms_set_doublespeed_threshold(int);

#endif /* MOUSE__H */
