/* =======================================================================
    LLS_BLOB.c      Generic Singly Linked Lists for Binary Large OBjects.
                    Linked Lists for variable size data-items.
                    This is a 'front end' for the generic LLS module.

                    v1.00  94-08-11

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, The Netherlands.
/  | \  --------------------------------------------------------------- */

#include "ll_defs.h"
#include "lls.h"            /* the generic LLS functions ...            */
#include "lls_blob.h"       /* also includes extkword.h if necessary    */

struct BlobDesc
{
    void * data ;           /* 'data' can be obtained by LLSnodePtr() ! */
    unsigned size ;
};

#define ERR_MEMORY          -1

/* ---- LL blob system management ------------------------------------- */

int LLSblobCreate( void )
{
    return LLScreate( sizeof( struct BlobDesc ));
}

/* ---- LL blob node management --------------------------------------- */

int LLSblobInsert( int List, void * Source, unsigned Size )
{                                       /* insert _BEFORE_ current node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLSnodeInsertFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

int LLSblobAdd( int List, void * Source, unsigned Size )
{                                        /* insert _AFTER_ current node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLSnodeAddFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

int LLSblobPrepend( int List, void * Source, unsigned Size )
{                                               /* insert as first node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLSnodePrependFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

int LLSblobAppend( int List, void * Source, unsigned Size )
{                                                /* insert as last node */
    struct BlobDesc Blob ;

    Blob.size = Size ;
    Blob.data = malloc( Size );

    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, Source, Size );
    LLSnodeAppendFrom( List, & Blob );

    return LIST_NO_PROBLEMS ;
}

void LLSblobDelete( int List )
{
    struct BlobDesc Blob ;

    LLSnodeDataTo( List, & Blob );
    free( Blob.data );

    LLSnodeDelete( List );

    return ;
}

/* ---- stored data management ---------------------------------------- */

unsigned LLSblobData( int List, void * Destination )
{
    struct BlobDesc Blob ;

    LLSnodeDataTo( List, & Blob );

    if( NULL != Destination )
        memcpy( Destination, Blob.data, Blob.size );

    return Blob.size ;       /* size needed for blob */
}

/* ==== LLS_BLOB.c  end =============================================== */
