/*
**  STK.C - Stack manager
**
**  by Dustin Puryear (dpuryear@intersurf.com), placed into Public Domain.
*/

#include <stdio.h>       /* General include file */
#include <stdlib.h>      /* malloc() and free()  */
#include "stk.h"         /* Function prototypes  */

void stkInit(Stk *pstack)
{
      pstack->ptop = NULL;
      pstack->vcount = 0;
}

int stkPush(Stk *pstack, void *pdata)
{
      StkNode *padd;   /* Node to add to top of stack */

      /*
      ** Create the node to append to the top of the stack. Also,
      ** check for a memory allocation error.
      */

      padd = (StkNode *) malloc(sizeof(StkNode));
      if ( padd == NULL )
      {
            return (0);
      }

      /*
      ** Now, add the data pointer to the node.
      */

      padd->pdata = pdata;

      /*
      ** Manipulate the stack so that the new node is on top, and
      ** increment count.
      */

      if ( pstack->ptop == NULL )
      {
            padd->pprev = NULL;   /* Anchor it!  */
            pstack->ptop = padd;
      }
      else
      {
            padd->pprev = pstack->ptop;
            pstack->ptop = padd;
      }
      pstack->vcount++;

      return (1);
}

int stkPop(void **ppdata, Stk *pstack)
{
      StkNode *pdel;   /* Pointer to node to remove (top) */

      /*
      ** Check to see if stack is empty.
      */

      if ( pstack == NULL )
      {
            ppdata = NULL;
            return (0);
      }

      /*
      ** Pop data pointer value into ppdata (pointer to user's pointer)
      */

      *ppdata = pstack->ptop->pdata;

      /*
      ** Pop the top of the stack off and make the next in line top.
      ** Also, decrement counter.
      */

      pdel = pstack->ptop;
      pstack->ptop = pstack->ptop->pprev;
      free(pdel);
      pstack->vcount--;

      return (1);
}

unsigned stkCount(Stk *pstack)
{
      return (pstack->vcount);
}
