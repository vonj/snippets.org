/*----------------------------[ dvideo.c ]-----------------------------*/
/*                     Hardware Interface Routines                     */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* This code is a subset of a library copyrighted by Jeff Dunlop.      */
/* License is hereby granted for unrestricted use.                     */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* dv_info: get video and version info about DesqView                  */
/* get_rows: determine number of rows on screen                        */
/* get_cols: determine number of columns on screen                     */
/* get_vidpage: determine the current text mode video page             */
/* get_vidbase: determine the base of video ram                        */
/* is_dv: determine if DesqView is loaded                              */
/* is_egavga: determine if monitor is ega/vga                          */
/*---------------------------------------------------------------------*/

/*--------------------------------------------------------------*/
/*-----------------------[ include files ]----------------------*/
/*--------------------------------------------------------------*/

#include <dos.h>
#include "dvideo.h"
#include "mk_fp.h"

/*------------------------[ get_vidpage ]-----------------------*/
/* Determine the current text mode video page                   */
/*--------------------------------------------------------------*/
/* local:                                                       */
/*      regs = register storage buffer                          */
/* return:                                                      */
/*      video page number as determined by bios call            */
/*--------------------------------------------------------------*/

unsigned char get_vidpage(void)
{
    union REGS regs;

    regs.h.ah = 0x0f;
    int86(0x10, &regs, &regs);
    return regs.h.bh;
}

/*---------------------------[ is_dv ]--------------------------*/
/*             Determine whether DesqView is active             */
/*--------------------------------------------------------------*/

int is_dv(void)
{
    union REGS regs;

    regs.h.ah = 0x2b;
    regs.x.cx = 0x4445;         /* 'DE' */
    regs.x.dx = 0x5351;         /* 'SQ' */
    regs.h.al = 1;              /* get version */
    int86(0x21, &regs, &regs);

    return regs.h.al != 0xff;
}

/*--------------------------[ dv_info ]-------------------------*/
/*      Return screen and version info about DesqView           */
/*--------------------------------------------------------------*/
/* return:                                                      */
/*      -1 on error                                             */
/*--------------------------------------------------------------*/

int get_dvinfo(DV_INFO *dv_info)
{
    union REGS regs;

    regs.h.ah = 0x2b;
    regs.x.cx = 0x4445;         /* 'DE' */
    regs.x.dx = 0x5351;         /* 'SQ' */
    regs.h.al = 1;              /* get version */
    int86(0x21, &regs, &regs);

    if (regs.h.al == 0xff)
        return -1;
    dv_info->ver_major = regs.h.bh;
    dv_info->ver_minor = regs.h.bl;
    regs.h.al = 4;              /* get screen info */
    int86(0x21, &regs, &regs);

    if (regs.h.al == 0xff)
        return -1;
    dv_info->regen_buf = regs.x.dx;
    dv_info->win_rows = regs.h.bh;
    dv_info->win_cols = regs.h.bl;
    return 0;
}

/*------------------------[ get_vidbase ]-----------------------*/
/* Determine the base of video ram                              */
/*--------------------------------------------------------------*/
/* local:                                                       */
/*      regs = register union for ISR                           */
/* return:                                                      */
/*      the current text base segment                           */
/*--------------------------------------------------------------*/

unsigned get_vidbase(void)
{
    union REGS regs;
    DV_INFO dv_info;

    if (is_dv() && get_dvinfo(&dv_info) != -1)
        return dv_info.regen_buf;
    else
    {
        regs.h.ah = 0xf;
        int86(0x10, &regs, &regs);

        if (regs.h.al == 7)
            return 0xb000;
        else
            return 0xb800;
    }
}

/*-------------------------[ get_rows ]-------------------------*/
/*  Determine the number of rows in current text mode screen    */
/*--------------------------------------------------------------*/

int get_rows(void)
{
    DV_INFO dv_info;
    char far *p = MK_FP(0x40, 0x84);

    if (is_dv() && get_dvinfo(&dv_info) != -1)
        return dv_info.win_rows;
    else
        return *p + is_egavga();
}

/*-------------------------[ get_cols ]-------------------------*/
/*  Determine the number of columns in current text screen      */
/*--------------------------------------------------------------*/

int get_cols(void)
{
    DV_INFO dv_info;
    int far *p = MK_FP(0x40, 0x4a);

    if (is_dv() && get_dvinfo(&dv_info) != -1)
        return dv_info.win_cols;
    else
        return *p;
}

/*-------------------------[ is_egavga ]------------------------*/
/*      Determine whether the current text mode is ega/vga      */
/*--------------------------------------------------------------*/

int is_egavga(void)
{
    union REGS regs;

    regs.h.ah = 0x1a;
    regs.h.al = 0;

    int86(0x10, &regs, &regs);

    return regs.h.al == 0x1a;
}
