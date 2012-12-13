/*
**  UCLOCK.H
**
**  Functions to perform microsecond accuracy timing operations
**  and calculations.
**
**  Adapted from free sources originally by:
**  Kris Heidenstrom (kheidens@actrix.gen.nz)
**
**  For additional information, ftp:
**  oak.oakland.edu//SimTel/msdos/info/pctim*.zip.
**
**  Modified by Bob Stout.
*/

#ifndef UCLOCK__H
#define UCLOCK__H

#include <dos.h>
#include <time.h>
#include "extkword.h"

/* Declarations and prototypes                                          */

typedef struct {                    /* Only 20, of 24, bits are used    */
      unsigned short count;
      unsigned long  ticks;
      } Uclock_T;

typedef double  uclock_t;

/* Ticks per second - replaces CLK_TCK                                  */
#define UCLK_TCK 1193181.667

/* uclock_t Midnight rollover value                                     */
#define UCLK_RLVR (uclock_t)(0x001800af * 65536.0 + 0xffff)


/* Functions in UCLOCK_.ASM                                             */

void CDECL Uclock_(Uclock_T *);
void CDECL SetMode2_(void);

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

uclock_t    usec_clock(Uclock_T *now);
uclock_t    uclock_diff(Uclock_T *Start, Uclock_T *Stop, Uclock_T *Diff);
uclock_t    uclock_cnvrt(Uclock_T *ticks);
void        usec_delay(unsigned long usecs);

#if defined(__cplusplus) && __cplusplus
 }
#endif

#endif /* UCLOCK__H */
