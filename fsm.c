/* =======================================================================
    FSM.c           Finite State machines.
                    Version 0.22x, 93-08-05.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, Nederland.
/  | \  --------------------------------------------------------------- */

#include <stdlib.h>
#include "ll_defs.h"
#include "fsm.h"

/* tmp definitions until error module is finished */
#define MEMORY      -1
#define Error(x)    return x

/*  --- Local definitions and data ------------------------------------ */
struct state_info
{
    struct FSMstate_entry * table ;
    int entries ;
};

static int state ;      /* the machine's state or it's maximum in setup */
static struct state_info * info ;  /* derived info in a handier format  */

/*  --- Setup and action functions ------------------------------------ */

int FSMsetup( struct FSMstate_entry * StateEntry )
{
    struct FSMstate_entry * Entry = StateEntry ;
    struct state_info * data ;

    /*  Find 'highest' state number. Negative number is end of list.
        Gaps allowed. Must be sorted on state number in current version.
        Prepared for non-sorted version though.
    */
    state = 0 ;
    do
    {
        if( state < Entry->state )
            state = Entry->state ;

    }while( (++Entry)->state >= 0 );

    /*  Create info 'array'
    */
    if( NULL == (info = MALLOC( state+1, struct state_info )))
        Error( MEMORY );

    /*  Setup info 'array': split into sub-lists with size info.
        First entry:
    */
    data = info ;
    data->table = StateEntry ;
    data->entries = 1 ;
    /*  ... and the second and following entries:
    */
    while( (++StateEntry)->state >= 0 )
    {
        if( data->table->state == StateEntry->state )
            data->entries ++ ;          /* same state */
        else
        {                               /* new state  */
            data = info + StateEntry->state ;
            data->table = StateEntry ;
            data->entries = 1 ;
        }
    }

    state = 0 ; /* initial state for state machine */

    return 0 ;
}

int FSMaction( int Cond )
{
    int count ;
    struct FSMstate_entry * Table = info[ state ].table ;

    /*  Find matching condition in second or higher entry in table.
    */
    for( count = 1 ; count < info[ state ].entries ; count ++ )
    {
        Table ++;
        if( Cond == Table->cond )
        {
            state = Table->next ;      /* match found: adjust state,    */
                                       /* and execute non-NULL funtion. */
            if( NULL != Table->action )
                return Table->action( Cond );
            else
                return 0 ;
        }
    }

    /*  No matching condition found: use first entry as default action.
    */
    Table = info[ state ].table ;
    state = Table->next ;
    return Table->action( Cond );
}

/* === FSM.c ========================================================== */
