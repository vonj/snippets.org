/* =======================================================================
    LLD.h           Generic Doubly Linked List for fixed size data-items.

                    v1.00  94-08-21
                    v1.01  95-10-21  Changed ListCountInit to unsigned.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LLD__H
#define LLD__H

#ifndef FAR
#include "extkword.h"
#endif

#ifndef LL__ERR_H
#define LL__ERR_H       /* same values used in LLS ...                  */

enum ListErrors         /* return values for LLDcheck()                 */
{                       /* The highest value is returned                */

    LIST_NO_PROBLEMS,   /* All is OK (multiple use)                     */
    LIST_EMPTY,         /* No data available                            */
    LIST_ERRORS,        /* Dummy to separate warnings from              */
                        /* ---- REAL errors --------------------------- */
    LIST_CORRUPT1,      /* invalid last node pointer: != first->next    */
                        /*      (empty list) or link error              */
    LIST_CORRUPT2,      /* invalid current node pointer: != first->next */
                        /*      (empty list)                            */
    LIST_CORRUPT3,      /* invalid last node pointer: Not really last.  */
    LIST_CORRUPT4,      /* invalid last node pointer: Not in list,      */
                        /*      or link error after current node        */
    LIST_ERR_LAST,      /* invalid last node pointer: NULL              */
    LIST_CORRUPT5,      /* invalid current node pointer: Not in list,   */
                        /*      or link error before current node       */
    LIST_CORRUPT6,      /* invalid current->next node pointer: NULL     */
                        /*      although the list is not empty          */
    LIST_CORRUPT7,      /* NULL current node pointer                    */
    LIST_ERR_HEAD,      /* NULL first node pointer                      */
                        /*      or error in head node                   */
    LIST_NOT_CREATED,   /* List deleted or not created                  */
    LIST_INV_NUMBER,    /* List number out of range                     */
    LIST_SYSTEM_NULL,   /* List system not initialized                  */
};

typedef int (*CompFunPtr)( const void *, const void * );
                                             /* simplifies declarations */
#endif

/* ---- LL system management and maintenance -------------------------- */
int  LLDsystemInit( unsigned ListCount );
               /* returns -1 on failure. It is not required to call it. */
               /* A second call does nothing: ListCount is ignored.     */

int  LLDcreate( int ItemSize );
                        /* returns list number to use or -1 on failure. */
                        /* MUST be called before using a list.          */
                        /* Calls LLsystemInit if necessary.             */

void LLDdelete( int List ); /* delete entire list, data is NOT free()'d */

int  LLDcheck( int List ); /* returns enum ListErrors value             */
                           /* its primary purpose is debugging.         */

/* ---- Node management --------------------------------------------------
   Functions changing current node pointer to the new node.
   Each created list has its own -- fixed -- datasize. See LLcreate().
   An ellipsis "..." indicates the data to insert.
*/
int  LLDnodeInsert( int List, ... );      /* insert BEFORE current node */
int  LLDnodeAdd( int List, ... );         /* insert AFTER current node  */
         /* a return value of -1 indicates a memory allocation problem. */

/* Functions NOT changing the current node pointer.
   Especially intended for implementation of Queue's and Stacks.
*/
int  LLDnodePrepend( int List, ... );           /* insert as first node */
int  LLDnodeAppend( int List, ... );            /* insert as last node  */
         /* a return value of -1 indicates a memory allocation problem. */

/* The following four functions are essentially the same as the preceding
   four. The data is however not passed by value but by reference.
*/
int  LLDnodeInsertFrom( int List, void * Source );
int  LLDnodeAddFrom( int List, void * Source );
int  LLDnodePrependFrom( int List, void * Source );
int  LLDnodeAppendFrom( int List, void * Source );

void LLDnodeDelete( int List );                  /* remove current node */
        /* current node ptr moved to next node. UNLESS the deleted node */
        /* was the last node: then current ptr moved to previous node   */

int  LLDnodeFind( int List, CompFunPtr Compare, void * DataPtr );
        /* Find *DataPtr in the List using the *Compare function.       */
        /* Returns the return value of *Compare. 0 == equal == found.   */
        /* non-zero == not found. Current node is set to found node.    */
        /* Returns 2 for an empty list.                                 */
        /* First checked node is current node.                          */
        /* A NULL Compare-function defaults to the use of memcmp() with */
        /* the list's itemsize as third (size) parameter.               */
        /* simple implementation. FindFirst and FindNext may be needed. */

/* ---- current node pointer management ----------------------------------
   These functions change the current node pointer and return 1 when there
   was a change. A return of 0 indicates trying to move past the begin or
   the end of the list, or an empty list. The return value is intended for
   iteration purposes. I.e. stopping a scan through a list.
*/
int  LLDnodePtr2First( int List );
int  LLDnodePtr2Last( int List );
int  LLDnodePtr2Next( int List );
int  LLDnodePtr2Prev( int List );

/* ---- stored data management -------------------------------------------
   return typed data:
*/
int  LLDnodeInt( int List );
long LLDnodeLong( int List );
void * LLDnodePtr( int List );
void FAR * LLDnodeFptr( int List );

/* 'return' typeless data. The return value is the size of the data.
   The data is transferred to Destination.
   If 'Destination' is NULL, the only action is returning the size.
*/
int LLDnodeDataTo( int List, void * Destination );

#endif /* LLD__H */
/* ==== LLD.h  end ==================================================== */
