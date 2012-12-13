/*
 * This file is part of PB-Lib C/C++ Library
 *
 * Copyright (c) 1995, 1996 Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This class is hereby donated to the SNIPPETS collection (maintained
 * by Bob Stout). You are granted the right to use the code contained
 * herein free of charge as long as you keep this copyright notice intact.
 *
 * Contact: 73023.262@compuserve.com
*/
#include "date.hpp"
#include <iostream.h>

ostream& operator<<(ostream &stream, zDate::moon_phase phase)
{
      switch( phase ){
            case zDate::new_moon: stream << "new"; break;
            case zDate::waxing_crescent: stream << "waxing crescent"; break;
            case zDate::first_quater: stream << "first quater"; break;
            case zDate::waxing_gibbous: stream << "waxing gibbous"; break;
            case zDate::full_moon: stream << "full"; break;
            case zDate::waning_gibbous: stream << "waning gibbous"; break;
            case zDate::third_quater: stream << "third quater"; break;
            case zDate::waning_crescent: stream << "waning crescent"; break;
            default: stream << "--error--"; break;
      }
      return stream;
}

ostream& operator<<(ostream &stream, const zDate &date)
{
      stream << date.Day() << "-" << (int)date.Month() << "-" << date.Year();
      return stream;
}

ostream& operator<<(ostream &stream, zDate::week_day day)
{
      switch( day ){
            case zDate::sun: stream << "Sunday"; break;
            case zDate::mon: stream << "Monday"; break;
            case zDate::tue: stream << "Tuesday"; break;
            case zDate::wed: stream << "Wednesday"; break;
            case zDate::thu: stream << "Thursday"; break;
            case zDate::fri: stream << "Friday"; break;
            case zDate::sat: stream << "Saturday"; break;
            default        : stream << "--error--"; break;
      }
      return stream;
}

void test()
{
      cout << " zDate Class Demo \n\n";

      // default constructor, Jan 1 0000
      zDate a;
      cout << a << endl;
      // Various versions of the constructors
      zDate x(zDate::oct,20,1962);
      cout << x << endl;
      // constructor with a julian
      zDate z( 2450000L );
      cout << z << endl;
      // make a date with system date (tests copy constructor)
      zDate s(zDate::Today());
      cout << s << endl;
      // init with the day of year
      zDate y(33, 1996);
      cout << y << endl;
      // init from current system time
      time_t secs_now = time(NULL);
      zDate n(localtime(&secs_now));
      cout << n << endl;

      // using date addition and subtraction
      zDate adder = x + 10;
      cout << adder << endl;
      adder = adder - 25;
      cout << adder << endl;

      //using subtraction of two date objects
      zDate a1(zDate::Today());
      zDate a2 = a1 + 14;
      cout << (a1 - a2) << endl;
      cout << (a2 += 10) << endl;

      a1++;
      cout << "Tommorrow= " << a1 << endl;

      a1 = zDate(zDate::jul, 14, 1991);
      cout << "a1 (7-14-91) < a2 (" << a2
             << ")? ==> " << ((a1 < a2) ? "TRUE" : "FALSE") << endl;
      cout << "a1 (7-14-91) > a2 ("<< a2
             << ")? ==> " << ((a1 > a2) ? "TRUE" : "FALSE") << endl;
      cout << "a1 (7-14-91) < 8-01-91 ? ==> "
             << ((a1 < zDate(zDate::aug, 1, 1991)) ? "TRUE" : "FALSE") << endl;
      cout << "a1 (7-14-91) > 8-01-91 ? ==> "
             << ((a1 > zDate(zDate::aug, 1, 1991)) ? "TRUE" : "FALSE") << endl;
      cout << "a1 (7-14-91) == 7-14-91 ? ==> "
             << ((a1==zDate(zDate::jul, 14, 1991)) ? "TRUE" : "FALSE") << endl;
      zDate a3 = a1;

      cout << "a1 (" << a1 << ") == a3 (" << a3
             << ") ? ==> " << ((a1==a3) ? "TRUE" : "FALSE") << endl;
      zDate a4 = a1;
      ++a4;
      cout << "a1 ("<< a1 <<") == a4 (" << a4
             << ") ? ==> " << ((a1==a4) ? "TRUE" : "FALSE") << endl;

      zDate a5(zDate::Today());
      cout << "Today is: " << a5 << endl;
      a4 = zDate::Today();
      cout << "Today (a4) is: " << a4 << endl;

      cout << "Today + 4 is: " << (a4 += 4) << endl;
      a4 = zDate::Today();
      cout << "Today - 4 is: " << (a4 -= 4) << endl;
      cout << "=========== Leap Year Test ===========\n";
      a1 = zDate(zDate::jan, 15, 1992);
      cout << a1 << "\t" << ((a1.IsLeapYear()) ? "Leap" : "non-Leap");
      cout << "\t" << "day of year:  " << a1.DayOfYear() << endl;

      a1 = zDate(zDate::feb, 16, 1993);
      cout << a1 << "\t" << ((a1.IsLeapYear()) ? "Leap" : "non-Leap");
      cout << "\t" << "day of year:  " << a1.DayOfYear() << endl;

      zDate v4(zDate::Today());
      cout << "---------- Add Stuff -----------\n";
      cout << "Start => " << v4 << endl;
      cout << "Add  4 Weeks  => " << v4.AddWeeks(4) << endl;
      cout << "Sub 52 Weeks  => " << v4.AddWeeks(-52)  << endl;
      cout << "Add  2 Years  => " << v4.AddYears(2)    << endl;

      cout << flush;

      cout << "---------- Misc Stuff -----------\n";
      cout << "The date aboves' day of the month is => " << v4.Day() << endl;
      cout << "There are " << v4.DaysInMonth() << " days in this month.\n";
      cout << "This day happens to be " << v4.DayOfWeek() << " day of week" << endl;
      cout << "on the " << v4.WeekOfYear() << " week of the year," << endl;
      cout << "on the " << v4.WeekOfMonth() << " week of the month, " << endl;
      cout << "which is the "<< (int)v4.Month() << "nth month in the year.\n";
      cout << "The year alone is " << v4.Year() << endl;
      cout << "And this is the " << v4.DayOfYear() << " day of year" << endl;
      cout << "of a year with " << v4.DaysInYear() << " days in it" << endl;
      cout << "which makes exatcly " << v4.WeeksInYear() << " weeks" << endl;

      zDate birthday(zDate::jul, 16, 1973);
      cout << "The age test: i was born on " << birthday
             << " which makes me " << v4.Age(birthday) << " years old" << endl;

      zDate       D2(zDate::jul, 4, 1776);
      int         I1 = 4;

      cout << "Before: I1 = " << I1 << ",  D2 = " << D2 << endl;
      cout << "---------- Postfix '++' test -----------\n";
      cout << "Test : I1++ = " << I1++ << ",  D2++ = " << D2++ << endl;
      cout << "After: I1   = " << I1 << ",  D2   = " << D2 << endl;

      cout << "---------- Prefix '++' test -----------\n";
      cout << "Test : ++I1 = " << ++I1 << ",  ++D2 = " << ++D2 << endl;
      cout << "After:   I1 = " << I1 << ",    D2 = " << D2 << endl;

      cout << "---------- Postfix '--' test -----------\n";
      cout << "Test : I1-- = " << I1-- << ",  D2-- = " << D2-- << endl;
      cout << "After: I1   = " << I1 << ",  D2   = " << D2 << endl;

      cout << "---------- Prefix '--' test -----------\n";
      cout << "Test : --I1 = " << --I1 << ",  --D2 = " << --D2 << endl;
      cout << "After:   I1 = " << I1 << ",    D2 = " << D2 << endl;

      cout << "Last day of this year is dayno "
             << zDate(zDate::dec, 31, 1996).DayOfYear() << endl;
      cout << "Last day of prev year is dayno "
             << zDate(zDate::dec, 31, 1995).DayOfYear() << endl;

      cout << "Today the moon is " << zDate::Today().MoonPhase() << endl;

      zDate today = zDate::Today();

      cout << "DST for " << today.Year() << " starts on " << today.BeginDST()
             << " and ends on " << today.EndDST() << endl;
      cout << "Today, " << today << ", DST is "
             << (today.IsDST() ? "" : "not") << "in effect" << endl;

      zDate date1(zDate::aug, 31, 1996);
      cout << "Adding 6 months to " << date1 << " results in "
             << date1.AddMonths(6) << endl;

      zDate date2(zDate::mar, 31, 1996);
      cout << "Subtracting 1 month from " << date2 << " results in "
             << date2.AddMonths(-1) << endl;

      zDate date3(zDate::jul, 4, 1776);
      cout << "Adding 2400 months to " << date3 << " results in "
             << date3.AddMonths(2400) << endl;

      cout << "Today's day number is " << zDate::Today().DayNumber() << endl;

      zDate date4(zDate::feb, 29, 1996);
      cout << date4 << " subtract two years = " << date4.AddYears(-2) << endl;

      cout << "In 1996, DST began on " << zDate::BeginDST(1996) << endl;

      zDate date5(zDate::sep, 26, 1996);
      cout << "Moon phase on " << date5 << " was " << date5.MoonPhase() << endl;

      zDate date6(zDate::oct, 3, 1996);
      cout << date6 << " + 55 days is " << (date6 + 55) << endl;

      zDate date7(zDate::oct, 4, 1996);
      cout << date7 << " + 217 days is ";
      date7 += 217;
      cout << date7 << endl;
      date7 = zDate(zDate::oct, 4, 1996);
      cout << "Same date - (-217) days is ";
      date7 -= -217;
      cout << date7 << endl;

      cout << "For 1996, Easter is on " << zDate::Easter(1996) << endl;
}


void
main()
{
      test();
};
