/* =======================================================================
    STK_BLOB.h      Stack of Blobs. Binary Large OBjects.

                    A.Reitsma, Delft, The Netherlands.
                    v0.10  94-07-03  Public Domain.

    This module is essentially an interface to the generic Stack module.
    The size of the items is however variable.
    This module is still VERY experimental.
    A separate FAR version required ???
----------------------------------------------------------------------- */


#ifndef STK_BLOB_H
#define STK_BLOB_H

int  StackBlobCreate( void );

int  PushBlob( int Stack, void * BlobSource, unsigned int BlobSize );
                /* Duplicates contents of BlobSource on a stack         */

unsigned int PopBlob( int Stack, void * BlobDestination );
                /* Returns blobsize. If BlobDestination == NULL that's  */
                /* all it does. I.e: it is NOT a real Pop.              */
                /* Not NULL: Blob data is put into the indicated space, */
                /* i.e. it is a 'real' Pop.                             */
                /* Purpose: giving caller opportunity to allocate space */

#endif
/* ==== STK_BLOB.h end ================================================ */
