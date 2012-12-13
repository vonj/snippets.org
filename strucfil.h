/*
**  SNIPPETS header file for STRUCFIL.C
*/

#ifndef STRUCFIL__H
#define STRUCFIL__H

#include <stdio.h>

/* make sure the record structure is byte aligned */

#if defined(_MSC_VER) || defined(_QC) || defined(__WATCOMC__)
 #pragma pack(1)
#elif defined(__ZTC__)
 #pragma ZTC align 1
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
 #pragma option -a-
#endif

static struct blackbook {
    int delete_flag;        /* 0 = active  -1 = deleted */
    int recordnum;          /* a sequential number in the file */
   /* The data fields in asciiz.  */
    char firstname[11];
    char lastname[16];
    char addr[26];
    char city[16];
    char state[3];
    char zip[10];
    char phone[11];
} rec, oldrec;             /* 97 byte record  * 2 */

/* set structure alignment to default */

#if defined (_MSC_VER) || defined(_QC) || defined(__WATCOMC__)
 #pragma pack()
#elif defined (__ZTC__)
 #pragma ZTC align
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
 #pragma option -a.
#endif

FILE * open_file(char *filename);
int    datadd(void);
int    data_delete(void);
int    data_read(long recnum);
int    data_update(void);
int    read_forward(void);
int    read_backward(void);

#endif /* STRUCFIL__H */
