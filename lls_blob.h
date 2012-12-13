/* =======================================================================
    LLS_BLOB.h      Generic Singly Linked List for Binary Large OBjects.
                    Linked Lists for variable size data-items.

                    v1.00  94-08-11

                    - Based on the LLS module for fixed size data-items.
                    - Use the functions in the LLS module for operations
                      not specific to Blobs. You can use LLSnodePtr() to
                      obtain a pointer to the stored Blob.
                    - Note that From and To suffixes to function names are
                      implied in the Blob data related functions.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LLS_BLOB__H
#define LLS_BLOB__H

/* ---- LL blob system management and maintenance --------------------- */

int  LLSblobCreate( void );
                        /* returns list number to use or -1 on failure. */
                        /* MUST be called before using a list of blobs. */

/* ---- Node management --------------------------------------------------
   Functions changing current node pointer to the new node.
   A return value of -1 indicates a memory allocation problem.
*/
int  LLSblobInsert( int List, void * Source, unsigned Size );
                                          /* insert BEFORE current node */
int  LLSblobAdd( int List, void * Source, unsigned Size );
                                          /* insert AFTER current node  */

/* Functions NOT changing the current node pointer.
   Especially intended for implementation of Queue's and Stacks.
*/
int  LLSblobPrepend( int List, void * Source, unsigned Size );
                                                /* insert as first node */
int  LLSblobAppend( int List, void * Source, unsigned Size );
                                                /* insert as last node  */

void LLSblobDelete( int List );
        /* remove current node and free() the data.                     */
        /* current node ptr moved to next node. UNLESS the deleted node */
        /* was the last node: then current ptr moved to previous node   */

/* ---- stored data management -------------------------------------------
   'return' typeless data. The return value is the size of the data.
   The data is transferred to Destination.
   If 'Destination' is NULL, the only action is returning the size.
*/
unsigned LLSblobData( int List, void * Destination );

#endif /* LLS_BLOB__H */
/* ==== LLS_BLOB.h  end =============================================== */
