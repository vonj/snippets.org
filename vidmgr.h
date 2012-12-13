/*
 *  VIDMGR.H; Screen drawing, cursor and keyboard routines for text mode
 *            16-bit and 32-bit MS-DOS, 16-bit and 32-bit OS/2, and 32-bit
 *            Windows 95/NT applications.  Release 1.2.
 *
 *  This module written in March 1996 by Andrew Clarke and released to the
 *  public domain.  Last modified in June 1996.
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

#ifndef __VIDMGR_H__
#define __VIDMGR_H__

#ifndef DOS
#if defined(_QC) || defined(__DOS__) || defined(MSDOS) || defined(__MSDOS__)
#define DOS
#endif
#endif

#ifndef OS2
#if defined(__OS2__) || defined(OS_2)
#define OS2
#endif
#endif

#ifndef EMX
#if defined(__EMX__)
#define EMX
#endif
#endif

#ifndef WINNT
#if defined(__NT__)
#define WINNT
#endif
#endif

#define BLACK         0x00
#define BLUE          0x01
#define GREEN         0x02
#define CYAN          0x03
#define RED           0x04
#define MAGENTA       0x05
#define PURPLE        MAGENTA
#define BROWN         0x06
#define LIGHTGRAY     0x07
#define LIGHTGREY     LIGHTGRAY
#define DARKGRAY      0x08
#define DARKGREY      DARKGRAY
#define LIGHTBLUE     0x09
#define LIGHTGREEN    0x0a
#define LIGHTCYAN     0x0b
#define LIGHTRED      0x0c
#define LIGHTMAGENTA  0x0d
#define LIGHTPURPLE   LIGHTMAGENTA
#define YELLOW        0x0e
#define WHITE         0x0f
#define BLINK         0x80

#define CURSORHIDE    0
#define CURSORNORM    1
#define CURSORHALF    2
#define CURSORFULL    3

#define vm_mkcolor(fore, back)  ((fore) | (back << 4))
#define vm_fore(attr)           (attr % 16)
#define vm_back(attr)           (attr / 16)
#define vm_mkcolour             vm_mkcolor
#define vm_attrib               vm_paintbox

struct vm_info
{
    char attr;                  /* text attribute */
#if defined(DOS)
    char mode;                  /* video mode */
#endif
    char height;                /* screen height */
    char width;                 /* screen width */
    char ypos;                  /* y-coordinate */
    char xpos;                  /* x-coordinate */
#if defined(WINNT)
    unsigned short dwSize;      /* 0=minimal; 49=half; 99=full */
    int bVisible;               /* 0=cursor invisible */
#else
    char cur_start;             /* cursor start line */
    char cur_end;               /* cursor end line */
#endif
};

extern struct vm_info vm_startup;
extern char vm_curattr;

extern char vm_frame_blank[];
extern char vm_frame_single[];
extern char vm_frame_double[];

void vm_init(void);
void vm_done(void);
char vm_getscreenwidth(void);
char vm_getscreenheight(void);
short vm_getscreensize(void);
void vm_gotoxy(char x, char y);
char vm_wherex(void);
char vm_wherey(void);
int vm_kbhit(void);
int vm_getch(void);
void vm_paintclearbox(char x1, char y1, char x2, char y2, char attr);
void vm_paintclearscreen(char attr);
void vm_paintclearline(char row, char attr);
void vm_paintcleareol(char row, char attr);
void vm_paintbox(char x1, char y1, char x2, char y2, char attr);
void vm_paintscreen(char attr);
void vm_paintline(char row, char attr);
void vm_painteol(char row, char attr);
void vm_clearbox(char x1, char y1, char x2, char y2);
void vm_clearscreen(void);
void vm_clearline(char row);
void vm_cleareol(char row);
void vm_fillbox(char x1, char y1, char x2, char y2, char ch);
void vm_fillscreen(char ch);
void vm_fillline(char row, char ch);
void vm_filleol(char row, char ch);
void vm_clrscr(void);
void vm_clreol(void);
char vm_getchxy(char x, char y);
char vm_getattrxy(char x, char y);
void vm_xgetchxy(char x, char y, char *attr, char *ch);
void vm_setattr(char attr);
void vm_putattr(char x, char y, char attr);
void vm_setcursorstyle(int style);
void vm_putch(char x, char y, char ch);
void vm_puts(char x, char y, char *str);
void vm_printf(char x, char y, const char *format,...);
void vm_printfcenter(char row, const char *format,...);
void vm_printfbetween(char x1, char x2, char row, const char *format,...);
void vm_xputch(char x, char y, char attr, char ch);
void vm_xputs(char x, char y, char attr, char *str);
void vm_xprintf(char x, char y, char attr, const char *format,...);
void vm_xprintfcenter(char row, char attr, const char *format,...);
void vm_xprintfbetween(char x1, char x2, char row, char attr, const char *format,...);
void vm_horizline(char x1, char x2, char row, char attr, char ch);
void vm_vertline(char y1, char y2, char col, char attr, char ch);
void vm_frame(char x1, char y1, char x2, char y2, char attr, char *frame);
void vm_gettext(char x1, char y1, char x2, char y2, char *dest);
void vm_puttext(char x1, char y1, char x2, char y2, char *srce);
void vm_getinfo(struct vm_info *v);

#endif  /* __VIDMGR_H__ */
