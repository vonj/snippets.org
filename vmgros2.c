/*
 *  VMGROS2.C; VidMgr module for OS/2 compilers.  Release 1.2.
 *
 *  This module written in March 1996 by Andrew Clarke and released to the
 *  public domain.  Last modified in May 1996.
 */

#define INCL_KBD
#define INCL_VIO
#define INCL_DOSPROCESS

#include <string.h>
#include <os2.h>
#include "vidmgr.h"

#ifndef KBDTRF_FINAL_CHAR_IN
#define KBDTRF_FINAL_CHAR_IN  FINAL_CHAR_IN
#endif

static void vm_setcursorsize(char start, char end);
static void vm_getcursorsize(char *start, char *end);

void vm_init(void)
{
    vm_getinfo(&vm_startup);
    vm_setattr(vm_startup.attr);
}

void vm_done(void)
{
    vm_setcursorsize(vm_startup.cur_start, vm_startup.cur_end);
}

void vm_getinfo(struct vm_info *v)
{
    v->ypos = vm_wherey();
    v->xpos = vm_wherex();
    v->attr = vm_getattrxy(v->xpos, v->ypos);
    v->height = vm_getscreenheight();
    v->width = vm_getscreenwidth();
    vm_getcursorsize(&v->cur_start, &v->cur_end);
}

char vm_getscreenwidth(void)
{
    VIOMODEINFO vi;
    vi.cb = sizeof(VIOMODEINFO);
    VioGetMode(&vi, 0);
    return vi.col;
}

char vm_getscreenheight(void)
{
    VIOMODEINFO vi;
    vi.cb = sizeof(VIOMODEINFO);
    VioGetMode(&vi, 0);
    return vi.row;
}

short vm_getscreensize(void)
{
    return (short)(vm_getscreenwidth() * vm_getscreenheight() * 2);
}

char vm_wherex(void)
{
    USHORT row, col;
    VioGetCurPos(&row, &col, 0);
    return (char)(col + 1);
}

char vm_wherey(void)
{
    USHORT row, col;
    VioGetCurPos(&row, &col, 0);
    return (char)(row + 1);
}

void vm_gotoxy(char x, char y)
{
    VioSetCurPos((USHORT) (y - 1), (USHORT) (x - 1), 0);
}

static void vm_setcursorsize(char start, char end)
{
    VIOCURSORINFO vi;
    vi.yStart = start;
    vi.cEnd = end;
    vi.cx = 0;
    vi.attr = 0;
    VioSetCurType(&vi, 0);
}

static void vm_getcursorsize(char *start, char *end)
{
    VIOCURSORINFO vi;
    VioGetCurType(&vi, 0);
    *start = vi.yStart;
    *end = vi.cEnd;
}

int vm_kbhit(void)
{
    KBDKEYINFO ki;
    ki.fbStatus = 0;
    KbdPeek(&ki, 0);
    return ki.fbStatus & KBDTRF_FINAL_CHAR_IN;
}

int vm_getch(void)
{
    KBDKEYINFO ki;

    ki.chChar = 0;
    ki.chScan = 0;
    KbdCharIn(&ki, IO_WAIT, 0);

    if (ki.chChar == 0xe0)
    {
        if (ki.chScan)
        {
            ki.chChar = 0;      /* force scan return */
        }
        else
        {                       /* get next block */
            ki.chChar = 0;
            KbdCharIn(&ki, IO_WAIT, 0);
            if (!ki.chScan)
            {                   /* still no scan? */
                ki.chScan = ki.chChar;  /* move new char over */
                ki.chChar = 0;  /* force its return */
            }
            else
            {
                ki.chChar = 0;  /* force new scan */
            }
        }
    }
    if (ki.chScan == 0xe0)
    {
        if (!ki.chChar)
        {
            ki.chScan = 0;
            KbdCharIn(&ki, IO_WAIT, 0);
            if (!ki.chScan)
            {                   /* still no scan? */
                ki.chScan = ki.chChar;  /* move new char over */
                ki.chChar = 0;  /* force its return */
            }
            else
            {
                ki.chChar = 0;  /* force new scan */
            }
        }
        else
        {
            ki.chScan = 0;      /* handle 0xe00d case */
        }
    }
    if (ki.chChar)
    {
        ki.chScan = 0;
    }

    return (int)((ki.chScan << 8) + (ki.chChar));
}

char vm_getchxy(char x, char y)
{
    char ch;
    USHORT len = 1;
    VioReadCharStr(&ch, &len, (USHORT) (y - 1), (USHORT) (x - 1), 0);
    return ch;
}

char vm_getattrxy(char x, char y)
{
    char cell[4];
    USHORT len = 4;
    VioReadCellStr(cell, &len, (USHORT) (y - 1), (USHORT) (x - 1), 0);
    return *(cell + 1);
}

void vm_xgetchxy(char x, char y, char *attr, char *ch)
{
    char cell[4];
    USHORT len = 4;
    VioReadCellStr(cell, &len, (USHORT) (y - 1), (USHORT) (x - 1), 0);
    *ch = *cell;
    *attr = *(cell + 1);
}

void vm_setcursorstyle(int style)
{
    VIOCURSORINFO vi;

    switch (style)
    {
    case CURSORHALF:
        vi.yStart = -50;
        vi.cEnd = -100;
        vi.cx = 0;
        vi.attr = 0;
        VioSetCurType(&vi, 0);
        break;
    case CURSORFULL:
        vi.yStart = 0;
        vi.cEnd = -100;
        vi.cx = 0;
        vi.attr = 0;
        VioSetCurType(&vi, 0);
        break;
    case CURSORNORM:
        vi.yStart = -90;
        vi.cEnd = -100;
        vi.cx = 0;
        vi.attr = 0;
        VioSetCurType(&vi, 0);
        break;
    case CURSORHIDE:
        vi.yStart = -90;
        vi.cEnd = -100;
        vi.cx = 0;
        vi.attr = -1;
        VioSetCurType(&vi, 0);
        break;
    default:
        break;
    }
}

void vm_putch(char x, char y, char ch)
{
    VioWrtCharStr(&ch, 1, (USHORT) (y - 1), (USHORT) (x - 1), 0);
}

void vm_puts(char x, char y, char *str)
{
    VioWrtCharStr(str, (USHORT) strlen(str), (USHORT) (y - 1), (USHORT) (x - 1), 0);
}

void vm_xputch(char x, char y, char attr, char ch)
{
    VioWrtCharStrAtt(&ch, 1, (USHORT) (y - 1), (USHORT) (x - 1), (PBYTE) &attr, 0);
}

void vm_xputs(char x, char y, char attr, char *str)
{
    VioWrtCharStrAtt(str, (USHORT) strlen(str), (USHORT) (y - 1), (USHORT) (x - 1), (PBYTE) &attr, 0);
}

void vm_putattr(char x, char y, char attr)
{
    VioWrtNAttr((PBYTE) &attr, 1, (USHORT) (y - 1), (USHORT) (x - 1), 0);
}

void vm_paintclearbox(char x1, char y1, char x2, char y2, char attr)
{
    char y, cell[2];
    cell[0] = ' ';
    cell[1] = attr;
    for (y = y1; y <= y2; y++)
    {
        VioWrtNCell((PBYTE) &cell, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
    }
}

void vm_paintbox(char x1, char y1, char x2, char y2, char attr)
{
    char y;
    for (y = y1; y <= y2; y++)
    {
        VioWrtNAttr((PBYTE) &attr, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
    }
}

void vm_clearbox(char x1, char y1, char x2, char y2)
{
    char y, ch;
    ch = ' ';
    for (y = y1; y <= y2; y++)
    {
        VioWrtNChar((PBYTE) &ch, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
    }
}

void vm_fillbox(char x1, char y1, char x2, char y2, char ch)
{
    char y;
    for (y = y1; y <= y2; y++)
    {
        VioWrtNChar((PBYTE) &ch, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
    }
}

void vm_gettext(char x1, char y1, char x2, char y2, char *dest)
{
    USHORT width;
    char y;
    width = (USHORT) ((x2 - x1 + 1) * 2);
    for (y = y1; y <= y2; y++)
    {
        VioReadCellStr((PBYTE) dest, &width, (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
        dest += width;
    }
}

void vm_puttext(char x1, char y1, char x2, char y2, char *srce)
{
    USHORT width;
    char y;
    width = (USHORT) ((x2 - x1 + 1) * 2);
    for (y = y1; y <= y2; y++)
    {
        VioWrtCellStr((PBYTE) srce, width, (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
        srce += width;
    }
}

void vm_horizline(char x1, char x2, char row, char attr, char ch)
{
    char cell[2];
    cell[0] = ch;
    cell[1] = attr;
    VioWrtNCell((PBYTE) &cell, (USHORT) (x2 - x1 + 1), (USHORT) (row - 1), (USHORT) (x1 - 1), 0);
}
