//
// loctm.cpp
// C++ class wrapper for ANSI-C struct tm
//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//

# include "loctm.h"

char loc_tm::timech = ':';
char loc_tm::datech = '/';

int loc_tm::datefmt = loc_tm::d_IntlShort;
int loc_tm::timefmt = loc_tm::t_International;


loc_tm::loc_tm (time_t t)
{
    if (t == 0)
        t = time(0);
    *(tm *)this = *localtime(&t);
}

loc_tm::loc_tm (tm const & t)
{
    *((tm *)this) = t;
}

loc_tm &
loc_tm::operator= (tm const & t)
{
    *((tm *)this) = t;
    return *this;
}

int
loc_tm::compare (loc_tm const & t) const
{
    return compare ((time_t)t);
}

int
loc_tm::compare (time_t const tt) const
{
    time_t tx = (time_t)*this;
    return (tx == tt) ? 0 : (tx > tt) ? 1 : -1;
}

loc_tm::operator time_t (void) const
{
    return mktime ((tm *)this);
}

int
loc_tm::is_valid (void) const
{
    static int maxd[] =
    {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    return (tm_year > 0 &&
            tm_mon >= 0 && tm_mon < 12 &&
            tm_mday > 0 && tm_mday <= maxd[tm_mon] &&
            tm_wday < 7 && tm_yday < 367 &&
            tm_sec < 60 && tm_min < 60 && tm_hour < 24);
}
