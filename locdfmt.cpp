//
// locdfmt.cpp
// Date formatting functions
//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//

# include "loctm.h"
# include STRSTREAM_H
# include "iomanip.h"

void
loc_tm::dSfx (ostream & os, int fmt) const
{
    if (fmt & d_SepChar)
        os << datech;
    if (fmt & d_SepSpace)
        os << ' ';
}

void
loc_tm::pYear (ostream & os, int fmt) const
{
    if (fmt & d_Year)
    {
        int year = tm_year;
        int dig;
        if (fmt & d_YearShort)
        {
            dig = 2;
            year %= 100;
        }
        else
        {
            dig = 4;
            if (year < 200)
                year += 1900;
        }
        os << setw((fmt & d_PadYear) ? dig : 0);
        os << setfill((fmt & d_ZeroYear) ? '0' : ' ');
        os << year;
        if ((fmt & d_YearFirst))
        {
            fmt &= (d_SepChar|d_SepSpace);
            dSfx (os, fmt);
        }
    }
}

void
loc_tm::pMonth (ostream & os, int fmt) const
{

    static const char * _months[] =
    {
        "January", "February", "March", "April",
        "May", "June", "July", "August", "September",
        "October", "November", "December"
    };

    if (fmt & d_Month)
    {
        int mon = (tm_mon % 12);
        if (fmt & d_MonText)
        {
            char const * tmon = _months[mon];
            if (!(fmt & d_PadMon))
                os << tmon;
            else
                for (int x = 0; x < 3; ++x)
                    os << tmon[x];
        }
        else
        {
            ++mon;
            os << setw((fmt & d_PadMon) ? 2 : 0);
            os << setfill((fmt & d_ZeroMon) ? '0' : ' ');
            os << mon;
        }
        if (((fmt & d_Year) && !(fmt & d_YearFirst)) ||
            ((fmt & d_Day) && (fmt & d_MonFirst)))
        {
            fmt &= (d_SepChar|d_SepSpace);
            dSfx (os, fmt);
        }
    }
}


void
loc_tm::pDate (ostream & os, int fmt) const
{
    if (fmt & d_Day)
    {
        int day = tm_mday;
        os << setw((fmt & d_PadDay) ? 2 : 0);
        os << setfill((fmt & d_ZeroDay) ? '0' : ' ');
        os << day;
        if (!(fmt & d_YearFirst) || !(fmt & d_MonFirst))
        {
            fmt &= (d_SepChar|d_SepSpace);
            dSfx (os, fmt);
        }
    }
}


ostream &
loc_tm::printDate (ostream & os, int fmt) const
{
    ostrstream pTmp;

    static const char * _days[] =
    {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };

    if (fmt & d_DayOfWeek)
    {
        int day = tm_wday % 7;
        char const * p = _days[day];
        if (fmt & d_PadDay)
            for (int x = 0; x < 3; ++x)
                pTmp << p[x];
        else
        {
            pTmp << p;
            if (fmt & d_DMY)
                pTmp << ',';
        }
        if ((fmt & d_DMY) && fmt & d_SepSpace)
            pTmp << ' ';
    }
    if (fmt & d_YearFirst)
        pYear (pTmp, fmt);
    if (fmt & d_MonFirst)
        pMonth (pTmp, fmt);
    pDate (pTmp, fmt);
    if (!(fmt & d_MonFirst))
        pMonth (pTmp, fmt);
    if (!(fmt & d_YearFirst))
        pYear (pTmp, fmt);
    pTmp << ends;

    char const * p = pTmp.str();
    pTmp.rdbuf()->freeze(0);
    return os << p;
}
