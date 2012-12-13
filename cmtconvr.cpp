/*
 * CMTCONVR.CPP
 * Implementation of comment converter.
 *
 * ver 1.0, 30 Jun 1996
 *
 * Public domain by:
 *   Jari Laaksonen
 *   Arkkitehdinkatu 30 A 2
 *   FIN-33720 Tampere
 *   FINLAND
 *
 *   Fidonet : 2:221/360.20
 *   Internet: jla@to.icl.fi
 */

#include "cmtconvr.h"

void CommentConverter::ProcessActions (Event theEvent)
{
    switch (itsState)
    {
    case BeginComment:
        if (theEvent == FOUND_SLASH)    // Yes, it's a C++ comment...
        {
            ChangeChar ('*');           // ...change to C-style.
        }
        break;

    case InCppComment:
        switch (theEvent)
        {
        case END_OF_FILE:           // EOF: if we are still in C++ comment...
        case FOUND_NL:              // End of C++ comment...
            print (" */");          // ...put ending C-comment mark.
            break;
        }
        break;

    case StarInCppComment:
        // End of C comment -- add space to prevent nested C comment.
        // For example: "// /*comment*/"
        //     becomes: "/* /*comment* / */"
        if (theEvent == FOUND_SLASH)
        {
            print (' ');
        }
        break;
    }

    PrintChar();
}

int   main (int argc, char **argv)
{
    CommentConverter CommConv;

    if (0 == CommConv.Init (argc, argv))
    {
        fprintf (stderr, "USAGE: CMTCONVR InFile [OutFile]\n");
    }
    else
    {
        CommConv.Run();
        CommConv.Uninit();

        fprintf (stderr, "\nOK! %lu lines processed. Last state = %d\n",
            CommConv.GetLines(), CommConv.GetLastState()
        );
    }

    return 0;
}
