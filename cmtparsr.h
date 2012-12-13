/*
 * CMTPARSR.H
 * Base class: comment parser.
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

#ifndef _CMTPARSR_H_
#define _CMTPARSR_H_

#include <stdio.h>

class CommentParser
{
public:
    enum State {
        NormalInput,
        InsideString,
        InsideChar,
        BeginComment,
        InsideEscape,
        InCppComment,
        InCComment,
        StarInCppComment,
        StarInCComment
    };

    enum Event {
        ANY_CHAR,
        FOUND_BACKSLASH,
        FOUND_NL,
        FOUND_QUOTE,
        FOUND_SINGLEQUOTE,
        FOUND_SLASH,
        FOUND_STAR,
        FOUND_WHITESPACE,
        END_OF_FILE
    };

    CommentParser (State InitState = NormalInput)
        : itsState(InitState)
        , lines(1)
    { }

    virtual ~CommentParser() { }

    int Init (int argc, char **argv);
    int Init (char *file);

    void Uninit();

    int Run();

    unsigned long GetLines() { return lines - 1; };

    int GetLastState() { return itsState; };

protected:
    virtual void ProcessState (Event theEvent);

    virtual void ProcessActions (Event theEvent) = 0;

    void print (char ch)
    {
        fputc (ch, OutFile);
    }

    void print (char *str)
    {
        fputs (str, OutFile);
    }

    void ChangeChar (int ch)
    {
        ReadCh = ch;
    }

    int ReadChar();
    void PrintChar();
    void PrintLineNumber();

    Event GetEvent();

    State   itsState;
    State   itsPrevState;

private:
    unsigned long  lines;
    int     arguments;
    int     ReadCh;
    FILE   *InFile,
           *OutFile;
};

#endif // _CMTPARSR_H_

