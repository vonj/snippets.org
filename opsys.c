/*
 *  OPSYS.C  Operating environment detection and time-slicing support for
 *           MS-DOS applications.
 *
 *  Written in June 1996 by Andrew Clarke and released to the public domain.
 */

#include <dos.h>
#include "opsys.h"

int opsys_id = OS_MSDOS;

int opsysDetect(void)
{
#if defined(__TURBOC__)
    /* detect DESQview */
    _AX = 0x2B01;
    _CX = 0x4445;
    _DX = 0x5351;
    geninterrupt(0x21);
    if (_AL != 0xFF)
    {
        opsys_id = OS_DESQVIEW;
        return opsys_id;
    }

    /* detect OS/2 */
    _AH = 0x30;
    geninterrupt(0x21);
    if (_AL >= 10)
    {
        opsys_id = OS_OS2;
        return opsys_id;
    }

    /* detect Windows */
    _AX = 0x160A;
    geninterrupt(0x2F);
    if (_AX == 0)
    {
        opsys_id = OS_WINDOWS;
        return opsys_id;
    }
#else
    union REGS regs;

    /* detect DESQview */
#if defined(__WATCOMC__) && defined(__386__)
    regs.w.ax = 0x2B01;
    regs.w.cx = 0x4445;
    regs.w.dx = 0x5351;
    int386(0x21, &regs, &regs);
#else
    regs.x.ax = 0x2B01;
    regs.x.cx = 0x4445;
    regs.x.dx = 0x5351;
#ifdef __EMX__
    _int86(0x21, &regs, &regs);
#else
    int86(0x21, &regs, &regs);
#endif
#endif
    if (regs.h.al != 0xFF)
    {
        opsys_id = OS_DESQVIEW;
        return opsys_id;
    }

    /* detect OS/2 */
    regs.h.ah = 0x30;
#if defined(__WATCOMC__) && defined(__386__)
    int386(0x21, &regs, &regs);
#else
#ifdef __EMX__
    _int86(0x21, &regs, &regs);
#else
    int86(0x21, &regs, &regs);
#endif
#endif
    if (regs.h.al >= 10)
    {
        opsys_id = OS_OS2;
        return opsys_id;
    }

    /* detect Windows */
#if defined(__WATCOMC__) && defined(__386__)
    regs.w.ax = 0x160A;
    int386(0x2F, &regs, &regs);
    if (regs.w.ax == 0)
    {
        opsys_id = OS_WINDOWS;
        return opsys_id;
    }
#else
    regs.x.ax = 0x160A;
#ifdef __EMX__
    _int86(0x2F, &regs, &regs);
#else
    int86(0x2F, &regs, &regs);
#endif
    if (regs.x.ax == 0)
    {
        opsys_id = OS_WINDOWS;
        return opsys_id;
    }
#endif
#endif

    /* must be MS-DOS */
    opsys_id = OS_MSDOS;
    return opsys_id;
}

void opsysTimeSlice(void)
{
#if defined(__TURBOC__)
    switch (opsys_id)
    {
    case OS_MSDOS:
        geninterrupt(0x28);
        break;
    case OS_OS2:
    case OS_WINDOWS:
        _AX = 0x1680;
        geninterrupt(0x2F);
        break;
    case OS_DESQVIEW:
        _AX = 0x1000;
        geninterrupt(0x15);
        break;
    case OS_DBLDOS:
        _AX = 0xEE01;
        geninterrupt(0x21);
        break;
    case OS_NETWARE:
        _BX = 0x000A;
        geninterrupt(0x7A);
        break;
    default:
        break;
    }
#else
    union REGS regs;
    switch (opsys_id)
    {
    case OS_MSDOS:
#if defined(__WATCOMC__) && defined(__386__)
        int386(0x28, &regs, &regs);
#else
#ifdef __EMX__
        _int86(0x28, &regs, &regs);
#else
        int86(0x28, &regs, &regs);
#endif
#endif
        break;

    case OS_OS2:
    case OS_WINDOWS:
#if defined(__WATCOMC__) && defined(__386__)
        regs.w.ax = 0x1680;
        int386(0x2F, &regs, &regs);
#else
        regs.x.ax = 0x1680;
#ifdef __EMX__
        _int86(0x2F, &regs, &regs);
#else
        int86(0x2F, &regs, &regs);
#endif
#endif
        break;

    case OS_DESQVIEW:
#if defined(__WATCOMC__) && defined(__386__)
        regs.w.ax = 0x1000;
        int386(0x15, &regs, &regs);
#else
        regs.x.ax = 0x1000;
#ifdef __EMX__
        _int86(0x15, &regs, &regs);
#else
        int86(0x15, &regs, &regs);
#endif
#endif
        break;

    case OS_DBLDOS:
#if defined(__WATCOMC__) && defined(__386__)
        regs.w.ax = 0xEE01;
        int386(0x21, &regs, &regs);
#else
        regs.x.ax = 0xEE01;
#ifdef __EMX__
        _int86(0x21, &regs, &regs);
#else
        int86(0x21, &regs, &regs);
#endif
#endif
        break;

    case OS_NETWARE:
#if defined(__WATCOMC__) && defined(__386__)
        regs.w.bx = 0x000A;
        int386(0x7A, &regs, &regs);
#else
        regs.x.bx = 0x000A;
#ifdef __EMX__
        _int86(0x7A, &regs, &regs);
#else
        int86(0x7A, &regs, &regs);
#endif
#endif
        break;

    default:
        break;
    }
#endif
}

#ifndef __EMX__

unsigned short opsysGetVideoSeg(unsigned short assumed_base)
{
    if (opsys_id == OS_DESQVIEW)
    {
#if defined(__TURBOC__)
        _ES = assumed_base;
        _DI = 0;
        _AX = 0xFE00;
        geninterrupt(0x10);
        return _ES;
#else
        union REGS regs;
        struct SREGS sregs;
        sregs.es = assumed_base;
#if defined(__WATCOMC__) && defined(__386__)
        regs.w.di = 0;
        regs.w.ax = 0xFE00;
        int386x(0x10, &regs, &regs, &sregs);
#else
        regs.x.di = 0;
        regs.x.ax = 0xFE00;
        int86x(0x10, &regs, &regs, &sregs);
#endif
        return sregs.es;
#endif
    }
    return assumed_base;
}

#endif
