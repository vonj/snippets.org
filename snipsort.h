/*
**  Header file for SNIPPETS sorting functions
*/

#ifndef SNIPSORT__H
#define SNIPSORT__H

#include <stddef.h>
#include "dirport.h"

/*
**  Prototypes
*/

#ifdef __STDC__
 #define strsort _strsort
#endif

void hugesort(void HUGE *basep, unsigned  nel,
      unsigned width,
      int (*comp)(void HUGE *, void HUGE *));         /* Hugesort.C     */
void    *sortl(void *list, void *(*getnext)(void *),
            void (*setnext)(void *, void *),
            int (*compare)(void *, void *));          /* Ll_Qsort.C     */
void isort(void *base, size_t nmemb, size_t size,
      int (*comp)(const void *, const void *));       /* Rg_Isort.C     */
void qsort(void *, size_t, size_t,
      int (*)(const void *, const void *));           /* Rg_Qsort.C     */
void swap_chars(char *, char *, size_t);              /* Rg_Qsort.C     */
void quicksort(int v[], unsigned n);                  /* Rgiqsort.C     */
void ssort (void *base, size_t nel, size_t width,
      int (*comp)(const void *, const void *));       /* Rg_Ssort.C     */
void strsort(char **v, unsigned n);                   /* Strsort.C      */

/*
**  File: LL_MSORT.C
*/

typedef struct list_struct {
         struct list_struct *next;
         char *key;
         /* other stuff */
         } list;

list *lsort (list *p);

/*
**  File: BINSRCH.C
*/

int BinSearch(double key, double *r, size_t n);

#endif /* SNIPSORT__H */
