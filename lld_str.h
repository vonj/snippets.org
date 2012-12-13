/* =======================================================================
    LLD_STR.h       Generic Doubly Linked List for Strings.

                    v1.00  94-08-11

                    - Based on the LLD_BLOB module for variable size data-
                      items. Use the functions in the LLD module for
                      operations not specific to Strings.
                    - Refer to LLD_BLOB.h for comments.
                    - Note that ALL functions of LLD_BLOB are redefined
                      for consistency reasons.

                    This module has no associated .C files.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LLD_STR__H
#define LLD_STR__H

#include <string.h>
#include "lld_blob.h"

#define LLDstringCreate()       LLDblobCreate()

#define LLDstringInsert(l,s)    LLDblobInsert( l, s, strlen( s ) +1 )

#define LLDstringAdd(l,s)       LLDblobAdd( l, s, strlen( s ) +1 )

#define LLDstringPrepend(l,s)   LLDblobPrepend( l, s, strlen( s ) +1 )

#define LLDstringAppend(l,s)    LLDblobAppend( l, s, strlen( s ) +1 )

#define LLDstringDelete(l)      LLDblobDelete( l )

#define LLDstringData(l,d)      LLDblobData( l, d )
                                /* returns strlen() +1 !!! */

#endif /* LLD_STR__H */
/* ==== LLD_STR.h  end ================================================ */
