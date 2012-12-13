/*
** stk.h - Stack manager (stk.c) header file.
**
** By Dustin Puryear (dpuryear@intersurf.com), placed into Public Domain.
*/

#ifndef STK__H
#define STK__H

typedef struct stknode
{
      void           *pdata;
      struct stknode *pprev;
} StkNode;

typedef struct stk
{
      StkNode        *ptop;
      unsigned       vcount;
} Stk;

/*
** stkInit()
**
** Precondition   - pstack points to a stk type variable.
** Postcondition  - Stack pointed to by pstack is initialized.
** Returns        - None.
*/

extern void stkInit(Stk *pstack);

/*
** stkPush()
**
** Precondition   - pstack points to an initialized stack.
**                  pdata points to data to be stored in stack.
** Postcondition  - Data contained in pdata is pushed onto the stack.
** Returns        - Non-zero if success, 0 if error.
*/

extern int stkPush(Stk *pstack, void *pdata);

/*
** stkPop()
**
** Precondition   - ppdata points to a pointer to contain data.
**                  pstack points to initialized stack.
** Postcondition  - Top of stack is pushed into ppdata (pointer).
** Returns        - Non-zero if success, 0 if error.
*/

extern int stkPop(void **ppdata, Stk *pstack);

/*
** stkCount()
**
** Precondition   - pstack points to an initialized stack.
** Postcondition  - None.
** Returns        - Number of items on stack.
*/

extern unsigned stkCount(Stk *pstack);

#endif /* STK__H */
