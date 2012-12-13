/* =======================================================================
    LL_STACK.h      Generic Stacks for fixed size data-items
                    based on the LLS Singly Linked List module.

                    v0.50  94-08-12

                    Note that the List's current node pointer always is
                    the same as its first node pointer.
                    This module has no related .C file.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LL_STACK__H
#define LL_STACK__H

#include "lls.h"

#define LLstackCreate(itemsize)        LLScreate( itemsize )
#define LLstackDelete(stack)           LLSdelete( stack )

#define LLstackPush(stack,data)        LLSnodePrepend( stack, data )
#define LLstackPushFrom(stack,source)  LLSnodePrependFrom( stack, source )

#define LLstackRepair(stack)           LLSnodePtr2First( stack )
          /* For 'repair' purposes and also to check for an empty Stack */

/* ---- stored data management -------------------------------------------
   'return' typed data:
*/
#define LLstackPopInt(stack)       LLSnodeInt(stack), LLSnodeDelete(stack)
#define LLstackPopLong(stack)      LLSnodeLong(stack),LLSnodeDelete(stack)
#define LLstackPopPtr(stack)       LLSnodePtr(stack), LLSnodeDelete(stack)
#define LLstackPopFptr(stack)      LLSnodeFptr(stack),LLSnodeDelete(stack)

/* 'return' typeless data.
*/
#define LLstackPopTo(stack,dest)   LLSnodeDataTo(stack,dest),           \
                                       (dest ? LLSnodeDelete(stack) : 0 )

#endif /* LL_STACK__H */
/* ==== LL_STACK.h  end =============================================== */
