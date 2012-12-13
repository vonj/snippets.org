/* =======================================================================
    LLS_STR.h       Generic Singly Linked List for Strings.

                    v1.00  94-08-11

                    - Based on the LLS_BLOB module for variable size data-
                      items. Use the functions in the LLS module for
                      operations not specific to Strings.
                    - Refer to LLS_BLOB.h for comments.
                    - Note that ALL functions of LLS_BLOB are redefined
                      for consistency reasons.

                    This module has no associated .C files.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LLS_STR__H
#define LLS_STR__H

#include <string.h>
#include "lls_blob.h"

#define LLSstringCreate()       LLSblobCreate()

#define LLSstringInsert(l,s)    LLSblobInsert( l, s, strlen( s ) +1 )

#define LLSstringAdd(l,s)       LLSblobAdd( l, s, strlen( s ) +1 )

#define LLSstringPrepend(l,s)   LLSblobPrepend( l, s, strlen( s ) +1 )

#define LLSstringAppend(l,s)    LLSblobAppend( l, s, strlen( s ) +1 )

#define LLSstringDelete(l)      LLSblobDelete( l )

#define LLSstringData(l,d)      LLSblobData( l, d )
                                /* returns strlen() +1 !!! */

#endif /* LLS_STR__H */
/* ==== LLS_STR.h  end ================================================ */
