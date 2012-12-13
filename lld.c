/* =======================================================================
    LLD.c           Generic Doubly Linked Lists for fixed size data.
                    Each List has its own specific data size.
                    This version uses dummy head and dummy tail nodes.
                    Which prevents special handling for the first and last
                    nodes.

                    v1.00  94-08-21
                    v1.01  95-10-21  Changed ListCountInit to unsigned.

                    Compile with NDEBUG not defined for debugging version.
                    Compile with NDEBUG defined for production version.

                    The node pointers are restricted to valid values.
                    They point only in empty lists to invalid data.

                    Possible future enhancements:
                    - List(s) of free nodes for fast node memory alloc.
                    - FindFirst() & FindNext().
                    - Data access via first and/or last node pointers.
                      (duplicate the functions and change .current to
                      .first or .last)
                    - Node deletion via first and/or last node pointers.
                      (as for access functions, then simplify ...)

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#include <stdarg.h>         /* variable arg handling */
#include <assert.h>         /* debugging */

#if defined( __TURBOC__ ) && !defined( __BORLANDC__ )
 #include <stdio.h> /* required for early Turbo compilers with assert() */
#endif

#include "ll_defs.h"        /* debugging incl MALLOC (re-) definition   */
#include "LLD.h"            /* also includes extkword.h if necessary    */

#define NO_PROBLEMS LIST_NO_PROBLEMS /* local redefinition */

struct Node
{
    struct Node * next;
    struct Node * prev;
    int data;               /* also place-holder for larger items.      */
};                          /* actual nodes have various sizes,         */
                            /* but a fixed size within a list.          */
struct ListHead
{
    struct Node * current;  /* points to the actual current node        */
    struct Node * first;    /* always points to dummy head node         */
    struct Node * last;     /* always points to dummy tail node         */
    int itemsize ;          /* zero value: used as 'list not used' flag */
};

#define ERR_MEMORY          -1

#define NODE_MALLOC(list)   (struct Node *) \
                            MALLOC( ListControl[ list ].itemsize \
                                    + 2 * sizeof( struct Node * ), char )

#define NODE_FREE(node)     FREE(node)

/* ---- Local data ---------------------------------------------------- */

static struct ListHead * ListControl = NULL;
static unsigned int ListCount ;

/* ---- LL system management ------------------------------------------ */

static int ListInit( int List, int ItemSize )
{
    struct Node * Tmp ;

    if( 0 != ItemSize )
    {
        /* create dummy head node
        */
        Tmp = NODE_MALLOC( List );
        if( NULL == Tmp )
        {
            return ERR_MEMORY ;
        }
        Tmp->prev = NULL ;     /* NULL identifies it as dummy head node */
        Tmp->data = (int)0xA709;       /* dummy value                   */
        ListControl[ List ].first = Tmp ;

        /* create dummy tail node
        */
        Tmp = NODE_MALLOC( List );
        if( NULL == Tmp )
        {
            NODE_FREE( Tmp );          /* no need to cause memory leaks */
            ListControl[ List ].first = NULL ; /* or other errors       */
            return ERR_MEMORY ;        /* even if we're in trouble ...  */
        }
        Tmp->next = NULL ;     /* NULL identifies it as dummy tail node */
        Tmp->data = (int)0xA725;       /* dummy value                   */
        Tmp->prev = ListControl[ List ].first ;

        ListControl[ List ].current     =
        ListControl[ List ].last        =
        ListControl[ List ].first->next = Tmp ;
    }
    else
    {
        ListControl[ List ].current =
        ListControl[ List ].first   =
        ListControl[ List ].last    = NULL ;
    }

    ListControl[ List ].itemsize = ItemSize ; /* zero: list not used    */
    return NO_PROBLEMS ;
}

int LLDsystemInit( unsigned ListCountInit )
{
    assert( (unsigned) ( ListCountInit -1 ) <= 20 -1 );
                 /* higher than 20 is ridiculous for an initial setup   */
                 /* zero is useless                                     */

    if( NULL != ListControl )
    {
        return NO_PROBLEMS ; /* LL system already initialized */
    }

    ListControl = MALLOC( ListCountInit, struct ListHead );
    if( NULL == ListControl )
    {
        return ERR_MEMORY ;
    }

    for( ListCount = 0 ; ListCount < ListCountInit ; ListCount++ )
        ListInit( ListCount, 0 ); /* just mark it as usable ... */

    /* ListCount is now ListCountInit */
    assert( ListCount == ListCountInit );

    return NO_PROBLEMS;
}

int LLDcreate( int ItemSize )
{
    unsigned List ;

    assert( (unsigned) ( ItemSize -1 ) < 1024 -1 ) ;
                             /* limit to 1kB. A size of 0 is ridiculous */

    /* trigger automatic system initialisation if necessary
    */
    if( NULL == ListControl  &&  0 != LLDsystemInit( 1 ))
    {
        return ERR_MEMORY ;
    }

    /* Look for empty slot
    */
    for( List = 0; List < ListCount; List++ )
    {
        if( 0 == ListControl[ List ].itemsize )
            break;
    }

    /*  What if NO EMPTY slot ???
    */
    if( List == ListCount )
    {
        struct ListHead * tmp ;         /* ListControl expansion needed */

        tmp = MALLOC( ListCount + 1, struct ListHead );
        if( NULL == tmp )
        {
            return ERR_MEMORY ;
        }

        memcpy( tmp, ListControl, ListCount * sizeof( struct ListHead ));
        ListControl = tmp ;
        ListCount++ ;
    }

    /* create dummy head node and set up ListControl for the list.
    */
    if( ERR_MEMORY == ListInit( List, ItemSize ))
    {
        return ERR_MEMORY ;
    }

    return (int) List ;
}

void LLDdelete( int List )
{
    struct Node * Tmp ;
    struct Node * Old ;

    assert( (unsigned) List < ListCount );

    Tmp = ListControl[ List ].first ; /* dummies are also deleted !!!   */
    while( NULL != Tmp )              /* still assuming last node has   */
    {                                 /* a NULL next pointer ...        */
        Old = Tmp ;
        Tmp = Old->next;
        NODE_FREE( Old ); /* data already presumed to be deleted */
    }

    ListInit( List, 0 ); /* 0: mark list as not used. */

    return ;
}

/* ---- LL system maintenance ----------------------------------------- */

int LLDcheck( int List )
{
    if( NULL == ListControl )
    {
        return LIST_SYSTEM_NULL ;
    }

    if( (unsigned) List >= ListCount )
    {
        return LIST_INV_NUMBER ;
    }

    if( 0 == ListControl[ List ].itemsize )
    {
        return LIST_NOT_CREATED ;
    }

    if( NULL == ListControl[ List ].first
        || NULL == ListControl[ List ].first->next    /* missing tail ? */
        || NULL != ListControl[ List ].first->prev )
    {
        return LIST_ERR_HEAD ;
    }

    /* Validate current pointer
    */
    if( NULL == ListControl[ List ].current )
    {
        return LIST_CORRUPT7 ;    /* shouldn't be NULL with a good head */
    }

    if( NULL != ListControl[ List ].first->next->next ) /* empty list ? */
    {                                                   /* not empty.   */
        struct Node * tmp = ListControl[ List ].first ;

        if( NULL == ListControl[ List ].current->next )
        {
            return LIST_CORRUPT6 ; /* a NULL next pointer is only valid */
        }                          /* for an empty list.                */

        /* look for .current in list,
           checking the .prev links along the way
        */
        do
        {
            tmp = tmp->next ;

            if( NULL == tmp || NULL == tmp->prev
                || tmp != tmp->prev->next )
            {
                return LIST_CORRUPT5 ;  /* current not found in list */
            }                           /* or link to/from next node */
                                        /* invalid                   */
        }while( tmp != ListControl[ List ].current );

        /* Found .current in list. Also without link errors.
           Now look for valid last node pointer in the list,
           checking the .prev links along the way
           Note that .current itself is never supposed to be equal
           to .last (which points to the dummy tail) !
        */
        if( NULL == ListControl[ List ].last )
        {
            return LIST_ERR_LAST ;
        }

        do
        {
            tmp = tmp->next ;
            if( NULL == tmp || NULL == tmp->prev
                || tmp != tmp->prev->next )
            {
                return LIST_CORRUPT4 ;  /* last not found in list    */
            }                           /* or link to/from prev node */
                                        /* invalid                   */
        }while( tmp != ListControl[ List ].last );

        /* Found .last in list but is it really a valid last pointer?
           Note: tmp == .last
        */
        if( NULL != tmp->next )
        {
            return LIST_CORRUPT3 ;
        }

        return NO_PROBLEMS ;
    }

    /* .first->next->next == NULL  =>  list is empty
    */
    if( ListControl[ List ].current != ListControl[ List ].first->next )
    {
        return LIST_CORRUPT2 ;
    }

    if( ListControl[ List ].last != ListControl[ List ].first->next
        || ListControl[ List ].last
                             != ListControl[ List ].current->prev->next )
    {
        return LIST_CORRUPT1 ;
    }

    return LIST_EMPTY ;
}

/* ---- node management ----------------------------------------------- */

int LLDnodeInsert( int List, ... )      /* insert _BEFORE_ current node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLDnodeInsertFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLDnodeAdd( int List, ... )          /* insert _AFTER_ current node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLDnodeAddFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLDnodePrepend( int List, ... )             /* insert as first node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLDnodePrependFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLDnodeAppend( int List, ... )               /* insert as last node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLDnodeAppendFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLDnodeInsertFrom( int List, void * Source )
{                                       /* insert _BEFORE_ current node */
    struct Node * New ;

    assert( (unsigned) List < ListCount );

    /* create new node if possible
    */
    New = NODE_MALLOC( List );
    if( NULL == New )
    {
        return ERR_MEMORY ;
    }

    /* fill node with data, link to next and previous nodes
       and adjust current node pointer
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );
    New->next = ListControl[ List ].current;
    New->prev = ListControl[ List ].current->prev;

    ListControl[ List ].current->prev = New ;
    New->prev->next = New ;

    ListControl[ List ].current = New ;

    return NO_PROBLEMS;
}

int LLDnodeAddFrom( int List, void * Source )
{                                        /* insert _AFTER_ current node */
    struct Node * New ;

    assert( (unsigned) List < ListCount );

    /* create new node if possible
    */
    New = NODE_MALLOC( List );
    if( NULL == New )
    {
        return ERR_MEMORY ;
    }

    /* fill node with data and link to next and previous nodes
       with special handling when the current node pointer points
       to the dummy tail node: i.e it is an empty list.
       (the same case in a non-empty list is made not to occur.)
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );

    if( NULL != ListControl[ List ].current->next )
        ListControl[ List ].current = ListControl[ List ].current->next ;

    New->next = ListControl[ List ].current;
    New->prev = ListControl[ List ].current->prev;

    ListControl[ List ].current->prev = New ;
    New->prev->next = New ;

    ListControl[ List ].current = New ;

    return NO_PROBLEMS;
}

int LLDnodePrependFrom( int List, void * Source )
{                                               /* insert as first node */
    struct Node * New ;

    assert( (unsigned) List < ListCount );

    /* create new node if possible
    */
    New = NODE_MALLOC( List );
    if( NULL == New )
    {
        return ERR_MEMORY ;
    }

    /* fill node with data and link to dummy head and actual first nodes
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );
    New->prev = ListControl[ List ].first;     /* == .first->next->prev */
    New->next = ListControl[ List ].first->next;

    ListControl[ List ].first->next = New;
    New->next->prev = New ;

    /* Prevent .current from pointing at the dummy tail
       (New is the only normal node...)
    */
    if( NULL == ListControl[ List ].current->next )
        ListControl[ List ].current = New;

    return NO_PROBLEMS;
}

int LLDnodeAppendFrom( int List, void * Source )
{                                                /* insert as last node */
    struct Node * New ;

    assert( (unsigned) List < ListCount );

    /* create new node if possible
    */
    New = NODE_MALLOC( List );
    if( NULL == New )
    {
        return ERR_MEMORY ;
    }

    /* fill node with data and link to dummy tail and actual last nodes
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );
    New->next = ListControl[ List ].last ;      /* == .last->prev->next */
    New->prev = ListControl[ List ].last->prev;

    ListControl[ List ].last->prev = New ;
    New->prev->next = New ;

    /* Prevent .current from pointing at the dummy tail
       (New is the only normal node...)
    */
    if( NULL == ListControl[ List ].current->next )
        ListControl[ List ].current = New;

    return NO_PROBLEMS;
}

void LLDnodeDelete( int List )
{
    struct Node * Old = ListControl[ List ].current ;

    assert( (unsigned) List < ListCount );

    if( NULL == ListControl[ List ].current->next )
    {
        return ;  /* don't delete dummy tail node (list is empty) */
    }

    /* adjust links
    */
    Old->prev->next = Old->next ;
    Old->next->prev = Old->prev ;

    /* adjust current node pointer
       prevent it from pointing to the dummy tail node
    */
    if( NULL != Old->next->next )
        ListControl[ List ].current = Old->next ;
    else
        ListControl[ List ].current = Old->prev ;

    NODE_FREE( Old );

    return ;
}

int LLDnodeFind( int List, CompFunPtr Compare, void * DataPtr )
{                        /* FindFirst/FindNext format may be needed ... */
    int RetVal ;

    assert( (unsigned) List < ListCount );

    if( NULL == ListControl[ List ].first->next->next ) /* empty list ? */
    {
        return 2; /* a compare usually returns just -1, 0 or 1 !!! */
    }

    /* note: current->next will never be NULL in a non-empty list */

    if( NULL == Compare ) /* default to memcmp with .itemsize */
    {
        while( 0 != (RetVal = memcmp( DataPtr,
                                      & ListControl[ List ].current->data,
                                      ListControl[ List ].itemsize ))
               && NULL != ListControl[ List ].current->next->next )
        {
            ListControl[ List ].current=ListControl[ List ].current->next;
        }
        return RetVal ;
    }
    else
    {
        while( 0 != (RetVal = (*Compare)( DataPtr,
                                   & ListControl[ List ].current->data ))
               && NULL != ListControl[ List ].current->next->next )
        {
            ListControl[ List ].current=ListControl[ List ].current->next;
        }
        return RetVal ;
    }
}

/* ---- current node pointer management ------------------------------- */

int  LLDnodePtr2First( int List )
{
    assert( (unsigned) List < ListCount );

    ListControl[ List ].current = ListControl[ List ].first->next ;

    return NULL != ListControl[ List ].first->next->next ;
}

int  LLDnodePtr2Last( int List )
{
    assert( (unsigned) List < ListCount );

    ListControl[ List ].current = ListControl[ List ].last->prev ;

    return NULL != ListControl[ List ].last->prev->prev ;
}

int  LLDnodePtr2Next( int List )
{
    assert( (unsigned) List < ListCount );

    if( NULL == ListControl[ List ].current->next       /* empty list ? */
        || NULL == ListControl[ List ].current->next->next ) /* at end ?*/
    {
        return 0 ;             /* do not allow the current node pointer */
    }                          /* to point at the dummy tail node ...   */

    ListControl[ List ].current = ListControl[ List ].current->next ;
    return 1 ;
}

int  LLDnodePtr2Prev( int List )
{
    assert( (unsigned) List < ListCount );

    if( NULL == ListControl[ List ].current->prev       /* empty list ? */
        || NULL == ListControl[ List ].current->prev->prev ) /* begin ? */
    {
        return 0 ;             /* do not allow the current node pointer */
    }                          /* to point at the dummy head node ...   */

    ListControl[ List ].current = ListControl[ List ].current->prev ;
    return 1 ;
}

/* ---- stored data management ---------------------------------------- */

int LLDnodeInt( int List )
{
    return ListControl[ List ].current->data;
}

long LLDnodeLong( int List )
{
    return *((long *) &ListControl[ List ].current->data );
}

void * LLDnodePtr( int List )
{
    return *((void **) &ListControl[ List ].current->data );
}

void FAR * LLDnodeFptr( int List )
{
    return *((void FAR **) &ListControl[ List ].current->data );
}

int LLDnodeDataTo( int List, void * Destination )
{
    if( NULL != Destination )
    {
        memcpy( Destination,
                & ListControl[ List ].current->data,
                ListControl[ List ].itemsize );
    }

    return ListControl[ List ].itemsize ;       /* size needed for blob */
}

/* ==== LLD.c  end ==================================================== */
