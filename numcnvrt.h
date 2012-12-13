/*
**  NUMCNVRT.H - Header file for SNIPPETS numerical <=> string conversions
*/

#ifndef NUMCNVRT__H
#define NUMCNVRT__H

#include <stddef.h>     /* For size_t     */
#include "sniptype.h"
#include "round.h"
#include "pi.h"

#define R_ERROR -2                              /* EVAL.C Range error   */

/*
**  Callable library functions begin here
*/

char  * base_convert(const char *in, char *out,
            size_t len, int rin, int rout);           /* Bascnvrt.C     */
char  * comma_float(double num, char *buf, int dec);  /* Commaflt.C     */
size_t  commafmt(char *buf, int bufsize, long N);     /* Commafmt.C     */
char  * eng(double value, int places);                /* Eng.C          */
int     evaluate(char *line, double *val);            /* Eval.C         */
char  * fmt_money(double amt);                        /* Fmtmoney.C     */
long    hexorint(const char *string);                 /* Hexorint.C     */

char  * ltostr(long num, char *string,
		   size_t max_chars, unsigned base);      /* Ltostr.C       */

char  * ordinal_text(int number);                     /* Ord_Text.C     */
int     scanfrac (const char buf[], double *f);       /* Scanfrac.C     */

unsigned int hstr_i(char *cptr);                      /* Hstr_I.C       */

char *long2roman(long val, char *buf, size_t buflen); /* L2Roman.C      */
long  roman2long(const char *str);                    /* Roman2L.C      */


#if defined(__ZTC__) && !defined(__SC__)
 char * ltoa(long val, char *buf, int base);          /* Ltoa.C         */
#endif


/*
**  File: STR27SEG.C
*/

struct Seg7disp {
      unsigned seg_a : 1;
      unsigned seg_b : 1;
      unsigned seg_c : 1;
      unsigned seg_d : 1;
      unsigned seg_e : 1;
      unsigned seg_f : 1;
      unsigned seg_g : 1;
};

char *str27seg(char *string);


#endif /* NUMCNVRT__H */
