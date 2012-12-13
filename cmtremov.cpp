/*
 * CMTREMOV.CPP
 * Implementation of comment remover.
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

#include "cmtremov.h"

void CommentRemover::ProcessActions (Event theEvent)
{
    int rc = 1;

    if (theEvent == END_OF_FILE)
    rc= 1;

    switch (itsState)
    {
    case NormalInput:
        if (theEvent == FOUND_SLASH)
        {
            rc = 0;                     // Don't print it yet...
        }
        break;

    case BeginComment:
        switch (theEvent)
        {
        case FOUND_SLASH:               // Yes, it's a C++ comment...
        case FOUND_STAR:                // Yes, it's a C-style comment.
        case FOUND_BACKSLASH:           // Escaped character.
            rc = 0;
            break;

        default:                        // No, just a slash...
            print ('/');                // Print the previous slash.
            break;
        }
        break;

    case InsideEscape:
        if (itsPrevState == InCppComment || itsPrevState == InCComment)
            rc = 0;
        break;

    case InCppComment:
    case InCComment:
    case StarInCppComment:
    case StarInCComment:
        rc = 0;
        break;
    }

    if (theEvent == FOUND_NL)       // Newline can be printed always.
    {
        rc = 1;
    }

    if (rc)
        PrintChar();
}

int   main (int argc, char **argv)
{
    CommentRemover CommRemv;

    if (0 == CommRemv.Init (argc, argv))
    {
        fprintf (stderr, "USAGE: CMTREMOV InFile [OutFile]\n");
    }
    else
    {
        CommRemv.Run();
        CommRemv.Uninit();

        fprintf (stderr, "\nOK! %lu lines processed. Last state = %d\n",
            CommRemv.GetLines(), CommRemv.GetLastState()
        );
    }

    return 0;
}
