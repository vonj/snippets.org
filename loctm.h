//
// loctm.h
// Time functions class (a C++ wrapper for ANSI struct tm)
//
// written by David Nugent
//
// This code is public domain. Use for any purpose is unrestricted.
//

# if !defined( _loctm_h )
# define _loctm_h 1
# include <time.h>
# if defined( __MSDOS__ ) || defined( MSDOS )
#  define STRSTREAM_H "strstrea.h"  // 8.3 (sigh)
# else
#  define STRSTREAM_H "strstream.h"
# endif

    // Forward declare for streams interface

class ostream;
class istream;

    // class loc_tm
    // C++ class (mainly I/O) wrapper for ANSI-C struct tm

class loc_tm : public tm
{

  public:

    loc_tm (void) {}                  // Do nothing constructor
    loc_tm (time_t t);     // Set to time, 0 is magic for 'now'
    loc_tm (tm const & t); // Copy constructor

    loc_tm & operator= (tm const & t);            // Assignment
    operator time_t (void) const;        // Conversion operator

    int is_valid (void) const;             // Test for validity

    int compare (loc_tm const & t) const;      // Compare times
    int compare (time_t const tt) const;       // Compare times

    enum f_date     // Date format flags
    {
        d_Year          = (int)0x0001,            // Print year
        d_YearShort     = (int)0x0002, // Print last two digits
        d_PadYear       = (int)0x0004,    // Pad year to 2 or 4
        d_ZeroYear      = (int)0x0008,        // Zero fill year
        d_Month         = (int)0x0010,           // Print month
        d_MonText       = (int)0x0020,   // Print month in text
        d_PadMon        = (int)0x0040, // Pad to 2 (trunc to 3)
        d_ZeroMon       = (int)0x0080,       // Zero fill month
        d_Day           = (int)0x0100,            // Print date
        d_DayOfWeek     = (int)0x0200,     // Print day of week
        d_PadDay        = (int)0x0400,         // Pad date to 2
        d_ZeroDay       = (int)0x0800,         // Zero fill day
        d_DMY           = (int)0x0111, // Print date, mth, year
        d_PadDMY        = (int)0x0444,         // Pad all three
        d_ZeroDMY       = (int)0x0888,   // Zero fill all three
        d_YearFirst     = (int)0x1000,      // Print year first
        d_MonFirst      = (int)0x2000,     // Print month first
        d_SepChar       = (int)0x4000,// Separate fields datech
        d_SepSpace      = (int)0x8000  // Separate fields space
    };
    enum date_f                         // Composite date flags
    {
        d_International = (int)(d_DMY|d_MonText|d_PadMon|
                                d_SepSpace),
        d_IntlShort     = (int)(d_DMY|d_MonText|d_PadMon|
                                d_SepSpace|d_YearShort),
        d_Usa           = (int)(d_DMY|d_MonFirst|d_PadDMY|
                                d_ZeroDMY|d_YearShort|d_SepChar),
        d_English       = (int)(d_DMY|d_YearShort|d_PadDMY|
                                d_ZeroDMY|d_SepChar),
        d_Japanese      = (int)(d_DMY|d_YearFirst|d_PadDMY|
                                d_ZeroDMY|d_YearShort|d_MonFirst|
                                d_SepChar),
        d_Full          = (int)(d_DMY|d_DayOfWeek|d_MonText|
                                d_SepSpace)
    };

    enum f_time  // Time format (nb: time zones not implemented)
    {
        t_Secs          = (int)0x0001,         // Print seconds
        t_ZeroSecs      = (int)0x0002,     // Zero fill seconds
        t_PadSecs       = (int)0x0004,      // Pad seconds to 2
        t_SecsAll       = (int)0x0007,
        t_TimeZone      = (int)0x0008,        // Print timezone
        t_Mins          = (int)0x0010,         // Print minutes
        t_ZeroMins      = (int)0x0020,     // Zero fill minutes
        t_PadMins       = (int)0x0040,      // Pad minutes to 2
        t_MinsAll       = (int)0x0070,
        t_TZNumeric     = (int)0x0080,      // Print numeric TZ
        t_Hour          = (int)0x0100,            // Print hour
        t_ZeroHour      = (int)0x0200,        // Zero fill hour
        t_PadHour       = (int)0x0400,  // Pad hour to 2 digits
        t_HourAll       = (int)0x0700,
        t_24hour        = (int)0x0800,          // 24hour clock
        t_SepChar       = (int)0x1000, // Separate field timech
        t_SepSpace      = (int)0x2000, // Separate fields space
        t_SepAbbrev     = (int)0x4000,     // Add abbreviations
        t_AmPm          = (int)0x8000       // Add 'am' or 'pm'
    };
    enum time_f  // Composite time flags
    {
        t_International = (int)(t_HourAll|t_MinsAll|t_SecsAll|
                                t_24hour|t_SepChar),
        t_ShortTime     = (int)(t_HourAll|t_MinsAll|t_24hour|
                                t_SepChar),
        t_ClockTime     = (int)(t_Hour|t_PadHour|t_MinsAll|
                                t_AmPm|t_SepChar),
        t_LongTime      = (int)(t_Hour|t_PadHour|t_MinsAll|
                                t_SecsAll|t_SepAbbrev|t_SepSpace),
        t_Millitary     = (int)(t_HourAll|t_MinsAll|t_24hour)
    };


    static char timech;    // Character used for time separator
    static char datech;    // Character used for date separator
    static int datefmt;                  // Default date format
    static int timefmt;                  // Default time format

                                              // Output methods
    ostream & print (ostream & os, int df =datefmt,
                                   int tf =timefmt) const;
    ostream & printTime (ostream & os, int f =timefmt) const;
    ostream & printDate (ostream & os, int f =datefmt) const;

                                       // Input/parsing methods
    istream & parseTime (istream & is);        // Unimplemented
    istream & parseDate (istream & is);        // Unimplemented
    istream & parse (istream & is);            // Unimplemented

  private:
                                                 // Time suffix
    void tSfx (ostream & os, int fmt, char ch) const;
                                       // Time field formatters
    void pHour (ostream & os, int fmt) const;
    void pMins (ostream & os, int fmt) const;
    void pSecs (ostream & os, int fmt) const;
                                                 // Date suffix
    void dSfx (ostream & os, int fmt) const;
                                       // Date field formatters
    void pDate (ostream & os, int fmt) const;
    void pMonth (ostream & os, int fmt) const;
    void pYear (ostream & os, int fmt) const;

};

inline ostream &                        // Stream output method
operator<< (ostream & os, loc_tm const & t)
{   return t.print(os);     }

inline int                                          // Equality
operator== (loc_tm const & t1, loc_tm const & t2)
{   return int(t1.compare(t2) == 0);    }

inline int                                        // Inequality
operator!= (loc_tm const & t1, loc_tm const & t2)
{   return int(t1.compare(t2) != 0);    }

inline int                                         // Less then
operator<  (loc_tm const & t1, loc_tm const & t2)
{   return int(t1.compare(t2) < 0);     }

inline int                                   // Less than/equal
operator<= (loc_tm const & t1, loc_tm const & t2)
{   return int(t1.compare(t2) <= 0);    }

inline int                                      // Greater than
operator>  (loc_tm const & t1, loc_tm const & t2)
{   return int(t1.compare(t2) > 0);     }

inline int                                // Greater than/equal
operator>=  (loc_tm const & t1, loc_tm const & t2)
{   return int(t1.compare(t2) >= 0);    }


    // class loc_date
    // Date formatter

class loc_date : public loc_tm
{

  public:

    loc_date (loc_tm const & t, int dtfmt =loc_tm::datefmt)
        : loc_tm (t), _fmt(dtfmt)   {}
    loc_date (time_t t, int dtfmt =loc_tm::datefmt)
        : loc_tm (t), _fmt(dtfmt)   {}
    int fmt (int f)                 {   return _fmt = f;    }
    int fmt (void) const            {   return _fmt;        }

    ostream & print (ostream & os) const
        {   return printDate (os, _fmt);    }

  private:

    int _fmt;

};

inline ostream &                // Stream output method - date
operator<< (ostream & os, loc_date const & d)
{   return d.print(os);     }


    // class loc_time
    // Time formatter

class loc_time : public loc_tm
{

  public:

    loc_time (loc_tm const & t, int tmfmt =loc_tm::timefmt)
        : loc_tm (t), _fmt(tmfmt)   {}
    loc_time (time_t t, int tmfmt =loc_tm::timefmt)
        : loc_tm (t), _fmt(tmfmt)   {}
    int fmt (int f)                 {   return _fmt = f;    }
    int fmt (void) const            {   return _fmt;        }

    ostream & print (ostream & os) const
        {   return printTime (os, _fmt);    }

  private:

    int _fmt;

};

inline ostream &                // Stream output method - time
operator<< (ostream & os, loc_time const & t)
{   return t.print(os);     }



# endif     // _loctm_h
