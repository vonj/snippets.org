/*
**  DOANSI.H - Portable ANSI screen code interpreter
**
**  From DRSK_105.LZH (ansi.c), hereby declared free for use for whatever
**  purposes by author: Mark Kimes
*/

#ifndef DOANSI__H
#define DOANSI__H

extern char curattr;
extern int  curx, cury;
extern int  maxx, maxy;
extern int  realmaxy,realmaxx;
extern char useansi;
extern int  tabspaces;

/* set maxx,maxy as desired */
void set_screensize (int reservedlines);

/* put character c at x,y using attr as attribute */
void put_char (char c,char attr,int x,int y);

/* position hardware cursor at x,y */
void pos_hardcursor (int x,int y);

/* turn hardware cursor off */
void hardcursor_off (void);

/* turn hardware cursor on at x,y */
void hardcursor_on (int x,int y);

/* scroll window tx,ty - bx,by up one line; fill with blank+attr */
void scroll_up (int tx,int ty,int bx,int by,char attr);

/* clear the window from tx,ty - bx,by; fill with blank+attr */
void clearwindow (int tx,int ty,int bx,int by,char attr);

/* clear line y from col x to eol (ex); fill with blank+attr */
void cleartoeol (int x,int y,int ex,char attr);

/* the ansi string interpreter */
int ansi_out (char *buf);

#endif /* DOANSI__H */
