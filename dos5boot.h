/*
**  DOS5BOOT.H - DOS 5 boot record
*/

#ifndef DOS5BOOT__H
#define DOS5BOOT__H

#include "extkword.h"

#if defined(__TURBOC__)
 #pragma option -a-
#elif defined(__ZTC__)
 #pragma ZTC align 1
#else /* MSC/QC/WATCOM/METAWARE */
 #pragma pack(1)
#endif

typedef struct {                                /* offset in buffer record */
        char      bsJump[3];                    /* 1 - 3                   */
        char      bsOemName[8];                 /* 4 - 11                  */
        short     bsBytesPerSec;                /* 12 - 13                 */
        char      bsSecPerClust;                /* 14                      */
        short     bsResSectors;                 /* 15 - 16                 */
        char      bsFATs;                       /* 17                      */
        short     bsRootDirEnts;                /* 18 - 19                 */
        short     bsSectors;                    /* 20 - 21                 */
        char      bsMedia;                      /* 22                      */
        short     bsFATsecs;                    /* 23 - 24                 */
        short     bsSecPerTrack;                /* 25 - 26                 */
        short     bsHeads;                      /* 27 - 28                 */
        long      bsHiddenSecs;                 /* 29 - 32                 */
        long      bsHugeSectors;                /* 33 - 36                 */
        char      bsDriveNumber;                /* 37                      */
        char      bsReserved1;                  /* 38                      */
        char      bsBootSignature;              /* 39                      */
        long      bsVolumeID;                   /* 40 - 43                 */
        char      bsVolumeLabel[11];            /* 44 - 54                 */
        char      bsFileSysType[8];             /* 54 - 61                 */
        char      bsReserved2[8];               /* 62 - 69                 */
        char      bsJunk[442];                  /* 70 - end of record
                                                   (byte 512 is last)      */
        } B_REC;                    /* Boot_record; total of 512 bytes     */

#endif /* DOS5BOOT__H */
