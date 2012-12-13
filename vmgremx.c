/*
 *  VMGREMX.C; VidMgr module for the EMX GNU C/C++ compiler.  Release 1.2.
 *
 *  This module written in April 1996 by Andrew Clarke and released to the
 *  public domain.  Last modified in June 1996.
 */

#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <sys/video.h>

#define INCL_KBD
#define INCL_VIO
#define INCL_DOSPROCESS

#include <os2.h>

#include "vidmgr.h"
#include "opsys.h"

static int vm_iscolorcard(void);
static void vm_setcursorsize(char start, char end);
static void vm_getcursorsize(char *start, char *end);
static void vm_getkey(unsigned char *chScan, unsigned char *chChar);

static int video_init = 0;

#define vi_init()  { if (!video_init) video_init = v_init(); }
#define vi_done()  { video_init = 0; }

void vm_init(void)
{
    vi_init();
    vm_getinfo(&vm_startup);
    vm_setattr(vm_startup.attr);
    if (_osmode == DOS_MODE)
    {
        opsysDetect();
    }
}

void vm_done(void)
{
    vm_setcursorsize(vm_startup.cur_start, vm_startup.cur_end);
    vi_done();
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
    if (_osmode == DOS_MODE)
    {
        int width, height;
        vi_init();
        v_dimen(&width, &height);
        return (char)width;
    }
    else
    {
        VIOMODEINFO vi;
        vi.cb = sizeof(VIOMODEINFO);
        VioGetMode(&vi, 0);
        return vi.col;
    }
}

char vm_getscreenheight(void)
{
    if (_osmode == DOS_MODE)
    {
        int width, height;
        vi_init();
        v_dimen(&width, &height);
        return (char)height;
    }
    else
    {
        VIOMODEINFO vi;
        vi.cb = sizeof(VIOMODEINFO);
        VioGetMode(&vi, 0);
        return vi.row;
    }
}

short vm_getscreensize(void)
{
    return (short)(vm_getscreenwidth() * vm_getscreenheight() * 2);
}

char vm_wherex(void)
{
    if (_osmode == DOS_MODE)
    {
        int row, col;
        vi_init();
        v_getxy(&col, &row);
        return (char)(col + 1);
    }
    else
    {
        USHORT row, col;
        VioGetCurPos(&row, &col, 0);
        return (char)(col + 1);
    }
}

char vm_wherey(void)
{
    if (_osmode == DOS_MODE)
    {
        int row, col;
        vi_init();
        v_getxy(&col, &row);
        return (char)(row + 1);
    }
    else
    {
        USHORT row, col;
        VioGetCurPos(&row, &col, 0);
        return (char)(row + 1);
    }
}

static int vm_iscolorcard(void)
{
    int hw = v_hardware();
    return hw == V_COLOR_8 || hw == V_COLOR_12 ? 1 : 0;
}

void vm_gotoxy(char x, char y)
{
    if (_osmode == DOS_MODE)
    {
        vi_init();
        v_gotoxy((int)(x - 1), (int)(y - 1));
    }
    else
    {
        VioSetCurPos((USHORT) (y - 1), (USHORT) (x - 1), 0);
    }
}

static void vm_setcursorsize(char start, char end)
{
    if (_osmode == DOS_MODE)
    {
        vi_init();
        v_ctype(start, end);
    }
    else
    {
        VIOCURSORINFO vi;
        vi.yStart = start;
        vi.cEnd = end;
        vi.cx = 0;
        vi.attr = 0;
        VioSetCurType(&vi, 0);
    }
}

static void vm_getcursorsize(char *start, char *end)
{
    if (_osmode == DOS_MODE)
    {
        int cstart, cend;
        vi_init();
        v_getctype(&cstart, &cend);
        *start = (char)cstart;
        *end = (char)cend;
    }
    else
    {
        VIOCURSORINFO vi;
        VioGetCurType(&vi, 0);
        *start = vi.yStart;
        *end = vi.cEnd;
    }
}

static void vm_getkey(unsigned char *chScan, unsigned char *chChar)
{
    union REGS regs;
    regs.h.ah = 0x00;
    _int86(0x16, &regs, &regs);
    *chScan = regs.h.ah;
    *chChar = regs.h.al;
}

int vm_kbhit(void)
{
    if (_osmode == DOS_MODE)
    {
        union REGS regs;
        static unsigned short counter = 0;
        if (counter % 10 == 0)
        {
            opsysTimeSlice();
        }
        counter++;
        regs.h.ah = 0x01;
        _int86(0x16, &regs, &regs);
        return !(regs.x.flags & 0x40);
    }
    else
    {
        KBDKEYINFO ki;
        ki.fbStatus = 0;
        KbdPeek(&ki, 0);
        return ki.fbStatus & KBDTRF_FINAL_CHAR_IN;
    }
}

int vm_getch(void)
{
    if (_osmode == DOS_MODE)
    {
        unsigned char chChar, chScan;

        while (!vm_kbhit())
        {
            /* nada */
        }

        vm_getkey(&chScan, &chChar);
        if (chChar == 0xe0)
        {
            if (chScan)
            {
                chChar = 0;     /* force scan return */
            }
            else
            {                   /* get next block */
                chChar = 0;

                vm_getkey(&chScan, &chChar);
                if (!chScan)
                {               /* still no scan? */
                    chScan = chChar;  /* move new char over */
                    chChar = 0; /* force its return */
                }
                else
                {
                    chChar = 0; /* force new scan */
                }
            }
        }
        if (chScan == 0xe0)
        {
            if (!chChar)
            {
                chScan = 0;

                vm_getkey(&chScan, &chChar);
                if (!chScan)
                {               /* still no scan? */
                    chScan = chChar;  /* move new char over */
                    chChar = 0; /* force its return */
                }
                else
                {
                    chChar = 0; /* force new scan */
                }
            }
            else
            {
                chScan = 0;     /* handle 0xe00d case */
            }
        }
        if (chChar)
        {
            chScan = 0;
        }

        return (int)((chScan << 8) + (chChar));
    }
    else
    {
        KBDKEYINFO ki;

        ki.chChar = 0;
        ki.chScan = 0;
        KbdCharIn(&ki, IO_WAIT, 0);

        if (ki.chChar == 0xe0)
        {
            if (ki.chScan)
            {
                ki.chChar = 0;  /* force scan return */
            }
            else
            {                   /* get next block */
                ki.chChar = 0;
                KbdCharIn(&ki, IO_WAIT, 0);
                if (!ki.chScan)
                {               /* still no scan? */
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
                {               /* still no scan? */
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
                ki.chScan = 0;  /* handle 0xe00d case */
            }
        }
        if (ki.chChar)
        {
            ki.chScan = 0;
        }

        return (int)((ki.chScan << 8) + (ki.chChar));
    }
}

char vm_getchxy(char x, char y)
{
    if (_osmode == DOS_MODE)
    {
        char cell[2];
        vi_init();
        v_getline(cell, (int)(x - 1), (int)(y - 1), 1);
        return *cell;
    }
    else
    {
        char cell[2];
        USHORT len = sizeof cell;
        VioReadCharStr(cell, &len, (USHORT) (y - 1), (USHORT) (x - 1), 0);
        return *cell;
    }
}

char vm_getattrxy(char x, char y)
{
    if (_osmode == DOS_MODE)
    {
        char cell[2];
        vi_init();
        v_getline(cell, (int)(x - 1), (int)(y - 1), 1);
        return *(cell + 1);
    }
    else
    {
        char cell[2];
        USHORT len = sizeof cell;
        VioReadCellStr(cell, &len, (USHORT) (y - 1), (USHORT) (x - 1), 0);
        return *(cell + 1);
    }
}

void vm_xgetchxy(char x, char y, char *attr, char *ch)
{
    if (_osmode == DOS_MODE)
    {
        char cell[2];
        vi_init();
        v_getline(cell, (int)(x - 1), (int)(y - 1), 1);
        *ch = *cell;
        *attr = *(cell + 1);
    }
    else
    {
        char cell[2];
        USHORT len = sizeof cell;
        VioReadCellStr(cell, &len, (USHORT) (y - 1), (USHORT) (x - 1), 0);
        *ch = *cell;
        *attr = *(cell + 1);
    }
}

void vm_setcursorstyle(int style)
{
    if (_osmode == DOS_MODE)
    {
        if (vm_iscolorcard())
        {
            switch (style)
            {
            case CURSORHALF:
                vm_setcursorsize(4, 7);
                break;
            case CURSORFULL:
                vm_setcursorsize(0, 7);
                break;
            case CURSORNORM:
                vm_setcursorsize(7, 8);
                break;
            case CURSORHIDE:
                v_hidecursor();
                break;
            default:
                break;
            }
        }
        else
        {
            switch (style)
            {
            case CURSORHALF:
                vm_setcursorsize(8, 13);
                break;
            case CURSORFULL:
                vm_setcursorsize(0, 13);
                break;
            case CURSORNORM:
                vm_setcursorsize(11, 13);
                break;
            case CURSORHIDE:
                vm_setcursorsize(32, 32);
                break;
            default:
                break;
            }
        }
    }
    else
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
}

void vm_putch(char x, char y, char ch)
{
    if (_osmode == DOS_MODE)
    {
        char cell[2];
        vi_init();
        v_getline(cell, (int)(x - 1), (int)(y - 1), 1);
        *cell = ch;
        v_putline(cell, (int)(x - 1), (int)(y - 1), 1);
    }
    else
    {
        VioWrtCharStr(&ch, 1, (USHORT) (y - 1), (USHORT) (x - 1), 0);
    }
}

void vm_puts(char x, char y, char *str)
{
    if (_osmode == DOS_MODE)
    {
        vi_init();
        while (*str)
        {
            vm_putch(x, y, *str);
            str++;
            x++;
        }
    }
    else
    {
        VioWrtCharStr(str, (USHORT) strlen(str), (USHORT) (y - 1), (USHORT) (x - 1), 0);
    }
}

void vm_xputch(char x, char y, char attr, char ch)
{
    if (_osmode == DOS_MODE)
    {
        char cell[2];
        vi_init();
        *cell = ch;
        *(cell + 1) = attr;
        v_putline(cell, (int)(x - 1), (int)(y - 1), 1);
    }
    else
    {
        VioWrtCharStrAtt(&ch, 1, (USHORT) (y - 1), (USHORT) (x - 1), (PBYTE) &attr, 0);
    }
}

void vm_xputs(char x, char y, char attr, char *str)
{
    if (_osmode == DOS_MODE)
    {
        char *p, *cell, *pcell;
        cell = malloc(strlen(str) * 2);
        if (cell)
        {
            pcell = cell;
            p = str;
            while (*p)
            {
                *pcell++ = *p++;
                *pcell++ = attr;
            }
            vi_init();
            v_putline(cell, (int)(x - 1), (int)(y - 1), strlen(str));
            free(cell);
        }
    }
    else
    {
        VioWrtCharStrAtt(str, (USHORT) strlen(str), (USHORT) (y - 1), (USHORT) (x - 1), (PBYTE) &attr, 0);
    }
}

void vm_putattr(char x, char y, char attr)
{
    if (_osmode == DOS_MODE)
    {
        char cell[2];
        vi_init();
        v_getline(cell, (int)(x - 1), (int)(y - 1), 1);
        *(cell + 1) = attr;
        v_putline(cell, (int)(x - 1), (int)(y - 1), 1);
    }
    else
    {
        VioWrtNAttr((PBYTE) &attr, 1, (USHORT) (y - 1), (USHORT) (x - 1), 0);
    }
}

void vm_paintclearbox(char x1, char y1, char x2, char y2, char attr)
{
    if (_osmode == DOS_MODE)
    {
        char x, y;
        for (y = y1; y <= y2; y++)
        {
            for (x = x1; x <= x2; x++)
            {
                vm_xputch(x, y, attr, ' ');
            }
        }
    }
    else
    {
        char y, cell[2];
        cell[0] = ' ';
        cell[1] = attr;
        for (y = y1; y <= y2; y++)
        {
            VioWrtNCell((PBYTE) &cell, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
        }
    }
}

void vm_paintbox(char x1, char y1, char x2, char y2, char attr)
{
    if (_osmode == DOS_MODE)
    {
        char x, y;
        for (y = y1; y <= y2; y++)
        {
            for (x = x1; x <= x2; x++)
            {
                vm_putattr(x, y, attr);
            }
        }
    }
    else
    {
        char y;
        for (y = y1; y <= y2; y++)
        {
            VioWrtNAttr((PBYTE) &attr, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
        }
    }
}

void vm_clearbox(char x1, char y1, char x2, char y2)
{
    if (_osmode == DOS_MODE)
    {
        char x, y;
        for (y = y1; y <= y2; y++)
        {
            for (x = x1; x <= x2; x++)
            {
                vm_putch(x, y, ' ');
            }
        }
    }
    else
    {
        char y, ch;
        ch = ' ';
        for (y = y1; y <= y2; y++)
        {
            VioWrtNChar((PBYTE) &ch, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
        }
    }
}

void vm_fillbox(char x1, char y1, char x2, char y2, char ch)
{
    if (_osmode == DOS_MODE)
    {
        char x, y;
        for (y = y1; y <= y2; y++)
        {
            for (x = x1; x <= x2; x++)
            {
                vm_putch(x, y, ch);
            }
        }
    }
    else
    {
        char y;
        for (y = y1; y <= y2; y++)
        {
            VioWrtNChar((PBYTE) &ch, (USHORT) (x2 - x1 + 1), (USHORT) (y - 1), (USHORT) (x1 - 1), 0);
        }
    }
}

void vm_gettext(char x1, char y1, char x2, char y2, char *dest)
{
    if (_osmode == DOS_MODE)
    {
        char x, y;
        for (y = y1; y <= y2; y++)
        {
            for (x = x1; x <= x2; x++)
            {
                vm_xgetchxy(x, y, dest + 1, dest);
                dest += 2;
            }
        }
    }
    else
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
}

void vm_puttext(char x1, char y1, char x2, char y2, char *srce)
{
    if (_osmode == DOS_MODE)
    {
        char x, y;
        for (y = y1; y <= y2; y++)
        {
            for (x = x1; x <= x2; x++)
            {
                vm_xputch(x, y, *(srce + 1), *srce);
                srce += 2;
            }
        }
    }
    else
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
}

void vm_horizline(char x1, char x2, char row, char attr, char ch)
{
    if (_osmode == DOS_MODE)
    {
        char x;
        for (x = x1; x <= x2; x++)
        {
            vm_xputch(x, row, attr, ch);
        }
    }
    else
    {
        char cell[2];
        cell[0] = ch;
        cell[1] = attr;
        VioWrtNCell((PBYTE) &cell, (USHORT) (x2 - x1 + 1), (USHORT) (row - 1), (USHORT) (x1 - 1), 0);
    }
}
