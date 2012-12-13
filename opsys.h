/*
 *  OPSYS.H  Operating environment detection and time-slicing support for
 *           MS-DOS applications.
 *
 *  Written in June 1996 by Andrew Clarke and released to the public domain.
 */

#ifndef __OPSYS_H__
#define __OPSYS_H__

#define OS_MSDOS     0  /* MS-DOS */
#define OS_OS2       1  /* IBM OS/2 */
#define OS_WINDOWS   2  /* Microsoft Windows */
#define OS_DESQVIEW  3  /* Quarterdeck DESQview */
#define OS_DBLDOS    4  /* DoubleDOS */
#define OS_NETWARE   5  /* Novell Netware */

extern int opsys_id;

int opsysDetect(void);
void opsysTimeSlice(void);
#ifndef __EMX__
unsigned short opsysGetVideoSeg(unsigned short assumed_base);
#endif

#endif
