/*
 *  VMGRDJGP.C; VidMgr module for the DJGPP GNU C/C++ compiler.  Release 1.2.
 *
 *  This module written in May 1996 by Andrew Clarke and released to the
 *  public domain.
 */

#include <dos.h>
#include <go32.h>
#include <sys/farptr.h>
#include "vidmgr.h"
#include "opsys.h"

static int vm_iscolorcard(void);
static char vm_getscreenmode(void);
static void vm_setscreenmode(char mode);
static void vm_setcursorsize(char start, char end);
static void vm_getcursorsize(char *start, char *end);
static void vm_getkey(unsigned char *chScan, unsigned char *chChar);
static unsigned long vm_screenaddress(char x, char y);

void vm_init(void)
{
    vm_getinfo(&vm_startup);
    vm_setattr(vm_startup.attr);
    opsysDetect();
}

void vm_done(void)
{
    if (vm_getscreenmode() != vm_startup.mode)
    {
        vm_setscreenmode(vm_startup.mode);
    }
    vm_setcursorsize(vm_startup.cur_start, vm_startup.cur_end);
}

void vm_getinfo(struct vm_info *v)
{
    v->ypos = vm_wherey();
    v->xpos = vm_wherex();
    v->attr = vm_getattrxy(v->xpos, v->ypos);
    v->mode = vm_getscreenmode();
    v->height = vm_getscreenheight();
    v->width = vm_getscreenwidth();
    vm_getcursorsize(&v->cur_start, &v->cur_end);
}

static int vm_iscolorcard(void)
{
    return vm_getscreenmode() != 7;
}

static char vm_getscreenmode(void)
{
    return (char)_farpeekb(_go32_conventional_mem_selector(), 0x0449);
}

static void vm_setscreenmode(char mode)
{
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}

char vm_getscreenwidth(void)
{
    return (char)_farpeekb(_go32_conventional_mem_selector(), 0x044a);
}

char vm_getscreenheight(void)
{
    return (char)(_farpeekb(_go32_conventional_mem_selector(), 0x0484) + 1);
}

short vm_getscreensize(void)
{
    return (short)_farpeekw(_go32_conventional_mem_selector(), 0x044c);
}

void vm_gotoxy(char x, char y)
{
    union REGS regs;
    regs.h.ah = 0x02;
    regs.h.bh = 0;
    regs.h.dh = (unsigned char)(y - 1);
    regs.h.dl = (unsigned char)(x - 1);
    int86(0x10, &regs, &regs);
}

char vm_wherex(void)
{
    return (char)(_farpeekb(_go32_conventional_mem_selector(), 0x0450) + 1);
}

char vm_wherey(void)
{
    return (char)(_farpeekb(_go32_conventional_mem_selector(), 0x0451) + 1);
}

static void vm_setcursorsize(char start, char end)
{
    union REGS regs;
    regs.h.ah = 0x01;
    regs.h.ch = start;
    regs.h.cl = end;
    int86(0x10, &regs, &regs);
}

static void vm_getcursorsize(char *start, char *end)
{
    union REGS regs;
    regs.h.ah = 0x03;
    regs.h.bh = 0;
    int86(0x10, &regs, &regs);
    *start = regs.h.ch;
    *end = regs.h.cl;
}

int vm_kbhit(void)
{
    union REGS regs;
    static unsigned short counter = 0;
    if (counter % 10 == 0)
    {
        opsysTimeSlice();
    }
    counter++;
    regs.h.ah = 0x01;
    int86(0x16, &regs, &regs);
    return !(regs.x.flags & 0x40);
}

static void vm_getkey(unsigned char *chScan, unsigned char *chChar)
{
    union REGS regs;
    regs.h.ah = 0x00;
    int86(0x16, &regs, &regs);
    *chScan = regs.h.ah;
    *chChar = regs.h.al;
}

int vm_getch(void)
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
            chChar = 0;         /* force scan return */
        }
        else
        {                       /* get next block */
            chChar = 0;

            vm_getkey(&chScan, &chChar);
            if (!chScan)
            {                   /* still no scan? */
                chScan = chChar;  /* move new char over */
                chChar = 0;     /* force its return */
            }
            else
            {
                chChar = 0;     /* force new scan */
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
            {                   /* still no scan? */
                chScan = chChar;  /* move new char over */
                chChar = 0;     /* force its return */
            }
            else
            {
                chChar = 0;     /* force new scan */
            }
        }
        else
        {
            chScan = 0;         /* handle 0xe00d case */
        }
    }
    if (chChar)
    {
        chScan = 0;
    }

    return (int)((chScan << 8) + (chChar));
}

void vm_setcursorstyle(int style)
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
            vm_setcursorsize(32, 32);
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

static unsigned long vm_screenaddress(char x, char y)
{
    unsigned short base;
    if (vm_iscolorcard())
    {
        base = opsysGetVideoSeg(0xB800);
    }
    else
    {
        base = opsysGetVideoSeg(0xB000);
    }
    return (unsigned long)((base << 4) + ((y - 1) * vm_getscreenwidth() * 2) + ((x - 1) * 2));
}

char vm_getchxy(char x, char y)
{
    unsigned long address;
    address = vm_screenaddress(x, y);
    return (char)_farpeekb(_go32_conventional_mem_selector(), address);
}

char vm_getattrxy(char x, char y)
{
    unsigned long address;
    address = vm_screenaddress(x, y);
    return (char)_farpeekb(_go32_conventional_mem_selector(), address + 1L);
}

void vm_xgetchxy(char x, char y, char *attr, char *ch)
{
    unsigned long address;
    address = vm_screenaddress(x, y);
    *ch = (char)_farpeekb(_go32_conventional_mem_selector(), address);
    *attr = (char)_farpeekb(_go32_conventional_mem_selector(), address + 1L);
}

void vm_putch(char x, char y, char ch)
{
    unsigned long address;
    address = vm_screenaddress(x, y);
    _farpokeb(_go32_conventional_mem_selector(), address, ch);
}

void vm_puts(char x, char y, char *str)
{
    char ofs;
    ofs = 0;
    while (*str)
    {
        unsigned long address;
        address = vm_screenaddress(x + ofs, y);
        _farpokeb(_go32_conventional_mem_selector(), address, *str);
        str++;
        ofs++;
    }
}

void vm_xputch(char x, char y, char attr, char ch)
{
    unsigned long address;
    address = vm_screenaddress(x, y);
    _farpokeb(_go32_conventional_mem_selector(), address, ch);
    _farpokeb(_go32_conventional_mem_selector(), address + 1L, attr);
}

void vm_xputs(char x, char y, char attr, char *str)
{
    char ofs;
    ofs = 0;
    while (*str)
    {
        unsigned long address;
        address = vm_screenaddress(x + ofs, y);
        _farnspokeb(address, *str);
        _farnspokeb(address + 1L, attr);
        str++;
        ofs++;
    }
}

void vm_putattr(char x, char y, char attr)
{
    unsigned long address;
    address = vm_screenaddress(x, y);
    _farpokeb(_go32_conventional_mem_selector(), address + 1L, attr);
}

void vm_paintclearbox(char x1, char y1, char x2, char y2, char attr)
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

void vm_paintbox(char x1, char y1, char x2, char y2, char attr)
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

void vm_clearbox(char x1, char y1, char x2, char y2)
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

void vm_fillbox(char x1, char y1, char x2, char y2, char ch)
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

void vm_gettext(char x1, char y1, char x2, char y2, char *dest)
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

void vm_puttext(char x1, char y1, char x2, char y2, char *srce)
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

void vm_horizline(char x1, char x2, char row, char attr, char ch)
{
    char x;
    for (x = x1; x <= x2; x++)
    {
        vm_xputch(x, row, attr, ch);
    }
}
