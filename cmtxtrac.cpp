/*
 * CMTXTRAC.CPP
 * Implementation of comment extractor.
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

#include "cmtxtrac.h"

void CommentExtractor::ProcessActions (Event theEvent)
{
    int rc = 0;

    // This flag is needed when comment starting is continued in next line:
    //   /\
    //   * comment */
    // The BeginComment state is still on after InsideEscape state and
    // without flag the slash would be printed twice.
    static int SlashPrinted = 0;

    switch (itsState)
    {
    case NormalInput:
        if (printWhiteSpace && theEvent == FOUND_WHITESPACE)
            rc = 1;                     // Print whitespace.
        break;

    case BeginComment:
        switch (theEvent)
        {
        case FOUND_SLASH:               // Yes, it's a C++ comment.
        case FOUND_STAR:                // Yes, it's a C comment.
        case FOUND_BACKSLASH:           // Escaped character.
            rc = 1;
            if (! SlashPrinted)
            {
                if (printLineNumbers)
                {
                    PrintLineNumber();
                }

                // Comment starting slash was not yet printed, so print it now.
                print ('/');
                SlashPrinted = 1;
            }
            else
                SlashPrinted = 0;

            break;
        }
        break;

    case InsideEscape:
        if (itsPrevState == InCppComment || itsPrevState == InCComment)
            rc = 1;
        break;

    case StarInCComment:
        if (theEvent == FOUND_SLASH)
            SlashPrinted = 0;           // End of comment, clear flag.
        rc = 1;
        break;

    case InCppComment:
        if (theEvent == FOUND_NL)
            SlashPrinted = 0;           // End of comment, clear flag.
        rc = 1;
        break;

    case InCComment:
    case StarInCppComment:
        rc = 1;
        break;
    }

    if (theEvent == FOUND_NL)           // Newline can be printed always.
    {
        rc = 1;
    }

    if (rc)
        PrintChar();
}

int   main (int argc, char **argv)
{
    CommentExtractor CommExtract;

    if (0 == CommExtract.Init (argc, argv))
    {
        fprintf (stderr, "USAGE: CMTREMOV InFile [OutFile]\n");
    }
    else
    {
        CommExtract.WhiteSpace (0);
        CommExtract.LineNumbers (1);
        CommExtract.Run();
        CommExtract.Uninit();

        fprintf (stderr, "\nOK! %lu lines processed. Last state = %d\n",
            CommExtract.GetLines(), CommExtract.GetLastState()
        );
    }

    return 0;
}
