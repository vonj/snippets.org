/*
 * CMTCOUNT.H
 * Comment counter class.
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

#ifndef _CMTCOUNT_H_
#define _CMTCOUNT_H_

#include "cmtparsr.h"

class CommentCounter : public CommentParser
{
public:
    CommentCounter()
        : CommentParser()
        , cpp_comments(0)
        , open_comment(0)
        , close_comment(0)
        , cmt_lines(0)
    { }

    int GetCppComments()  { return cpp_comments; }
    int GetOpenComment()  { return open_comment; }
    int GetCloseComment() { return close_comment; }
    int GetCommentLines() { return cmt_lines; }

protected:
    virtual void ProcessActions (Event theEvent);

private:
    int cpp_comments;
    int cmt_lines;
    int open_comment,
        close_comment;
};

#endif // _CMTCOUNT_H_

