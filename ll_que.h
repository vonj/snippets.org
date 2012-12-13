/* =======================================================================
    LL_QUE.h        Generic Queues for fixed size data-items
                    based on the LLS Singly Linked List module.

                    v0.51  94-08-20

                    Note that a LLS version with a 'last node pointer'
                    has some performance advantages on 'putting'.
                    Note that the List's current node pointer always is
                    the same as its first node pointer.
                    The ...Get... 'functions' cannot be used as parameters
                    to other functions.

                    This module has no related .C file.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LL_QUE__H
#define LL_QUE__H

#include "lls.h"

#define LLQueCreate(itemsize)   LLScreate( itemsize )
#define LLQueDelete(que)        LLSdelete( que )

#define LLQuePut(que,data)      LLSnodeAppend( que, data )
#define LLQuePutFrom(que,src)   LLSnodeAppendFrom( que, src )

#define LLQueRepair(que)        LLSnodePtr2First( que )
          /* For 'repair' purposes and also to check for an empty Queue */

/* ---- stored data management -------------------------------------------
   return typed data:
*/
#define LLQuePeekInt(que)       LLSnodeInt( que )
#define LLQuePeekLong(que)      LLSnodeLong( que )
#define LLQuePeekPtr(que)       LLSnodePtr( que )
#define LLQuePeekFptr(que)      LLSnodeFptr( que )

#define LLQueGetInt(que)        LLSnodeInt( que ),  LLSnodeDelete( que )
#define LLQueGetLong(que)       LLSnodeLong( que ), LLSnodeDelete( que )
#define LLQueGetPtr(que)        LLSnodePtr( que ),  LLSnodeDelete( que )
#define LLQueGetFptr(que)       LLSnodeFptr( que ), LLSnodeDelete( que )

/* 'return' typeless data.
*/
#define LLQuePeekDataTo(que,dest)     LLSnodeDataTo( que, dest )
#define LLQueGetDataTo(que,dest)      LLSnodeDataTo( que, dest ),        \
                                        (dest ? LLSnodeDelete( que ): 0 )

#endif /* LL_QUE__H */
/* ==== LL_QUE.h  end ================================================= */
