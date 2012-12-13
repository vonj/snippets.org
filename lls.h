/* =======================================================================
    LLS.h           Generic Singly Linked List for fixed size data-items.

                    v1.00  94-08-21
                    v1.01  95-10-21  Changed ListCountInit to unsigned.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#ifndef LLS__H
#define LLS__H

#ifndef FAR
#include "extkword.h"
#endif

#ifndef LL__ERR_H
#define LL__ERR_H       /* same values used in LLD ...                  */

enum ListErrors         /* return values for LLScheck()                 */
{                       /* The highest value is returned                */

    LIST_NO_PROBLEMS,   /* All is OK (multiple use)                     */
    LIST_EMPTY,         /* No data available                            */
    LIST_ERRORS,        /* Dummy to separate warnings from              */
                        /* ---- REAL errors --------------------------- */
    LIST_CORRUPT1,      /* invalid last node pointer: Not NULL          */
                        /*      (empty list: ptr should have been NULL) */
    LIST_CORRUPT2,      /* invalid current node pointer: Not NULL       */
                        /*      (empty list: ptr should have been NULL) */
    LIST_CORRUPT3,      /* invalid last node pointer: Not really last.  */
    LIST_CORRUPT4,      /* invalid last node pointer: Not in list.      */
    LIST_ERR_LAST,      /* invalid last node pointer: NULL              */
    LIST_CORRUPT5,      /* invalid current node pointer: Not in list.   */
    LIST_CORRUPT6,      /* invalid current->next node pointer: NULL     */
                        /*      although the list is not empty          */
    LIST_CORRUPT7,      /* NULL current node pointer                    */
    LIST_ERR_HEAD,      /* NULL first node pointer                      */
    LIST_NOT_CREATED,   /* List deleted or not created                  */
    LIST_INV_NUMBER,    /* List number out of range                     */
    LIST_SYSTEM_NULL,   /* List system not initialized                  */
};
            /* NOTE: 'invalid last node pointer' errors will ONLY occur */
            /* when LLS.c is compiled with USE_LASTPTR defined ...      */

typedef int (*CompFunPtr)( const void *, const void * );
                                             /* simplifies declarations */
#endif

/* ---- LL system management and maintenance -------------------------- */
int  LLSsystemInit( unsigned ListCount );
               /* returns -1 on failure. It is not required to call it. */
               /* A second call does nothing: ListCount is ignored.     */

int  LLScreate( int ItemSize );
                        /* returns list number to use or -1 on failure. */
                        /* MUST be called before using a list.          */
                        /* Calls LLsystemInit if necessary.             */

void LLSdelete( int List ); /* delete entire list, data is NOT free()'d */

int  LLScheck( int List ); /* returns enum ListErrors value             */
                           /* its primary purpose is debugging.         */

/* ---- Node management --------------------------------------------------
   Functions changing current node pointer to the new node.
   Each created list has its own -- fixed -- datasize. See LLcreate().
   An ellipsis "..." indicates the data to insert.
*/
int  LLSnodeInsert( int List, ... );      /* insert BEFORE current node */
int  LLSnodeAdd( int List, ... );         /* insert AFTER current node  */
         /* a return value of -1 indicates a memory allocation problem. */

/* Functions NOT changing the current node pointer.
   Especially intended for implementation of Queue's and Stacks.
*/
int  LLSnodePrepend( int List, ... );           /* insert as first node */
int  LLSnodeAppend( int List, ... );            /* insert as last node  */
         /* a return value of -1 indicates a memory allocation problem. */

/* The following four functions are essentially the same as the preceding
   four. The data is however not passed by value but by reference.
*/
int  LLSnodeInsertFrom( int List, void * Source );
int  LLSnodeAddFrom( int List, void * Source );
int  LLSnodePrependFrom( int List, void * Source );
int  LLSnodeAppendFrom( int List, void * Source );

void LLSnodeDelete( int List );                  /* remove current node */
        /* current node ptr moved to next node. UNLESS the deleted node */
        /* was the last node: then current ptr moved to previous node   */

int  LLSnodeFind( int List, CompFunPtr Compare, void * DataPtr );
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
int  LLSnodePtr2First( int List );
int  LLSnodePtr2Last( int List );
int  LLSnodePtr2Next( int List );
int  LLSnodePtr2Prev( int List );

/* ---- stored data management -------------------------------------------
   return typed data:
*/
int  LLSnodeInt( int List );
long LLSnodeLong( int List );
void * LLSnodePtr( int List );
void FAR * LLSnodeFptr( int List );

/* 'return' typeless data. The return value is the size of the data.
   The data is transferred to Destination.
   If 'Destination' is NULL, the only action is returning the size.
*/
int LLSnodeDataTo( int List, void * Destination );

#endif /* LLS__H */
/* ==== LLS.h  end ==================================================== */
