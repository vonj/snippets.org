/* =======================================================================
    STACK.h     Stack manager.
                A.Reitsma, Delft, Nederland.

                v0.25  94-07-03  Public Domain.

 -  Four default stacks are created by StackSystemInit. User stacks must
    be created by StackCreate; which may fail on insufficient memory.
 -  Initially a stack has space for 32 items and can/will be grown (auto-
    matically) in steps of 16 items as long as there is sufficient memory
    space. Therefore the maximum ItemSize is 2kB.
    In the DEBUG version -- NDEBUG not defined during compilation of
    STACKS.c -- the maximum is actually restricted to 1kB by an assert.
    A future version may have reduced initial sizes and growth factors
    for large itemsizes.
 -  Stacks do NOT decrease in size. Except by complete deletion.
 -  Pushes fail when the stack is full and the memory resizing fails.
    The return value indicates this.
 -  Underflowing Pops return the Stack's bottom value.
 -  Pushes and Pops do NOT check stack-integrity. Use StackCheck().
----------------------------------------------------------------------- */

#ifndef STACK_H
#define STACK_H

#include "extkword.h"

enum StackErrors         /* Return values for StackCheck()          */
{                        /* The highest value is reported.          */

    NO_PROBLEMS,         /* All is OK (multiple use)                */
    STACK_EMPTY,         /* stack is empty: error on next pop       */
    STACK_LIMIT_REACHED, /* stack is full: maybe error on next push */
    STACK_UNDERFLOW,     /* one or more pop's too many              */
    STACK_ERRORS,        /* Dummy to separate warnings and          */
                         /* -------- REAL errors ---------          */
    STACK_CORRUPT1,      /* stack.top > stack.max                   */
    STACK_CORRUPT2,      /* top * itemsize != offset                */
    STACK_NULL,          /* Stack deleted or not created            */
    STACK_INV_NUMBER,    /* Stack number out of range               */
};

enum StackDefault{ STACK_INT, STACK_LONG, STACK_PTRS, STACK_FAR_PTRS,
                   STACK_COUNT_DEFAULT }; /* supplied for management    */

/* ---- management operations ----------------------------------------- */

int  StackSystemInit( int StackCountAdditional );
        /* StackCountAdditional is the amount of User stacks.           */
        /* Returns actual number of stacks, or -1 on memory error.      */
        /* MUST be called before other use of Stack system.             */
        /* Also creates the four default stacks                         */

int  StackCreate( int ItemSize );    /* Returns the Stack number to use */
                                     /* or -1 on memory failure.        */
void StackDelete( int Stack );       /* You MAY delete a default stack  */

int  StackAdapt( int Stack, int Free );
        /* Grows Stack as far as possible and needed to have a Free     */
        /* number of free locations. Partial growth is possible !!!     */
        /* Returns -1 on memory error, including partial growth.        */
int  StackCheck( int Stack );
        /* Returns enum StackErrors value. Will clear underflow flag if */
        /* it is the only problem. Major purpose is debugging.          */

/* ---- Generic push/pop operations ----------------------------------- */

int  Push( int Stack, void * Source );         /* returns -1 on failure */
void Pop( int Stack, void * Destination );
        /* Stack value is NOT validated.                                */
        /* Destination buffer _MUST_ be at least of size ItemSize       */
        /* as given on StackCreate() !!! Otherwise: major SNAFU.        */
        /* These functions are not meant for the default stacks.        */

void StackUnpop( int Stack ); /* Reverse last pop. Purpose is to take a */
                              /* look and not accepting the data ...    */

/* ---- Push/pop operations to/from default stacks -------------------- */
/*      VERY destructive when the relevant Stack has been deleted !!!   */

int  PushInt( int Value );    /* non-zero return of pushes is failure!  */
int  PopInt( void );          /* Pop's return the value                 */
int  PushLong( long Value );
long PopLong( void );
int  PushPtr( void * Value );
void * PopPtr( void );
int  PushFptr( void FAR * Value );
void FAR * PopFptr( void );

#endif /* STACK_H */

/* === STACK.h end ==================================================== */
