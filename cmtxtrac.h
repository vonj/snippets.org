/*
 * CMTXTRAC.H
 * Comment extractor class.
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

#ifndef _CMTXTRAC_H_
#define _CMTXTRAC_H_

#include "cmtparsr.h"

class CommentExtractor : public CommentParser
{
public:
    CommentExtractor()
        : CommentParser()
        , printWhiteSpace(0)
        , printLineNumbers(0)
    { }

    void WhiteSpace  (int OnOff) { printWhiteSpace  = OnOff; }
    void LineNumbers (int OnOff) { printLineNumbers = OnOff; }

protected:
    virtual void ProcessActions (Event theEvent);

private:
    int printWhiteSpace;
    int printLineNumbers;
};

#endif // _CMTXTRAC_H_

