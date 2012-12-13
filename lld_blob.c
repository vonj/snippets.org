/* =======================================================================
    LLD_BLOB.c      Generic Doubly Linked Lists for Binary Large OBjects.
                    Linked Lists for variable size data-items.
                    This is a 'front end' for the generic LLD module.

                    v1.00  94-08-11

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#include "ll_defs.h"
#include "lld.h"            /* the generic LLD functions ...            */
#include "lld_blob.h"       /* also includes extkword.h if necessary    */

struct BlobDesc
{
    void * data ;           /* 'data' can be obtained by LLDnodePtr() ! */
    unsigned size ;
};

#define ERR_MEMORY          -1

/* ---- LL blob system management ------------------------------------- */

int LLDblobCreate( void )
{
    return LLDcreate( sizeof( struct BlobDesc ));
}

/* ---- LL blob node management --------------------------------------- */

int LLDblobInsert( int List, void * Source, unsigned Size )
{                                       /* insert _BEFORE_ current node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLDnodeInsertFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

int LLDblobAdd( int List, void * Source, unsigned Size )
{                                        /* insert _AFTER_ current node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLDnodeAddFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

int LLDblobPrepend( int List, void * Source, unsigned Size )
{                                               /* insert as first node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLDnodePrependFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

int LLDblobAppend( int List, void * Source, unsigned Size )
{                                                /* insert as last node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLDnodeAppendFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

void LLDblobDelete( int List )
{
    struct BlobDesc Blob ;

    LLDnodeDataTo( List, & Blob );
    free( Blob.data );

    LLDnodeDelete( List );

    return ;
}

/* ---- stored data management ---------------------------------------- */

unsigned LLDblobData( int List, void * Destination )
{
    struct BlobDesc Blob ;

    LLDnodeDataTo( List, & Blob );

    if( NULL != Destination )
        memcpy( Destination, Blob.data, Blob.size );

    return Blob.size ;       /* size needed for blob */
}

/* ==== LLD_BLOB.c  end =============================================== */
