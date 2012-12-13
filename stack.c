/* =======================================================================
    STACK.c     Stack manager.
                A.Reitsma, Delft, Nederland.

                v0.25  94-07-03  Public Domain.

    Many times I wanted to have one or more stacks for the program I was
    working on. Sometimes I did without -- especially on Quick and Dirty
    utilities. At other times I cobbled up something that more or less
    seemed to work. Crashes were common ...
    So I decided to take the time to do something about it.
    This is the result.

 -  The Stack system is based on dynamic arrays: automatically growing
    the stacks when needed. The growing is done in steps of 16 items.
    This reduces the number of required memory reallocations (and
    therefore the number of allocation failures).
 -  Stack shrinking is intentionally not implemented.
 -  ItemSizes larger than 2kB are not supported: the minimum of initial
    StackItems per stack is currently 32.
    An assert (debug version) limits ItemSizes: <= 1kB and >= 2 Bytes.
    For large data items a separate 'user' module is probably sensible.
    StackUnpop is added to take a look at info, especially size info so
    an appropriate amount of memory can be allocated by the user.
 -  A certain amount of redundancy is provided to enable integrity checks
    for use in programs with 'wild pointer' problems.
 -  Compile with NDEBUG defined for a 'production' version.
    Compile with NDEBUG _not_ defined for a debug version.
 -  Some functions benefit by using the compilers optimizer.
    Especially 'Common Sub-expression Optimizations'.
 -  Some items should be unsigned in stead of int. Not a real problem.
    Detected by checking the appropriateness and ranges of types.
 -  In some places an int is cast to unsigned, then tested for a 'large'
    value including 'negative' values. This may be non-portable.
 -  Not checked for potential alignment problems. I don't think there are.
___-------------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>           /* memcpy() */
#include <assert.h>           /* debugging */
#include "stack.h"
#include "stk_defs.h"         /* MALLOC, CALLOC, FREE (re-)definitions  */
                              /* and other debugging re-def's with a    */
                              /* special debugging version.             */

#define MEMORY          -1    /* Preparations/replacements for GP Error */
#define Error(x)        (x)   /* manager. Error may terminate program.  */

#define STACK_GROWTH    16

struct StackHead
{
    char * base ;        /* base address for the stack (char * because  */
                         /*     adding offset to void * doesn't work.)  */
    unsigned offset ;    /* offset of data from base in bytes           */
    int top ;            /* offset of data from base in items           */
    int itemsize ;       /* itemsize * top should equal to offset       */
    int max ;            /* max < top is an error                       */
    int underflow ;      /* flag. Incremented when Pops exceed Pushes.  */
};

/*  --- local data and prototypes ------------------------------------- */

static struct StackHead * StackArray ; /* initialised: StackSystemInit  */
                                       /* modified by: StackCreate      */
static int StackCount ;                /* same. Used by management only */
static int StackInit( int Stack, int Itemsize );

/* ---- Management operations ----------------------------------------- */

int StackSystemInit( int StackCountAdditional )
{
    assert( (unsigned) StackCountAdditional <= 20 );
                 /* negative is logic error (cast => neg. is large int) */
                 /* higher than 20 is ridiculous for an initial setup   */

    StackCountAdditional += STACK_COUNT_DEFAULT ;

    /* Create and initialize stack 'descriptor' array. A zero value
       for all bytes of a pointer is assumed to be a NULL pointer.
    */
    StackArray = CALLOC( StackCountAdditional, struct StackHead );
    if( NULL == StackArray )
    {
        return Error( MEMORY );
    }

    /* Initialize each default stack.
    */
    if( StackInit( STACK_INT, sizeof( int )))
    {
        return Error( MEMORY );
    }

    if( StackInit( STACK_LONG, sizeof( long )))
    {
        return Error( MEMORY );
    }

    if( StackInit( STACK_PTRS, sizeof( char * )))
    {
        return Error( MEMORY );
    }

    if( StackInit( STACK_FAR_PTRS, sizeof( char FAR * )))
    {
        return Error( MEMORY );
    }

    StackCount = StackCountAdditional ;
    return StackCount ;
}

static int StackInit( int Stack, int Itemsize )
{
    StackArray[ Stack ].itemsize  = Itemsize ;
    StackArray[ Stack ].max       = STACK_GROWTH * 2 ;
    StackArray[ Stack ].top       = 0 ;
    StackArray[ Stack ].offset    = 0 ;
    StackArray[ Stack ].underflow = 0 ;

    StackArray[ Stack ].base = MALLOC( STACK_GROWTH *2 * Itemsize, char );

    if( NULL == StackArray[ Stack ].base )
    {
        return MEMORY ;
    }

    return NO_PROBLEMS ;
}

int StackCreate( int ItemSize )
{
    int Stack ;

    assert( (unsigned) ItemSize-2 <= 1022 );
                     /* Not too small, too large or negative please !!! */

    /* Look for empty slot
    */
    for( Stack = 0; Stack < StackCount; Stack++ )
    {
        if( NULL == StackArray[ Stack ].base )
            break;
    }

    /*  What if NO EMPTY slot ???
    */
    if( Stack == StackCount )
    {
        struct StackHead * tmp ;         /* StackArray expansion needed */

        tmp = realloc( StackArray,
                       (StackCount + 1) * sizeof( struct StackHead ));
        if( NULL == tmp )
        {
            return Error( MEMORY );
        }

        StackArray = tmp ;
        StackCount ++ ;
    }

    if( StackInit( Stack, ItemSize ))
    {
        return Error( MEMORY );
    }

    return Stack ;
}

void StackDelete( int Stack )
{
    if( StackCheck( Stack ) < STACK_ERRORS )  /* OK if only warning */
    {
        FREE( StackArray[ Stack ].base );
        StackArray[ Stack ].base = NULL ;
    }
    return ;
}

int StackAdapt( int Stack, int Free )
{
   assert( (unsigned) Stack < StackCount );    /* validate stack number */

   assert( (unsigned) Free <= 4 * STACK_GROWTH );
                       /* Negative and large numbers are ridiculous ... */
                       /* assuming casted negative int = large unsigned */

   while( StackArray[ Stack ].top + Free >= StackArray[ Stack ].max )
   {
        char * tmp ;    /* resize Stack in steps */

        tmp = realloc( StackArray[ Stack ].base,
                       StackArray[ Stack ].itemsize
                        * (StackArray[ Stack ].max + STACK_GROWTH ));
        if( NULL == tmp )
        {
            return MEMORY ;
        }

        StackArray[ Stack ].max += STACK_GROWTH ;
        StackArray[ Stack ].base = tmp;
   }

   return NO_PROBLEMS ;
}

int StackCheck( int Stack )      /* Check for possible problems in more */
{                                /* or less decreasing severity         */
    if( (unsigned) Stack >= StackCount )
    {
        return STACK_INV_NUMBER ;
    }

    if( NULL == StackArray[ Stack ].base )
    {
        return STACK_NULL ;
    }

    if( StackArray[ Stack ].top > StackArray[ Stack ].max )
    {
        return STACK_CORRUPT2 ;
    }

    if( StackArray[ Stack ].top * StackArray[ Stack ].itemsize
         != StackArray[ Stack ].offset )
    {
        return STACK_CORRUPT1 ;
    }

    if( 0 != StackArray[ Stack ].underflow )
    {
        StackArray[ Stack ].underflow = 0; /* reset underflow flag */
        return STACK_UNDERFLOW ;
    }

    if( StackArray[ Stack ].top == StackArray[ Stack ].max )
    {
        return STACK_LIMIT_REACHED ;
    }

    if( 0 == StackArray[ Stack ].top )
    {
        return STACK_EMPTY ;
    }

    return NO_PROBLEMS ;
}

void StackUnpop( int Stack ) /* Reverse a pop. Data is still present!   */
{                            /* very useful for implementing stacks     */
                             /* with variable sized items ...           */

    if( StackArray[ Stack ].top < StackArray[ Stack ].max )
    {
        StackArray[ Stack ].offset += StackArray[ Stack ].itemsize ;
        StackArray[ Stack ].top ++ ;
    }
    return ;
}

/* ---- Generic push/pop operations ----------------------------------- */

int Push( int Stack, void * Source )
{
    if( NO_PROBLEMS != StackAdapt( Stack, 1 ))
    {
        return MEMORY;
    }

    memcpy( StackArray[ Stack ].base + StackArray[ Stack ].offset,
            Source,
            StackArray[ Stack ].itemsize );

    StackArray[ Stack ].offset += StackArray[ Stack ].itemsize ;
    StackArray[ Stack ].top ++ ;

    return NO_PROBLEMS ;
}

void Pop( int Stack, void * Destination )
{
    if( 0 != StackArray[ Stack ].top )  /* don't ACTUALLY underflow ... */
    {
        StackArray[ Stack ].offset -= StackArray[ Stack ].itemsize ;
        StackArray[ Stack ].top -- ;
    }
    else
        StackArray[ Stack ].underflow ++ ;

    memcpy( Destination,
            StackArray[ Stack ].base + StackArray[ Stack ].offset,
            StackArray[ Stack ].itemsize );
    return ;
}

/* ---- Push/pop operations to/from default stacks -------------------- */

int PushInt( int Value )
{
    if( NO_PROBLEMS != StackAdapt( STACK_INT, 1 ))
    {
        return MEMORY;
    }

    * ((int *) ( StackArray[ STACK_INT ].base
                  + StackArray[ STACK_INT ].offset )) = Value ;

    StackArray[ STACK_INT ].offset += sizeof( int );
    StackArray[ STACK_INT ].top ++ ;

    return NO_PROBLEMS;
}

int PopInt( void )
{
    if( 0 != StackArray[ STACK_INT ].top )
    {
        StackArray[ STACK_INT ].offset -= sizeof( int );
        StackArray[ STACK_INT ].top -- ;
    }
    else
        StackArray[ STACK_INT ].underflow ++ ;

    return * ((int *) ( StackArray[ STACK_INT ].base
                         + StackArray[ STACK_INT ].offset ));
}

int PushLong( long Value )
{
    if( NO_PROBLEMS != StackAdapt( STACK_LONG, 1 ))
    {
        return MEMORY;
    }

    * ((long *) ( StackArray[ STACK_LONG ].base
                   + StackArray[ STACK_LONG ].offset )) = Value ;

    StackArray[ STACK_LONG ].offset += sizeof( long );
    StackArray[ STACK_LONG ].top ++ ;

    return NO_PROBLEMS;
}

long PopLong( void )
{
    if( 0 != StackArray[ STACK_LONG ].top )
    {
        StackArray[ STACK_LONG ].offset -= sizeof( long );
        StackArray[ STACK_LONG ].top -- ;
    }
    else
        StackArray[ STACK_LONG ].underflow ++ ;

    return * ((long *) ( StackArray[ STACK_LONG ].base
                          + StackArray[ STACK_LONG ].offset ));
}

int PushPtr( void * Value )
{
    if( NO_PROBLEMS != StackAdapt( STACK_PTRS, 1 ))
    {
        return MEMORY;
    }

    * ((char **) ( StackArray[ STACK_PTRS ].base
                   + StackArray[ STACK_PTRS ].offset )) = Value ;

    StackArray[ STACK_PTRS ].offset += sizeof( char * );
    StackArray[ STACK_PTRS ].top ++ ;

    return NO_PROBLEMS;
}

void * PopPtr( void )
{
    if( 0 != StackArray[ STACK_PTRS ].top )
    {
        StackArray[ STACK_PTRS ].offset -= sizeof( char * );
        StackArray[ STACK_PTRS ].top -- ;
    }
    else
        StackArray[ STACK_PTRS ].underflow ++ ;

    return * ((char **) ( StackArray[ STACK_PTRS ].base
                           + StackArray[ STACK_PTRS ].offset ));
}

int PushFptr( void FAR * Value )
{
    if( NO_PROBLEMS != StackAdapt( STACK_FAR_PTRS, 1 ))
    {
        return MEMORY;
    }

    * ((char FAR **) ( StackArray[ STACK_FAR_PTRS ].base
                        + StackArray[ STACK_FAR_PTRS ].offset )) = Value ;

    StackArray[ STACK_FAR_PTRS ].offset += sizeof( char FAR * );
    StackArray[ STACK_FAR_PTRS ].top ++ ;

    return NO_PROBLEMS;
}

void FAR * PopFptr( void )
{
    if( 0 != StackArray[ STACK_FAR_PTRS ].top )
    {
        StackArray[ STACK_FAR_PTRS ].offset -= sizeof( char FAR * );
        StackArray[ STACK_FAR_PTRS ].top -- ;
    }
    else
        StackArray[ STACK_FAR_PTRS ].underflow ++ ;

    return * ((char FAR **) ( StackArray[ STACK_FAR_PTRS ].base
                               + StackArray[ STACK_FAR_PTRS ].offset ));
}

/* === STACK.c end ==================================================== */
