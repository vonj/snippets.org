//
// locfmt.cpp
// Date/time (combined) format
//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//

# include "loctm.h"
# include STRSTREAM_H

ostream &
loc_tm::print (ostream & os, int df, int tf) const
{
    ostrstream pTmp;
    printDate (pTmp, df);
    pTmp << ' ';
    printTime (pTmp, tf);
    pTmp << ends;

    char const * p = pTmp.str();
    pTmp.rdbuf()->freeze(0);
    return os << p;
}
