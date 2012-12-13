//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//
// $Id$
//
// $Log$
//

# include "iostream.h"
# include "loctm.h"

# define NL '\n'

int
main ()
{
    loc_tm t(0);            // Get current time

    cout << "Dates ...\n"
      << " Intl    : " << loc_date(t,loc_tm::d_International) << NL
      << " Intl(s) : " << loc_date(t,loc_tm::d_IntlShort)     << NL
      << " USA     : " << loc_date(t,loc_tm::d_Usa)           << NL
      << " IntlEng : " << loc_date(t,loc_tm::d_English)       << NL
      << " Japan   : " << loc_date(t,loc_tm::d_Japanese)      << NL
      << " Full    : " << loc_date(t,loc_tm::d_Full)          << NL
      << " Weekday : " << loc_date(t,loc_tm::d_DayOfWeek)     << NL
      << " Date    : " << loc_date(t,loc_tm::d_Day)           << NL
      << " Month   : " << loc_date(t,loc_tm::d_Month)         << NL
      << " Month(t): " << loc_date(t,loc_tm::d_Month|loc_tm::d_MonText) << NL
      << " Year    : " << loc_date(t,loc_tm::d_Year)          << NL
      << " Year(s) : " << loc_date(t,loc_tm::d_Year|loc_tm::d_YearShort) << NL
      << endl;
    cout << "Times ...\n"
      << " Intl    : " << loc_time(t,loc_tm::t_International) << NL
      << " Short   : " << loc_time(t,loc_tm::t_ShortTime)     << NL
      << " Clock   : " << loc_time(t,loc_tm::t_ClockTime)     << NL
      << " Long    : " << loc_time(t,loc_tm::t_LongTime)      << NL
      << " Millit  : " << loc_time(t,loc_tm::t_Millitary)     << NL
      << endl;
    return 0;
}
