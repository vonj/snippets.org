/* =======================================================================
    LLS.c           Generic Singly Linked Lists for fixed size data.
                    Each List has its own specific data size.
                    This version uses a dummy head node, which prevents
                    special handling of the first node.

                    v1.00  94-08-21
                    v1.01  95-10-21  Changed ListCountInit to unsigned.

                    Compile with NDEBUG not defined for debugging version.
                    Compile with NDEBUG defined for production version.

                    Prepared for use of a last node pointer.
                    Compile with USE_LASTPTR defined to use it.

                    The node pointers are restricted to valid values.
                    They point only in empty lists to invalid data.

                    Possible future enhancements:
                    - List(s) of free nodes for fast node memory alloc.
                      Or a special memory sub-allocator.
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
#include "LLS.h"            /* also includes extkword.h if necessary    */

#define NO_PROBLEMS LIST_NO_PROBLEMS /* local redefinition */

struct Node
{
    struct Node * next;
    int data;               /* also place-holder for larger items.      */
};                          /* actual nodes have various sizes,         */
                            /* but a fixed size within a list.          */
struct ListHead
{
    struct Node * current;  /* will actually point to preceding node    */
    struct Node * first;    /* always points to dummy head node         */

#ifdef USE_LASTPTR

    struct Node * last;     /* will actually point to preceding node    */

#endif

    int itemsize ;          /* zero value: used as 'list not used' flag */
};

#define ERR_MEMORY          -1

#define NODE_MALLOC(list)   (struct Node *) \
                            MALLOC( ListControl[ list ].itemsize \
                                    + sizeof( struct Node * ), char )

#define NODE_FREE(node)     FREE(node)

/* ---- Local data ---------------------------------------------------- */

static struct ListHead * ListControl = NULL;
static unsigned int ListCount ;

/* ---- LL system management ------------------------------------------ */

static int ListInit( int List, int ItemSize )
{
    struct Node * Tmp ;

    /* Create dummy head node.
       This is not a part of of the ListControl structure because that can
       and will move, making 'first' invalid. That _could_ be handled by
       adjusting it; or by getting rid of 'first' entirely and having its
       function taken over by "&.head" and ".first->next" by ".head.next".
    */
    if( 0 != ItemSize )
    {
        Tmp = NODE_MALLOC( List );
        if( NULL == Tmp )
        {
            return ERR_MEMORY ;
        }
        Tmp->next = NULL;
        Tmp->data = 0x4709 ; /* dummy value */
    }
    else
        Tmp = NULL ;

    /* initialize control structure
    */
    ListControl[ List ].current  =
    ListControl[ List ].first    = Tmp ;

#ifdef USE_LASTPTR

    ListControl[ List ].last     = Tmp ;

#endif

    ListControl[ List ].itemsize = ItemSize ; /* zero: list not used    */
    return NO_PROBLEMS ;
}

int LLSsystemInit( unsigned ListCountInit )
{
    assert( ( ListCountInit -1 ) <= 20 -1 );
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

int LLScreate( int ItemSize )
{
    unsigned List ;

    assert( (unsigned) ( ItemSize -1 ) < 1024 -1 ) ;
                             /* limit to 1kB. A size of 0 is ridiculous */

    /* trigger automatic system initialisation if necessary
    */
    if( NULL == ListControl  &&  0 != LLSsystemInit( 1 ))
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
        {                               /* realloc is not used because  */
            return ERR_MEMORY ;         /* MALLOC is not necessarily    */
        }                               /* based on malloc()            */

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

void LLSdelete( int List )
{
    struct Node * Tmp ;
    struct Node * Old ;

    assert( (unsigned) List < ListCount );

    Tmp = ListControl[ List ].first ; /* dummy head is also deleted !!! */
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

int LLScheck( int List )
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

    if( NULL == ListControl[ List ].first )
    {
        return LIST_ERR_HEAD ;
    }

    /* Validate current pointer,
       and the last node pointer if it is used ...
    */
    if( NULL == ListControl[ List ].current )
    {
        return LIST_CORRUPT7 ;    /* shouldn't be NULL with a good head */
    }

    if( NULL != ListControl[ List ].first->next )       /* list empty ? */
    {                                                   /* not empty    */
        struct Node * tmp = ListControl[ List ].first ;

        if( NULL == ListControl[ List ].current->next )
        {
            return LIST_CORRUPT6 ; /* a NULL next pointer is only valid */
        }                          /* for an empty list.                */

        /* look for .current in list
        */
        while( tmp != ListControl[ List ].current )
        {
            tmp = tmp->next ;

            if( NULL == tmp )
            {
                return LIST_CORRUPT5 ;  /* current not found in list */
            }
        }

#ifdef USE_LASTPTR

        /* Found .current in list.
           Now look for valid last node pointer in list
        */
        if( NULL == ListControl[ List ].last )
        {
            return LIST_ERR_LAST ;
        }

        while( tmp != ListControl[ List ].last )
        {
            tmp = tmp->next ;
            if( NULL == tmp )
            {
                return LIST_CORRUPT4 ;  /* last not found in list */
            }
        }

        /* Found .last in list but is it really the last ?
           Note that last should actually point to the preceding node ...
           Note: tmp == .last
        */
        if( NULL == tmp->next || NULL != tmp->next->next )
        {
            return LIST_CORRUPT3 ; /* a NULL next pointer is only valid */
        }                          /* for an empty list. But next->next */
                                   /* should be NULL for last pointer   */
#endif

        return NO_PROBLEMS ;
    }

    /* .first->next == NULL -> list is empty
    */
    if( ListControl[ List ].current != ListControl[ List ].first )
    {
        return LIST_CORRUPT2 ;
    }

#ifdef USE_LASTPTR

    if( ListControl[ List ].last != ListControl[ List ].first )
    {
        return LIST_CORRUPT1 ;
    }

#endif

    return LIST_EMPTY ;
}

/* ---- node management ----------------------------------------------- */

int LLSnodeInsert( int List, ... )      /* insert _BEFORE_ current node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLSnodeInsertFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLSnodeAdd( int List, ... )          /* insert _AFTER_ current node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLSnodeAddFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLSnodePrepend( int List, ... )             /* insert as first node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLSnodePrependFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLSnodeAppend( int List, ... )               /* insert as last node */
{
    va_list DataPtr ;
    int Retval ;

    /* set DataPtr to the address of "..."
       then action, cleanup and return.
    */
    va_start( DataPtr, List );

    Retval = LLSnodeAppendFrom( List, DataPtr );

    va_end( DataPtr );
    return Retval ;
}

int LLSnodeInsertFrom( int List, void * Source )
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

    /* fill node with data and link to previous node
       Note that explicitly changing .current is not necessary!
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );
    New->next = ListControl[ List ].current->next;
    ListControl[ List ].current->next = New ;

#ifdef USE_LASTPTR
    /* advance last node pointer if needed
    */
    if( NULL != ListControl[ List ].last->next->next )
        ListControl[ List ].last = ListControl[ List ].last->next ;

#endif

    return NO_PROBLEMS;
}

int LLSnodeAddFrom( int List, void * Source )
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

    /* fill node with data and link to previous node,
       with special handling if the list is empty
       Note that the changing of .current is the only difference with
       the LLSnodeInsertFrom() function!
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );

    if( NULL != ListControl[ List ].current->next )
        ListControl[ List ].current = ListControl[ List ].current->next ;

    New->next = ListControl[ List ].current->next;
    ListControl[ List ].current->next = New;

#ifdef USE_LASTPTR
    /* advance last node pointer if needed
    */
    if( NULL != ListControl[ List ].last->next->next )
        ListControl[ List ].last = ListControl[ List ].last->next ;

#endif

    return NO_PROBLEMS;
}

int LLSnodePrependFrom( int List, void * Source )
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

    /* fill node with data
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );
    New->next = ListControl[ List ].first->next;
    ListControl[ List ].first->next = New;

    if( NULL != New->next && NULL == New->next->next )
    {
        /* The new node is not the only node and is the node preceding
           the actual last node.
           So it is the first of only two valid nodes.
           Note that before insertion of 'New ' .current was .first
           As was the optional last node pointer.
           Note also that this section is a consequence of using a
           'trailing' current node pointer!
        */
        ListControl[ List ].current = New ; /* == .current->next */

#ifdef USE_LASTPTR

        ListControl[ List ].last = New ; /* == .last->next */

#endif

    }

    return NO_PROBLEMS;
}

int LLSnodeAppendFrom( int List, void * Source )
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

    /* fill node with data
    */
    memcpy( & New->data, Source, ListControl[ List ].itemsize );
    New->next = NULL ;

#ifdef USE_LASTPTR

    if( NULL != ListControl[ List ].last->next )    /* non empty list ? */
        ListControl[ List ].last = ListControl[ List ].last->next ;
    ListControl[ List ].last->next = New ;

#else

    {
        struct Node * Tmp = ListControl[ List ].current;

        while( NULL != Tmp->next ) /* find last node */
            Tmp = Tmp->next ;

        Tmp->next = New ;
    }

#endif

    return NO_PROBLEMS;
}

void LLSnodeDelete( int List )
{
    {
        struct Node * Old = ListControl[ List ].current->next ;

        assert( (unsigned) List < ListCount );

        if( NULL == ListControl[ List ].current->next )
        {
            return ;  /* nothing there to delete ... */
        }

        ListControl[ List ].current->next = Old->next ;
        NODE_FREE( Old );
    }

    /* Modification to prevent current and last node pointers pointing
       past the last node of a list: adjust the current and last node
       pointers when the last node was deleted
    */
    if( NULL == ListControl[ List ].current->next )
    {
        struct Node * Tmp = ListControl[ List ].first;

        if( NULL != Tmp->next )                 /* list not empty ?     */
            while( NULL != Tmp->next->next )    /* find the node before */
                Tmp = Tmp->next ;               /* the last node        */

        ListControl[ List ].current = Tmp ;

#ifdef USE_LASTPTR

        ListControl[ List ].last = Tmp ;

#endif

    }
    return ;
}

int LLSnodeFind( int List, CompFunPtr Compare, void * DataPtr )
{                        /* FindFirst/FindNext format may be needed ... */
    int RetVal ;

    assert( (unsigned) List < ListCount );

    if( NULL == ListControl[ List ].first->next )
    {
        return 2; /* a compare usually returns just -1, 0 or 1 !!! */
    }

    if( NULL == Compare ) /* default to memcmp with .itemsize */
    {
        while( 0 != (RetVal = memcmp( DataPtr,
                                & ListControl[ List ].current->next->data,
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
                              & ListControl[ List ].current->next->data ))
               && NULL != ListControl[ List ].current->next->next )
        {
            ListControl[ List ].current=ListControl[ List ].current->next;
        }
        return RetVal ;
    }
}

/* ---- current node pointer management ------------------------------- */

int  LLSnodePtr2First( int List )
{
    assert( (unsigned) List < ListCount );

    ListControl[ List ].current = ListControl[ List ].first ;

    return NULL != ListControl[ List ].first->next ;
}

int  LLSnodePtr2Last( int List )
{
    assert( (unsigned) List < ListCount );

#ifdef USE_LASTPTR

    ListControl[ List ].current = ListControl[ List ].last ;

    return NULL != ListControl[ List ].first->next ;

#else

    /* special handling for empty list
    */
    if( NULL == ListControl[ List ].first->next )
    {
        return 0; /* .current also same as .first */
    }

    /* Let the current node pointer point to the last valid node
    */
    while( NULL != ListControl[ List ].current->next->next )
        ListControl[ List ].current = ListControl[ List ].current->next ;

    return 1;

#endif

}

int  LLSnodePtr2Next( int List )
{
    assert( (unsigned) List < ListCount );

    if( NULL == ListControl[ List ].current->next       /* empty list ? */
        || NULL == ListControl[ List ].current->next->next ) /* at end ?*/
    {
        return 0 ;             /* note: 'at end' condition added to     */
    }                          /* to prevent .current pointing past end */

    ListControl[ List ].current = ListControl[ List ].current->next ;
    return 1 ;
}

int  LLSnodePtr2Prev( int List )
{
    struct Node * Prev ;

    assert( (unsigned) List < ListCount );

    Prev = ListControl[ List ].first ;
    if( NULL == Prev->next                       /* empty list or */
        || ListControl[ List ].current == Prev ) /* at beginning? */
    {
        return 0 ;
    }

    /* Find previous Node
    */
    while( Prev->next != ListControl[ List ].current )
        Prev = Prev->next ;

    ListControl[ List ].current = Prev ;

    return 1 ;
}

/* ---- stored data management ---------------------------------------- */

int LLSnodeInt( int List )
{
    return ListControl[ List ].current->next->data;
}

long LLSnodeLong( int List )
{
    return *((long *) &ListControl[ List ].current->next->data );
}

void * LLSnodePtr( int List )
{
    return *((void **) &ListControl[ List ].current->next->data );
}

void FAR * LLSnodeFptr( int List )
{
    return *((void FAR **) &ListControl[ List ].current->next->data );
}

int LLSnodeDataTo( int List, void * Destination )
{
    if( NULL != Destination )
    {
        memcpy( Destination,
                & ListControl[ List ].current->next->data,
                ListControl[ List ].itemsize );
    }

    return ListControl[ List ].itemsize ;       /* size needed for blob */
}

/* ==== LLS.c  end ==================================================== */
