/*
 * CSplit.h
 * 08/31/93  Fred Cole     original
 */

#ifndef  __STDC__
 #include "dirport.h"
#endif

#define VERSION   "2.2"

#ifndef B_FALSE
#define B_FALSE 0
#endif

#ifndef B_TRUE
#define B_TRUE  1
#endif

enum { NOERR = 0, SYNTAX, ABORT, MEMORY, FILEIO, PROCESS };

#define FNAMELEN  12    /* 8.3 filename character count     */
#define LLENMAX  128    /* maximum input source line length */
#define PATHMAX   80    /* maximum path length              */
#define MAXFSPEC (PATHMAX + FNAMELEN)

#define LENMIN    25    /* minimum source lines per section */
#define LENDEF    90    /* default source lines per section */ /* TS */
#define LENMAX SHRT_MAX /* maximum source lines per section */
#define TABMIN     2    /* minimum spaces per tab char      */
#define TABDEF     4    /* default spaces per tab char      */
#define TABMAX    16    /* maximum spaces per tab char      */
#define WIDMIN    55    /* minimum source chars per line    */
#define WIDDEF    75    /* default source chars per line    */
#define WIDMAX   255    /* maximum source chars per line    */

#define SEP_CLEN 10     /* separator character string length */
#define SEP_CDEF '>'    /* default separator character */
#define SEP_BF   "Begin file "
#define SEP_BP   "Begin part "
#define SEP_EF   "End file "
#define SEP_EP   "End part "
#define SEP_ID   "CSplit: "
#define SEP_VR   "Version "

typedef struct _slst
{
  char srcfile[MAXFSPEC + 1];     /* path and filename */
  struct _slst *next;
} SLST;

SLST *add_list  (char *);
void  cleanup   (void);
char *csp_fgets (char *, int, FILE *, int);
void  disp_help (void);
int   extr_file (char *, char);
void  free_list (void);
int   init_list (int, char **, int);
int   split_src (SLST *, char *, int, int, int, char);
unsigned short updcrc (unsigned short, unsigned char *, unsigned int);
void  initcrctab (void);
