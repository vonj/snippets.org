/*
 *  VIDMGR.C; Screen drawing, cursor and keyboard routines for text mode
 *            16-bit and 32-bit MS-DOS, 16-bit and 32-bit OS/2, and 32-bit
 *            Windows 95/NT applications.  Release 1.3.
 *
 *  This module written in March 1996 by Andrew Clarke and released to the
 *  public domain.  Last modified in October 1996.
 *
 *  VidMgr has been compiled and tested with the following C compilers:
 *
 *    - Borland C++ (16-bit) for DOS 3.1
 *    - Borland C++ (16-bit) for DOS 4.5
 *    - Borland C++ (32-bit) for OS/2 1.0
 *    - Cygnus GNU C (32-bit) for Windows 95/NT b14.0
 *    - DJGPP GNU C (32-bit) for DOS 2.0
 *    - EMX GNU C (32-bit) for OS/2 & DOS 0.9b
 *    - IBM VisualAge C/C++ 3.0 (32-bit) for OS/2
 *    - Microsoft C/C++ (16-bit) for OS/2 6.00a
 *    - Microsoft C/C++ (16-bit) for DOS 8.00c
 *    - Microsoft Quick C (16-bit) for DOS 2.50
 *    - Microsoft Visual C/C++ (16-bit) for DOS 1.52
 *    - WATCOM C/C++ (16-bit & 32-bit) for DOS 9.5
 *    - WATCOM C/C++ (16-bit & 32-bit) for DOS 10.0
 *    - WATCOM C/C++ (32-bit) for OS/2 10.0
 *    - WATCOM C/C++ (32-bit) for Windows 95/NT 10.0
 *    - HI-TECH Pacific C (16-bit) for DOS 7.51
 *    - Symantec C/C++ (16-bit) for DOS 7.0
 *    - Zortech C/C++ (16-bit) for DOS 3.0r4
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "vidmgr.h"

struct vm_info vm_startup;
char vm_curattr;

char vm_frame_blank[] = "      ";
char vm_frame_single[] = "ÚÄ¿³ÀÙ";
char vm_frame_double[] = "ÉÍ»ºÈ¼";

void vm_setattr(char attr)
{
    vm_curattr = attr;
}

void vm_printf(char x, char y, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    vm_puts(x, y, buffer);
}

void vm_printfcenter(char row, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    vm_puts((char)((vm_getscreenwidth() / 2) - (strlen(buffer) / 2)), row, buffer);
}

void vm_printfbetween(char x1, char x2, char row, const char *format,...)
{
    char x;
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    if ((char)strlen(buffer) >= (char)(x2 - x1 + 1))
    {
        vm_puts(x1, row, buffer);
    }
    else
    {
        x = (char)(x1 + (x2 - x1 + 1 - strlen(buffer)) / 2);
        vm_puts(x, row, buffer);
    }
}

void vm_xprintf(char x, char y, char attr, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    vm_xputs(x, y, attr, buffer);
}

void vm_xprintfcenter(char row, char attr, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    vm_xputs((char)((vm_getscreenwidth() / 2) - (strlen(buffer) / 2)), row, attr, buffer);
}

void vm_xprintfbetween(char x1, char x2, char row, char attr, const char *format,...)
{
    char x;
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    if ((char)strlen(buffer) >= (char)(x2 - x1 + 1))
    {
        vm_xputs(x1, row, attr, buffer);
    }
    else
    {
        x = (char)(x1 + (x2 - x1 + 1 - strlen(buffer)) / 2);
        vm_xputs(x, row, attr, buffer);
    }
}

void vm_paintclearscreen(char attr)
{
    vm_paintclearbox(1, 1, vm_getscreenwidth(), vm_getscreenheight(), attr);
}

void vm_paintclearline(char row, char attr)
{
    vm_paintclearbox(1, row, vm_getscreenwidth(), row, attr);
}

void vm_paintcleareol(char row, char attr)
{
    vm_paintclearbox(vm_wherex(), row, (char)(vm_getscreenwidth() - vm_wherex() + 1), row, attr);
}

void vm_paintscreen(char attr)
{
    vm_paintbox(1, 1, vm_getscreenwidth(), vm_getscreenheight(), attr);
}

void vm_paintline(char row, char attr)
{
    vm_paintbox(1, row, vm_getscreenwidth(), row, attr);
}

void vm_painteol(char row, char attr)
{
    vm_paintbox(vm_wherex(), row, (char)(vm_getscreenwidth() - vm_wherex() + 1), row, attr);
}

void vm_clearscreen(void)
{
    vm_clearbox(1, 1, vm_getscreenwidth(), vm_getscreenheight());
}

void vm_clearline(char row)
{
    vm_clearbox(1, row, vm_getscreenwidth(), row);
}

void vm_cleareol(char row)
{
    vm_clearbox(vm_wherex(), row, (char)(vm_getscreenwidth() - vm_wherex() + 1), row);
}

void vm_fillscreen(char ch)
{
    vm_fillbox(1, 1, vm_getscreenwidth(), vm_getscreenheight(), ch);
}

void vm_fillline(char row, char ch)
{
    vm_fillbox(1, row, vm_getscreenwidth(), row, ch);
}

void vm_filleol(char row, char ch)
{
    vm_fillbox(vm_wherex(), row, (char)(vm_getscreenwidth() - vm_wherex() + 1), row, ch);
}

void vm_clrscr(void)
{
    vm_paintclearscreen(vm_curattr);
    vm_gotoxy(1, 1);
}

void vm_clreol(void)
{
    vm_paintcleareol(vm_wherey(), vm_curattr);
}

void vm_vertline(char y1, char y2, char col, char attr, char ch)
{
    char y;
    for (y = y1; y <= y2; y++)
    {
        vm_xputch(col, y, attr, ch);
    }
}

void vm_frame(char x1, char y1, char x2, char y2, char attr, char *frame)
{
    vm_xputch(x1, y1, attr, frame[0]);
    vm_horizline((char)(x1 + 1), (char)(x2 - 1), y1, attr, frame[1]);
    vm_xputch(x2, y1, attr, frame[2]);
    vm_vertline((char)(y1 + 1), (char)(y2 - 1), x1, attr, frame[3]);
    vm_vertline((char)(y1 + 1), (char)(y2 - 1), x2, attr, frame[3]);
    vm_xputch(x1, y2, attr, frame[4]);
    vm_horizline((char)(x1 + 1), (char)(x2 - 1), y2, attr, frame[1]);
    vm_xputch(x2, y2, attr, frame[5]);
}
