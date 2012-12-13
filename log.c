/************************************************************************
* LOG.C - Creates a ASCII file with time and date stamps for logging    *
*         hours.                                                        *
*                                                                       *
*   usage: LOG [IN][OUT][CALC]                                          *
*               IN - Creates an opening entry from which a "time spent" *
*                    is calculated.                                     *
*              OUT - Creates a closing entry and calculates             *
*                    "time spent" for that entry.                       *
*             CALC - Calculates total overall "time spent" for the      *
*                    entire log.                                        *
*                                                                       *
*   NOTES:  I used seconds to do all the calculations. The other        *
*           time/date entries are for human readability. Some           *
*           enhancements can be done to this program.                   *
*           i.e. Wage/Pay calculation, closing the log after a CALC     *
*           to insure log is not reused, tracking hours for individual  *
*           people, tracking hours for individual projects, etc.        *
*                                                                       *
*  Public domain by Robert Sprawls.                                     *
************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

/* Define time file constants   */

#define HOUR        3600        /* Number of seconds in an hour.        */
#define MINS        60          /* Number of seconds in a minute        */
#define IN_ENTRY    40          /* Size of an IN entry                  */
#define SEC_OFF     4           /* Offset of seconds in an IN entry     */
#define HOUR_OFF    64          /* Offset of seconds in an "time spent" */
                                /* calculated entry.                    */

/* Define values returned to DOS */

#define OK          0   /* Executed normally, nothing will be echoed */
#define OPENLOG     1   /* Attempted to log in while open entry in log */
#define CLOSEDLOG   2   /* Attempted to log out while closed entry in log */
#define FILE_ERROR  3   /* File access error. Just in case. */
#define SEEK_ERROR  4   /* File positioning error */
#define NOPARMS     5   /* No parameters supplied to program */
#define INVALID     6   /* Invalid parameters */

void    usage( void );
long    get_in_entry( FILE * );
void    fastforw( FILE * );     /* Opposite of rewind();        */
void    quit( int );

char    strbuf[ IN_ENTRY + 1 ];
FILE    *wrklog;

int main( int argc, char *argv[] )
{
    char    outline[ IN_ENTRY * 2 + 1 ];
    long    in_entry_time = 0, total_seconds = 0;
    time_t  current_time;
    div_t   hdiv, mdiv;

    if( argc < 2 )
    {
        usage();
        quit( NOPARMS );
    }

    /* Open log. Can be any directory.  */
    if(( wrklog = fopen( "WORK.LOG", "a+" )) == NULL )
        quit( FILE_ERROR );

    strupr( argv[ 1 ] );

    time( &current_time );

    /* Create an opening IN entry.  */
    if( strcmp( "IN", argv[ 1 ] ) == 0 )
    {
        /* Make sure there isn't a open entry already.  */
        if( get_in_entry( wrklog ) )
            quit( OPENLOG );

        /* Stamp it.    */
        fprintf( wrklog, "%3s %ld %s", argv[ 1 ], current_time,
            ctime( &current_time ));
    }
    /* Create a closing OUT entry.  */
    else if( strcmp( "OUT", argv[ 1 ] ) == 0 )
    {
        /* Make sure there is a previous IN entry.  */
        if( ( in_entry_time = get_in_entry( wrklog )) == 0 )
            quit( CLOSEDLOG );

        total_seconds = current_time - in_entry_time;
        sprintf( outline, "%3s %ld %s", argv[ 1 ], current_time,
            ctime( &current_time ));

        /* Cut off the newline character that's normally put on.    */
        outline[ strlen( outline ) - 1 ] = '\0';
        hdiv = div( total_seconds, HOUR );
        mdiv = div( hdiv.rem, MINS );

        sprintf( strbuf, "     Time Spent: %02d:%02d:%02d/%ld\n\n",
            hdiv.quot, mdiv.quot, mdiv.rem, total_seconds );
        strcat( outline, strbuf );
        fprintf( wrklog, outline );
    }
    /* Calculate the overall "time spent"   */
    else if( strcmp( "CALC", argv[ 1 ] ) == 0 )
    {
        rewind( wrklog );
        while( !feof( wrklog ) )
        {
            /* This is to eliminate garbage or residual entries.    */
            outline[ 0 ] = '\0';

            fgets( outline, 80, wrklog );
            if( strstr( outline, "OUT" ) != NULL )
            {
                total_seconds += atol( &outline[ HOUR_OFF ] );
            }

        }

        /* goto to end of file and stamp total hours    */
        fastforw( wrklog );
        if( total_seconds )
        {
            hdiv = div( total_seconds, HOUR );
            mdiv = div( hdiv.rem, MINS );
            fprintf( wrklog, "\t\t\t\t\t\t\t\t\t\t  "
                "Total Hours: %02d:%02d:%02d/%ld\n",
                hdiv.quot, mdiv.quot, mdiv.rem, total_seconds );
        }
    }
    else
    {
        usage();
        quit( INVALID );
    }

    quit( OK );
    return 0;
}

void usage( void )
{
    printf( "\nusage: LOG [IN][OUT][CALC]\n");
}

/****************************************************************
* get_in_entry - gets a previous IN entry.                      *
*                                                               *
*  Params: FILES *fp - file pointer.                            *
* Returns: The entry's seconds if successful, else 0            *
*                                                               *
* NOTES: If fseek fails for any reason, function does not       *
*        return. Instead, quit is call with the error code.     *
****************************************************************/

long get_in_entry( FILE *fp )
{

    if( fseek( fp, -IN_ENTRY, SEEK_END ) != 0 )
        quit( SEEK_ERROR );

    fread( strbuf, 1, IN_ENTRY, fp );
    fastforw( fp );

    if( strstr( strbuf, "IN" ) == NULL )
        return( 0 );
    else
    {
        return( atol( &strbuf[ SEC_OFF ]));
    }
}

/****************************************************************
* quit() - Program exit function. Reports of any outstanding    *
*          errors.                                              *
*                                                               *
*  Params:  errcode - Error code as defined in beginning.       *
* Returns:  nothing.                                            *
****************************************************************/

void quit( int errcode )
{
    char *errmsg[] =
    {
        "",
        "Log has an open entry.",
        "No corresponding IN entry.",
        "File open error.",
        "Seek error",
        "No parameters specified.",
        "Invalid Parameters."
    };

    printf( "\n%s\n", errmsg[ errcode ] );

    fclose( wrklog );
    exit( errcode );
}

/****************************************************************
* fastforw() - Puts file pointer to end of file.                *
*                                                               *
*  Params: fp - File pointer.                                   *
* Returns: nothing.                                             *
****************************************************************/

void fastforw( FILE *fp )
{
    fseek( fp, 0, SEEK_END );
}
