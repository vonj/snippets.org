/* =======================================================================
    LL_DEMO.C   Demonstration of the use of my Linked List modules.
                Intentionally over-commented w.r.t the List stuff and
                under-commented w.r.t. the rest.
                Not all LLS functions are used, especially LLSnodeDelete()
                is not used. But I've tried to use all the most important
                functions. Even when this resulted in less than optimal
                functionality. E.g. the use of LLSnodeInsert for inserting
                the data for a directory in the list.

                All subdirectories in the current directory are displayed
                first sorted by name. Then all files sorted by date and
                time.
                The sorting is done by inserting a new node at the
                appropriate point in the list. This is not the most
                efficient way to sort, but it is the only possibility with
                these Linked Lists because some info internal to the list
                is (intentionally) not available to the user. Some kind of
                tree would be better anyway ;-)

                Doubly Linked Lists should work the same here as the used
                Singly Linked Lists. Just replace any 'LLS' by 'LLD'
                preserving the case. I haven't tried it though, as their
                only advantage is speed (at the expense of space) when
                moving backwards in a list.

                v1.00  97-05-22  Initial version.
                       97-05-23  Comments added and completed.

 _____          This version is Public Domain.
 /_|__|         A.Reitsma, Delft, Nederland.
/  | \  --------------------------------------------------------------- */

#include <stdio.h>        /* for printf()                         */
#include "dirport.h"      /* for all the FIND_* and ff_* stuff    */
#include "sniptype.h"     /* for Success_                         */
#include "lls.h"          /* the Singly Linked List stuff         */

/*  Comparison functions which are to be called by LLSnodeFind
*/
int DirComp( const void * D1, const void * D2 );
int FileComp( const void * D1, const void * D2 );

/*  Separate output functions for directories and files.
    Could be one function with a conditional on ff_attr() == _A_SUBDIR
*/
void DirOutput( int List );
void FileOutput( int List );

int CompareResult; /* Kludge to solve an (intentional) limitation of    */
                   /* the Linked List system. Serves as secondary       */
                   /* output channel from the ...Comp functions to main.*/

int main( int argc , char * argv[] )
{
    DOSFileData Info;

    /*  Create and initialize two lists
    */
    int ListDirs  = LLScreate( sizeof( DOSFileData ));
    int ListFiles = LLScreate( sizeof( DOSFileData ));

    if( Success_ != FIND_FIRST( argc < 2 ? "*.*" : argv[ 1 ],
                                _A_ANY, &Info ))
    {
        return EXIT_FAILURE;
    }

    do
    {
        if( _A_SUBDIR & ff_attr( &Info ))
        {
            /*  It is a directory but we'll ignore the '.' and '..' dirs
            */
            if( '.' == ff_name( &Info )[ 0 ])
                continue;

            /*  Search from the start ...
            */
            LLSnodePtr2First( ListDirs );
            /*  ... and set the 'current node pointer' to the node
                matching -- according to the function DirComp --
                the new data.
            */
            LLSnodeFind( ListDirs, DirComp, &Info );
            /*  LLSnodeFind has a problem in that it is basically designed
                to find EXACT matches. And DirComp says it is a match when
                it is the current node is 'larger' than the new data.
                Also -- unless the list is empty -- the design of the list
                system is such that the current node pointer can NEVER
                point at an empty (dummy) node. This means we can NEVER
                use LLSnodeInsert() to insert a node as the last element
                of the list.
                Resulting in the following if/else construct.
            */
            if( 0 <= CompareResult )
                LLSnodeInsert( ListDirs, Info );
            else
                LLSnodeAdd( ListDirs, Info );
            /*  Now note that Info is put directly on the stack. This
                _should_ cause a warning, but you can safely ignore it.
            */
        }
        else if( _A_VOLID & ff_attr( &Info ))
        {
            /*  Do nothing with volume label */
        }
        else
        {   /*  It is a file. Deal with it in a similar way as a
                directory. The major difference is the way the data
                is inserted in the list. Note that the difference is
                for demonstration purposes!
            */
            LLSnodePtr2First( ListFiles );
            LLSnodeFind( ListFiles, FileComp, &Info );
            if( 0 <= CompareResult )
                LLSnodeInsertFrom( ListFiles, &Info );
            else
                LLSnodeAddFrom( ListFiles, &Info );
            /*  In this case a _pointer_ to the info is put on the stack.
                This is -- in this case -- more efficient.
                With data sizes less than the size of a pointer the direct
                method -- the way a directory is handled earlier -- is
                both faster and smaller.
            */
        }
    }while( Success_ == FIND_NEXT( &Info ));

    FIND_END( &Info );

    /*  Output the content of the List with directories,
        then get rid of it.
    */
    DirOutput( ListDirs);
    LLSdelete( ListDirs );

    /*  And similar for the files.
    */
    FileOutput( ListFiles );
    LLSdelete( ListFiles );

    return EXIT_SUCCESS;
}

int DirComp( const void * D1, const void * D2 )
{
    int RetVal = strcmp( ff_name( (DOSFileData *) D2 ),
                         ff_name( (DOSFileData *) D1 ));

    CompareResult = RetVal;

    /*  Return a 'match' (0) if the item in the list is 'larger' than the
        new data.
    */
    return RetVal >= 0 ? 0 : 1 ;
}

int FileComp( const void * D1, const void * D2 )
{
    /*  Date and time structs are transformed into 'normal' ints by
        casting. This is not really portable to non-DOS systems ...
    */
    int RetVal = (int)ff_date( (DOSFileData *) D2 ) -
                 (int)ff_date( (DOSFileData *) D1 );

    if( 0 == RetVal )
        RetVal = (int)ff_time( (DOSFileData *) D2 ) -
                 (int)ff_time( (DOSFileData *) D1 );

    CompareResult = RetVal;

    /*  Return a 'match' (0) if the item in the list is 'larger' than the
        new data.
    */
    return RetVal >= 0 ? 0 : 1 ;
}

void DirOutput( int List )
{
    /*  In this case we KNOW the type of the data for certain.
        Otherwise we'd use a pointer and malloc sufficient memory.
        E.g. by:
            char * Sufficient = malloc( LLSnodeDataTo( List, NULL );
        Note the NULL! It says: "return only the size".
        Later on we'd use "Sufficient" in stead of "&Data".
    */
    DOSFileData Data;

    /*  From the start of the list ...
    */
    if( ! LLSnodePtr2First( List ))
    {
        return; /* list is empty */
    }

    do
    {
        /* ... transfer the data the current node points at to 'Data' ...
        */
        LLSnodeDataTo( List, &Data );
        /*  ... then output it.
        */
        printf( "%-14.14s  %10.10s  "
                "%4.4d-%2.2d-%2.2d  "
                "%2.2d:%2.2d:%2.2d  "
                "%2.2x\n",
                ff_name( &Data ), "<<dir>>",
                1980 + ff_yr( &Data ), ff_mo( &Data ), ff_day( &Data ),
                ff_hr( &Data ), ff_min( &Data ), 2 * ff_sec( &Data ),
                ff_attr( &Data ));
        /*  And go to the next node if there is one.
        */
    }while( LLSnodePtr2Next( List ));
}

void FileOutput( int List ) /* Nearly identical to DirOutput */
{
    DOSFileData Data;

    if( ! LLSnodePtr2First( List ))
    {
        return; /* list is empty */
    }

    do
    {
        LLSnodeDataTo( List, &Data );
        printf( "%-14.14s  %10.1ld  "
                "%4.4d-%2.2d-%2.2d  "
                "%2.2d:%2.2d:%2.2d  "
                "%2.2x\n",
                ff_name( &Data ), ff_size( &Data ),
                1980 + ff_yr( &Data ), ff_mo( &Data ), ff_day( &Data ),
                ff_hr( &Data ), ff_min( &Data ), 2 * ff_sec( &Data ),
                ff_attr( &Data ));

    }while( LLSnodePtr2Next( List ));
}

/* === LL_demo.c ======================================================= */
