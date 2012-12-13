/*
**    SNIPKBIO.H - Snippets header file for keyboard I/O function
*/

#ifndef SNIPKBIO__H
#define SNIPKBIO__H

#include <dos.h>
#include <conio.h>
#include "sniptype.h"
#include "pchwio.h"

Boolean_T getYN(char *prompt, int def_ch,             /* Getyn.C        */
                unsigned timeout);

int  ungetkey(unsigned short key);                    /* Kb_Stuff.C     */
int  KB_stuff(char *str);                             /* Kb_Stuff.C     */

int  IsLeftShift(void);                               /* Isshift.C      */
int  IsRightShift(void);                              /* Isshift.C      */
int  IsShift(void);                                   /* Isshift.C      */

int  IsLeftAlt(void);                                 /* Isshift.C      */
int  IsRightAlt(void);                                /* Isshift.C      */
int  IsAlt(void);                                     /* Isshift.C      */

int  IsLeftCtl(void);                                 /* Isshift.C      */
int  IsRightCtl(void);                                /* Isshift.C      */
int  IsCtl(void);                                     /* Isshift.C      */

int  IsSysRq(void);                                   /* Isshift.C      */
int  timed_getch(int n_seconds);                      /* Timegetc.C     */
int  isxkeybrd(void);                                 /* Isxkbrd.C      */
void setcaps(void);                                   /* Keylocks.C     */
void clrcaps(void);                                   /* Keylocks.C     */
void setnumlock(void);                                /* Keylocks.C     */
void clrnumlock(void);                                /* Keylocks.C     */

#define     RIGHT_SHIFT       0x0001
#define     LEFT_SHIFT        0x0002

#define     EITHER_ALT        0x0008
#define     LEFT_ALT          0x0200

#define     EITHER_CTL        0x0004
#define     LEFT_CTL          0x0100

#if defined(__OS2__)
 #define    SYSRQ             0x8000
#else /* assume DOS */
 #define    SYSRQ             0x0400
#endif

#ifdef __OS2__
 #define INCL_NOPM
 #define INCL_KBD
 #define INCL_DOSPROCESS    /* for DosSleep() */
 #include <os2.h>

KBDINFO setkbmode(void);    /* Change keyboard to binary mode */
void restkbmode(KBDINFO);   /* restore keyboard mode */
                            /* both defined in EXT_KEYS.C */

/* 30-Mar-96 - EBB:
** OS/2 doesn't have a place in memory where information about the last
** key is held, like DOS does.      All the information about a keystroke is
** wrapped up into a structure (KBDKEYINFO) returned from OS/2's
** get-a-key API call, KbdCharIn(), so if you want to query that information
** at some arbitrary later time, you have to save it somewhere.  I have
** chosen to use a global variable defined in ISSHIFT.C to do this.  The
** functions in ISSHIFT.C report the status of the last key stored in that
** global, which may not be the last key pressed in your program if you're
** also storing keystrokes elsewhere.
*/
extern KBDKEYINFO ki;         /* Holds key info - defined in ISSHIFT.C */

 #define peekkey()      (&ki.fsState)
#else /* !__OS2__ */
 #define key_seg  0x40
 #define key_off  0x17
 #define peekkey()      ((unsigned short FAR*) MK_FP(key_seg, key_off))
#endif

#endif /* SNIPKBIO__H */
