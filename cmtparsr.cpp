/*
 * CMTPARSR.CPP
 * Implementation of the comment parser.
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

#include <string.h>

#include "cmtparsr.h"

static  char    CannotOpen[] = "Cannot open %s for %sput\n\n";

/* CommentParser::Init
 *   Initialization function which takes the program's command line
 *   arguments and opens the input file and optionally the output file.
 */
int CommentParser::Init (int argc, char **argv)
{
    if (argc < 2)
    {
        return 0;
    }

    if (0 == Init (argv[1]))
    {
        return 0;
    }

    if (argc == 3)
    {
        if (stricmp (argv[1], argv[2]) == 0)
        {
            // input and output files cannot be the same,
            // output goes to stdout instead.
            OutFile = stdout;
        }
        else if ((OutFile = fopen (argv[2], "w")) == NULL)
        {
            fprintf (stderr, CannotOpen, argv[2], "out");
            // if cannot open, output goes to stdout instead.
            OutFile = stdout;
        }
    }

    arguments = argc;

    return 1;
}

/* CommentParser::Init
 *   Initialization function which opens the input file.
 *   Action processors that don't need output file can use this function.
 */
int CommentParser::Init (char *file)
{
    OutFile = stdout;

    if ((InFile = fopen (file, "r")) == NULL)
    {
        fprintf (stderr, CannotOpen, file, "in");
        return 0;
    }

    arguments = 2;

    return 1;
}

/* CommentParser::Uninit
 *  Closes input and output files.
 */
void CommentParser::Uninit()
{
    if (arguments == 3)
        fclose (OutFile);
    fclose (InFile);

    fflush (stdout);
}

/* CommentParser::ProcessState
 *  The heart of the comment parser. A finite state machine which does
 *  nothing else than determines the current state based on the events.
 */
void CommentParser::ProcessState (Event theEvent)
{
    if (itsState != InsideEscape)
        itsPrevState = itsState;

    if (theEvent == FOUND_BACKSLASH)           // Escaped character.
    {
        itsState = InsideEscape;
        return;
    }

    switch (itsState)
    {
    case NormalInput:
        switch (theEvent)
        {
        case FOUND_QUOTE:
            itsState = InsideString;
            break;

        case FOUND_SINGLEQUOTE:
            itsState = InsideChar;
            break;

        case FOUND_SLASH:
            itsState = BeginComment;  // to be investigated: begin of comment.
            break;
        }
        break;

    case InsideString:
        switch (theEvent)
        {
        case FOUND_QUOTE:
            itsState = NormalInput;     // End of the string.
            break;
        }
        break;

    case InsideChar:
        switch (theEvent)
        {
        case FOUND_SINGLEQUOTE:
            itsState = NormalInput;     // End of the character constant.
            break;
        }
        break;

    // We have found a '/', maybe this is a comment...
    case BeginComment:
        switch (theEvent)
        {
        case FOUND_SLASH:
            itsState = InCppComment;    // Yes, it's a C++ comment.
            break;

        case FOUND_STAR:
            itsState = InCComment;      // Yes, it's a C comment.
            break;

        default:
            itsState = NormalInput;     // No, just a slash.
            break;
        }
        break;

    case InsideEscape:
        itsState = itsPrevState;  // Found backslash -- restore previous state.
        break;

    case InCppComment:
        switch (theEvent)
        {
        case FOUND_NL:
            itsState = NormalInput;     // Newline is end of C++ comment.
            break;

        case FOUND_STAR:
            itsState = StarInCppComment;
            break;
        }
        break;

    case InCComment:
        switch (theEvent)
        {
        case FOUND_STAR:
            itsState = StarInCComment;  // to be investigated: end of comment.
            break;
        }
        break;

    // '*' in C++ comment: action processors can use this to check
    // e.g. C comments inside C++ comment.
    case StarInCppComment:
        switch (theEvent)
        {
        case FOUND_STAR:
            itsState = StarInCppComment; // another '*' -- don't change state.
            break;

        default:
            itsState = InCppComment;
            break;
        }
        break;

    // We are inside a C comment, and there is a '*';
    // maybe this is the end of the comment...
    case StarInCComment:
        switch (theEvent)
        {
        case FOUND_STAR:
            itsState = StarInCComment;  // another '*' -- don't change state.
            break;

        case FOUND_SLASH:
            itsState = NormalInput;     // Yes, it's end of the C comment.
            break;

        default:
            itsState = InCComment;      // No, we are still in C comment.
            break;
        }
        break;
    }
}

/* CommentParser::GetEvent
 *  Selects the next event according to the read character.
 */
CommentParser::Event CommentParser::GetEvent()
{
    switch (ReadCh)
    {
    case ' ':
    case '\t':
        return FOUND_WHITESPACE;

    case '\"':
        return FOUND_QUOTE;

    case '\'':
        return FOUND_SINGLEQUOTE;

    case '\\':
        return FOUND_BACKSLASH;

    case '\n':
        lines++;          // count processed lines, if someone needs it...
        return FOUND_NL;

    case '/':
        return FOUND_SLASH;

    case '*':
        return FOUND_STAR;

    default:
        return ANY_CHAR;
    }
}

/* CommentParser::ReadChar
 *  Read character from input file.
 */
int CommentParser::ReadChar()
{
    ReadCh = fgetc (InFile);
    return ReadCh;
}

/* CommentParser::PrintChar
 *  Print current character which can be the last read from the input file
 *  or changed by the action processor.
 */
void CommentParser::PrintChar()
{
    if (ReadCh != EOF)
        fputc (ReadCh, OutFile);
}

/* CommentParser::PrintLineNumber
 *  Print current line number.
 */
void CommentParser::PrintLineNumber()
{
    fprintf (OutFile, "\n%05lu:\n", lines);
}

/* CommentParser::Run
 *  Loop that reads the input file and calls the action processor and
 *  the state processor.
 */
int CommentParser::Run()
{
    Event theEvent;

    while (ReadChar() != EOF)
    {
        theEvent = GetEvent();
        ProcessActions (theEvent);
        ProcessState (theEvent);
    }

    // Some action processors may need to do some extra processing
    // based on the state of the FSM after the file is processed.
    ProcessActions (END_OF_FILE);

    return 0;
}

