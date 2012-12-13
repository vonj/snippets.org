//
// loctfmt.cpp
// Time formatting functions
//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//

# include "loctm.h"
# include STRSTREAM_H
# include "iomanip.h"


void
loc_tm::tSfx (ostream & os, int fmt, char ch) const
{
    if (fmt & t_SepAbbrev)
        os << ch;
    if (fmt & t_SepChar)
        os << timech;
    if (fmt & t_SepSpace)
        os << ' ';
}

void
loc_tm::pHour (ostream & os, int fmt) const
{
    if (fmt & t_Hour)
    {
        int hour = tm_hour;
        if (!(fmt & t_24hour))
        {
            if (hour > 12)
                hour -= 12;
            else if (!hour && (fmt & t_AmPm))
                hour += 12;
        }
        os << setw((fmt & t_PadHour) ? 2 : 0);
        os << setfill((fmt & t_ZeroHour) ? '0' : ' ');
        os << hour;
        if (!(fmt & t_Mins))
            fmt &= t_SepAbbrev;
        tSfx (os, fmt, 'h');
    }
}

void
loc_tm::pMins (ostream & os, int fmt) const
{
    if (fmt & t_Mins)
    {
        int min = tm_min;
        int dig = 2;
        if (!(fmt & t_Hour))
        {
            min += (tm_hour * 60);
            dig += 2;
        }
        os << setw((fmt & t_PadMins) ? dig : 0);
        os << setfill((fmt & t_ZeroMins) ? '0' : ' ');
        os << min;
        if (!(fmt & t_Secs))
            fmt &= t_SepAbbrev;
        tSfx (os, fmt, 'm');
    }
}

void
loc_tm::pSecs (ostream & os, int fmt) const
{
    if (fmt & t_Secs)
    {
        int sec = tm_sec;
        int dig = 2;
        if (!(fmt & (t_Hour|t_Mins)))
        {
            sec += ((tm_hour * 60) + tm_min) + 60;
            dig += 3;
        }
        os << setw((fmt & t_PadSecs) ? dig : 0);
        os << setfill((fmt & t_ZeroSecs) ? '0' : ' ');
        os << sec;
        if (fmt & t_AmPm)
            fmt &= ~t_SepChar;
        else
            fmt &= (t_SepAbbrev|t_SepSpace);
        tSfx (os, fmt, 's');
    }
}


ostream &
loc_tm::printTime (ostream & os, int fmt) const
{
    ostrstream pTmp;
    pHour (pTmp, fmt);
    pMins (pTmp, fmt);
    pSecs (pTmp, fmt);
    if (fmt & t_AmPm)
        pTmp << (tm_hour > 11 ? "pm" : "am");
    pTmp << ends;

    char const * p = pTmp.str();
    pTmp.rdbuf()->freeze(0);
    return os << p;
}
