/* =======================================================================
    STK_STR.h       Stack of Strings based on generic stack of Blobs.

                    A.Reitsma, Delft, The Netherlands.
                    v0.10  94-07-02  Public Domain.

                    This module has no related .C file.
                    Modules STK_BLOB and STACK are required.
___-------------------------------------------------------------------- */

#ifndef STK_STR_H
#define STK_STR_H

#include <string.h>
#include "stk_blob.h"

#define StackStringCreate()       StackBlobCreate()
#define PushString(Stack,Str)     PushBlob( Stack, Str, strlen( Str ) +1 )
#define PopString(Stack,Str)      PopBlob( Stack, Str )

#endif
/* ==== STK_STR.c end ================================================= */
