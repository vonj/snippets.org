/*
**  Macros for managing direct video writes by Jerry Houston
**
**  prototypes for SCROLL.C and VIDPORT.C functions added by Bob Stout
*/

#ifndef SCRNMACS__H
#define SCRNMACS__H

#include "sniptype.h"
#include "extkword.h"

#if !defined(COLORMODE)
 #define COLORMODE  ((*(char FAR *)0x0449L) != 7)
 #define EXT_KBD    (*(char FAR *)0x0496L & 16)
 #define VIDPAGE    (*((unsigned char far *)0x0462L))
 #define ROWSIZE    (*(int FAR *)0x044AL)
 #define SCANLINES  ((int)*(char FAR*)0x0461L)
 #define SCRBUFF    ((unsigned FAR *)((COLORMODE)?0xB8000000L:0xB0000000L))
 #define SCREENSEG  ((unsigned)((COLORMODE)?0xB800:0xB000))
 #define SCRNBYTES  (*(int FAR *)0x44CL)
 #define SCRNPIXELS (SCRNBYTES >> 1)
 #define SCREENCOLS (*(int FAR *)0x044AL)
 #define SCREENROWS ((*(char FAR *)0x0484L) ? 1 + (*(char FAR *)0x0484L): 25)
#endif

/*
     COLORMODE  = true/false, are we using color?
     EXT_KBD    = true/false, extended keyboard in use?
     VIDPAGE    = current video page in use
     SCANLINES  = number of scan lines in a character.
     SCRBUFF    = returns B800:0000 if using color, B000:0000 if mono.
     SCREENSEG  = when you just need the segment portion.
     SCRNBYTES  = number of bytes required to save screen.
     SCRNPIXELS = number of (2-byte) pixels required to save screen.
     SCREENCOLS = number of columns, often 80.
     SCREENROWS = number of rows, usually defaults to 25.
*/

/*
** video attributes
*/

#define BLINKING 0x87
#define REVERSE 0x70
#define REVBLINK 0xf0
#define NORMAL 0x07
#define HIGHLITE 0x0f
#define HIGHBLINK 0x8f
#define BLINKBIT 0x80   /* OR in to cause blink */
#define HILTBIT 0x08    /* OR in to cause highlight */

/*
** colors -- Use as is for foreground colors
**           For background, shift left by 4 and OR with
**           foreground and possible video attributes
*/

#define BLACK_ 0
#define BLUE_ 1
#define GREEN_ 2
#define CYAN_ 3
#define RED_ 4
#define MAGENTA_ 5
#define BROWN_ 6
#define WHITE_ 7
#define GRAY_ 8
#define LTBLUE_ 9
#define LTGREEN_ 10
#define LTCYAN_ 11
#define LTRED_ 12
#define LTMAGENTA_ 13
#define YELLOW_ 14
#define HIWHITE_ 15     /* hi-intensity white */

#define BG_(a) (((a) & 0x7f) << 4)

/*
** e.g. blue background with yellow characters
**
** video_attribute = BG_(BLUE_)+YELLOW_
*/ 

/*
**  File: SCROLL.C
*/

#define SCROLL_UP 0
#define SCROLL_DN 1

void scroll(int direction,
            int num_lines,
            int vattrib,
            int ulrow,
            int ulcomumn,
            int lrrow,
            int lrcolumn);

/*
**  File: VIDPORT.C
*/

void GotoXY(int col, int row);
void ClrScrn(int vattrib);
void GetCurPos(int *col, int *row);
int  GetCurAtr(void);
void ClrEol(void);
void ClrEop(void);
void Repaint(int vattrib);


/*
**  File: SCNRSAVE.C
*/

struct SCREEN {
      unsigned short *vbuf;
      int             curX,
                      curY;
};

struct SCREEN *SaveScrn(void);
void RestoreScrn(struct SCREEN *screen);
void FreeScrnBuf(struct SCREEN *screen);


/*
**  File: FSCRNSAVE.C
*/

Boolean_T fSaveScrn(const char *fname);
Boolean_T fRestoreScrn(const char *fname);

/*
**  File: ATR2ANSI.C
*/

char *make_ansi(int vatr);

#endif /* SCRNMACS__H */
