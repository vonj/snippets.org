/* =======================================================================
    STK_BLOB.c      Stack of Blobs. Binary Large OBjects.

                    A.Reitsma, Delft, The Netherlands.
                    v0.10  94-07-03  Public Domain.

    Stack for variable sized items. Implemented by making a stack of
    'Blob Descriptors' and duplicating the item in malloc'ed memory.
    Localizing the stack number storage required interface functions
    to the stack module.
    The Pop function has two 'modes': one just returns the size of the
    blob; the other actually pops the blob.
    WARNING: This version is not really tested!
----------------------------------------------------------------------- */

#include <string.h>
#include "stk_defs.h"
#include "stack.h"
#include "stk_blob.h"

#define ERR_MEMORY   -1
#define NO_PROBLEMS  0
#define Error(x)     (x)

struct BlobDesc
{
    void * data;
    int size;
};

int StackBlobCreate( void )
{
    return StackCreate( sizeof( struct BlobDesc ));
}

int PushBlob( int Stack, void * BlobSource, unsigned int BlobSize )
{
    struct BlobDesc Blob;

    Blob.size = BlobSize;

    /* get storage space for the blob
    */
    Blob.data = MALLOC( BlobSize, char );
    if( NULL == Blob.data )
    {
        return ERR_MEMORY ;
    }

    memcpy( Blob.data, BlobSource, BlobSize ); /* duplicate the blob */

    if( ERR_MEMORY == Push( Stack, & Blob )) /* push descriptor */
    {
        FREE( Blob.data ); /* no need to cause memory leaks ... */
        return ERR_MEMORY ;
    }

    return NO_PROBLEMS ;
}

unsigned int PopBlob( int Stack, void * BlobDestination )
{
    struct BlobDesc Blob ;

    Pop( Stack, & Blob );

    if( NULL == BlobDestination )
        StackUnpop( Stack );
    else
    {
        memcpy( BlobDestination, Blob.data, Blob.size );
        FREE( Blob.data );
    }

    return Blob.size ;
}

/* ==== STK_BLOB.c end ================================================ */
