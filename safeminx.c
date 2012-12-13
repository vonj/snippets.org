/* =======================================================================
    SAFEMINX.c  Safe Multiple Includes.

                A utility to prevent problems with multiple #include's of
                the same file. Which is a severe problem for some older
                compilers like Turbo C version 1.5 ...

                95-10-26  v1.00  Initial version

 _____          This version is Public Domain.
 /_|__|         A.Reitsma, Delft, Nederland.
/  |  \ --------------------------------------------------------------- */

#include <stdlib.h>     /* for EXIT_SUCCESS               */
#include <stdio.h>      /* for fprintf()/fopen()/fclose() */
#include <string.h>     /* for strlen etc ...             */
#include <ctype.h>      /* for isalpha()                  */
#include "dirport.h"

#ifndef FILENAME_MAX
  #define FILENAME_MAX  80
#endif
#ifndef FNAME_MAX
  #define FNAME_MAX     8+3+1
#endif

#define PROGRAM   "SAFEMINX"
#define VERSION   "v1.00"
#define AUTHOR    "A.Reitsma, Delft, The Netherlands."
#define CREATED   "95-10-26"
#define COPYRIGHT "Public Domain."

char * Syntax[] =
{
    PROGRAM "  " VERSION "  " CREATED "  " AUTHOR "  " COPYRIGHT "\n\n",
    "Usage: " PROGRAM " <destination directory name>\n\n",
    "The problem:\n",
    "Some old compilers have severe problems with multiple #include's\n",
    "of the same file. This program eliminates the problem by creating\n",
    "'include-file' wrappers in the destination directory for the\n",
    "include files in the current directory.\n",
    "The wrappers could also be edited to include items which _should_\n",
    "be in the include file according to the standard but aren't.\n",
    "E.g.: FILENAME_MAX in stdio.h or CLOCKS_PER_SEC in time.h\n\n",
    "Instructions:\n- create a new include file directory,\n",
    "- change directory to the old include file directory,\n",
    "- run this program with the new directory as parameter,\n",
    "- change your compiler's settings to use the new directory.\n",
    "(Or something to the same effect.)\n\n",
    "Files are NEVER deleted or modified!\n\n",
    "Problem permanently solved ...\n",
    NULL
};

enum Errors
{
    ERR_ARGS = 1,
    ERR_MEM,
    ERR_FOPEN,
    ERR_NODIR,
    ERR_INVDRIVE,
    ERR_DIRCURRENT,
    ERR_NOFILES,
};

main( int argc, char * argv[] )
{
    char DirCurrent[ FILENAME_MAX ];
    char DestName[ FILENAME_MAX ];
    int DestDirLen;
    DOSFileData Data;

    /* validate the arguments
    */
    if( argc < 2 )
    {
        int ix = 0;
        do
        {
            fprintf( stdout, Syntax[ ix ]);
        }while( NULL != Syntax[ ++ix ]);
        return ERR_ARGS;
    }

    /* obtain the name of the current directory
    */
    getcwd( DirCurrent, FILENAME_MAX );
    strlwr( DirCurrent );

    /* validate the destination drive and directory
    */
    strcpy( DestName, argv[ 1 ]);
    strlwr( DestName );
    if( ':' == DestName[ 1 ] )
    {
        DestName[ 2 ] = '.';
        DestName[ 3 ] = '\0';
        if( 0 != chdir( DestName ))
        {
            fprintf( stderr, "\aInvalid destination drive\n" );
            return ERR_INVDRIVE;
        }
        DestName[ 2 ] = argv[ 1 ][ 2 ];
        DestName[ 3 ] = argv[ 1 ][ 3 ];
    }
    if( 0 != chdir( argv[ 1 ]) )
    {
        fprintf( stderr, "\aInvalid destination directory\n" );
        return ERR_NODIR;
    }

    chdir( DirCurrent ); /* just in case it was changed ... */

    if( 0 == strcmp( DirCurrent, DestName ))
    {
        fprintf( stderr, "\aDestination is current directory.\n" );
        return ERR_DIRCURRENT;
    }

    /* anything to do?
    */
    if( 0 != FIND_FIRST( "*.h", _A_NORMAL, &Data ))
    {
        fprintf( stderr, "\aNo .h files in the current directory\n" );
        return ERR_NOFILES;
    }

    /* Yes, something to do: prepare for action
    */
    fprintf( stdout, "Directory: \"%s\" ---\n", DirCurrent );
    DestDirLen = strlen( argv[ 1 ]);
    memcpy( DestName, argv[ 1 ], DestDirLen+1 );
    if( '\\' != argv[ 1 ][ DestDirLen - 1 ] )
    {
        DestName[ DestDirLen ] = '\\' ;
        DestDirLen++;
    }

    do
    {
        char NameAlpha[ FNAME_MAX+1 ];
        char * NamePtr = NameAlpha;
        FILE * FileOut;

        strcpy( DestName+DestDirLen, ff_name( &Data ));

        FileOut = fopen( DestName, "r" );
        if( NULL != FileOut )
        {
            fprintf( stdout, 
                     "Not overwriting \"%s\". It already exists!\n",
                     ff_name( &Data ));
            continue;
        }
        FileOut = fopen( DestName, "w" );
        if( NULL == FileOut )
        {
            fprintf( stdout, "\aFailure opening \"%s\".\n",
                             ff_name( &Data ));
            continue;
        }

        fprintf( stdout, "Creating \"%s\"\n", DestName );

        strcpy( NameAlpha, ff_name( &Data ));
        while( '\0' != *NamePtr )
        {
            if( !isalpha( *NamePtr ))
                 *NamePtr = '_';
            NamePtr ++ ;
        }
        fprintf( FileOut,       /* fixed text: */
                "/*  %s\n"
                "    Prevention of double inclusions.\n"
                "*/\n\n"
                "#ifndef SAFE__%s\n"
                "#define SAFE__%s\n\n"
                "#include <%s\\%s>    /* 'original' */\n"
                "\n#endif\n",
                                /* parameters: */
                ff_name( &Data ),
                NameAlpha,
                NameAlpha,
                DirCurrent,
                ff_name( &Data )
               );

        fclose( FileOut );
    }while( 0 == FIND_NEXT( &Data ));
    FIND_END( &Data );

    fprintf( stdout, "--- Done ---\n" );
    return EXIT_SUCCESS;
}

/* === SAFEMINX.c end ================================================= */
