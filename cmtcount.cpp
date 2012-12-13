/*
 * CMTCOUNT.CPP
 * Implementation of comment counter.
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

#include <stdio.h>

#include "cmtcount.h"

void CommentCounter::ProcessActions (Event theEvent)
{
    if (theEvent == FOUND_NL)
    {
        if (open_comment > close_comment)
        {
            cmt_lines++;
        }
    }

    switch (itsState)
    {
    case BeginComment:
        switch (theEvent)
        {
        case FOUND_SLASH:               // Yes, it's a C++ comment.
            cpp_comments++;
            cmt_lines++;
            break;

        case FOUND_STAR:                // Yes, it's a C comment.
            open_comment++;
            cmt_lines++;
            break;
        }
        break;

    case StarInCComment:
        switch (theEvent)
        {
        case FOUND_SLASH:               // End of C comment.
            close_comment++;
            break;
        }
        break;
    }
}

int   main (int argc, char **argv)
{
    CommentCounter CommCnt;
    double Coverage;

    if (argc < 2 || 0 == CommCnt.Init (argv[1]))
    {
        fprintf (stderr, "USAGE: CMTCOUNT InFile\n");
    }
    else
    {
        CommCnt.Run();
        CommCnt.Uninit();

        printf ("\n%s:\tOpens: %03d, Closes: %03d (%s), C++ comments: %03d\n",
            argv[1],
            CommCnt.GetOpenComment(),
            CommCnt.GetCloseComment(),
            CommCnt.GetOpenComment() != CommCnt.GetCloseComment() ? "*" : " ",
            CommCnt.GetCppComments()
        );
        Coverage = 100.0 * (double)CommCnt.GetCommentLines() /
            (double)CommCnt.GetLines();
        printf ("Comments in %d out of %ld total lines (%3.1f%% coverage)\n",
            CommCnt.GetCommentLines(), CommCnt.GetLines(), Coverage);
    }

    return 0;
}
