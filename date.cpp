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

const int             zDate::ReformYear      = 1582;
const ulong           zDate::ReformDayNumber = 577737L;
const zDate::month    zDate::ReformMonth     = zDate::oct;
        zDate::week_day zDate::BeginDSTDay     = zDate::sun;
        zDate::month    zDate::BeginDSTMonth   = zDate::apr;
        zDate::week_day zDate::EndDSTDay       = zDate::sun;
        zDate::month    zDate::EndDSTMonth     = zDate::oct;

zDate::zDate()
{
      Set(jan, 1, 1);
}

zDate::zDate(month aMonth, int aDay, int aYear)
{
      Set(aMonth, aDay, aYear);
}

zDate::zDate(int dayOfYear, int year)
{
      m_day   = 31;
      m_month = dec;
      m_year  = year - 1;
      m_dayno = MakeDayNumber();
      FromDayNumber(m_dayno + dayOfYear);
}

zDate
zDate::Set(month aMonth, int aDay, int aYear)
{
      m_month = aMonth;
      m_day   = aDay;
      m_year  = aYear;
      m_dayno = MakeDayNumber();

      return *this;
}

zDate::zDate(const zDate &aDate)
{
      m_month = aDate.m_month;
      m_day   = aDate.m_day;
      m_year  = aDate.m_year;
      m_dayno = aDate.m_dayno;
}

zDate::zDate(ulong nJulian)
{
      FromDayNumber(nJulian);
}

zDate::zDate(const struct tm *tmDate)
{
      m_month = (month)(tmDate->tm_mon + 1);
      m_day   = tmDate->tm_mday;
      m_year  = tmDate->tm_year + 1900;
      m_dayno = MakeDayNumber();
}

zDate
zDate::Today()
{
      time_t     secs_now = time(0);
      struct tm *time_now = localtime(&secs_now);
      zDate      today(time_now);

      return today;
}

Boolean
zDate::operator!=(const zDate &aDate) const
{
      return Boolean(m_dayno != aDate.m_dayno);
}

Boolean
zDate::operator==(const zDate &aDate) const
{
      return Boolean(m_dayno == aDate.m_dayno);
}

Boolean
zDate::operator<(const zDate &aDate) const
{
      return Boolean(m_dayno < aDate.m_dayno);
}

Boolean
zDate::operator<=(const zDate &aDate) const
{
      return Boolean(m_dayno <= aDate.m_dayno);
}

Boolean
zDate::operator>(const zDate &aDate) const
{
      return Boolean(m_dayno > aDate.m_dayno);
}

Boolean
zDate::operator>=(const zDate &aDate) const
{
      return Boolean(m_dayno >= aDate.m_dayno);
}

zDate&
zDate::operator=(const zDate &aDate)
{
      m_day   = aDate.m_day;
      m_month = aDate.m_month;
      m_year  = aDate.m_year;
      m_dayno = aDate.m_dayno;

      return *this;
}

int
zDate::DaysInMonth(month aMonth, int aYear)
{
      static const int days[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

      if( aYear == ReformYear && aMonth == ReformMonth ) return 21;
      return days[aMonth] + (feb == aMonth && IsLeapYear(aYear));
}

int
zDate::DayOfYear() const
{
      zDate first(jan, 1, m_year);

      return 1 + (int)(m_dayno - first.m_dayno);
}

int
zDate::DaysInYear(int year)
{
      int days = 365 + IsLeapYear(year);
      // 10 days cancelled by the reform of pope Gregor XIII
      if( year == ReformYear ) return days - 10;
      else return days;
}

Boolean
zDate::IsLeapYear(int year)
{
      if( year % 4 ) return False;  // if not divisible by 4, not leap
      if( year < ReformYear ) return True; // before this year, all were leap
      if( year % 100 ) return True; // by 4, but not by 100 is leap
      if( year % 400 ) return False;      // not by 100 and not by 400 not leap
      return True;
}

Boolean
zDate::IsValid(month aMonth, int aDay, int aYear)
{
      return Boolean(
               aYear  > 0
            && aMonth >= jan && aMonth <= dec
            && aDay   >  0   && aDay   <= DaysInMonth(aMonth, aYear)
      );
}

int
zDate::Age(const zDate &birthday) const
{
      int age = m_year - birthday.m_year - 1;

      if( m_month > birthday.m_month ) age++;
      else if( m_month == birthday.m_month && m_day >= birthday.m_day ) age++;

      return age;
}

zDate
zDate::operator+(int nDays) const
{
      return zDate(m_dayno + (long)nDays);
}

zDate
zDate::operator+(long nDays) const
{
      return zDate(m_dayno + nDays);
}

zDate
zDate::operator-(int nDays) const
{
      return zDate(m_dayno - (long)nDays);
}

zDate
zDate::operator-(long nDays) const
{
      return zDate(m_dayno - nDays);
}

long
zDate::operator-(const zDate &aDate) const
{
      return (long)(m_dayno - aDate.m_dayno);
}

zDate&
zDate::operator+=(int nDays)
{
      FromDayNumber(m_dayno + (long)nDays);
      return *this;
}

zDate&
zDate::operator+=(long nDays)
{
      FromDayNumber(m_dayno + nDays);
      return *this;
}

zDate
zDate::operator++()
{
      FromDayNumber(m_dayno + 1L);
      return *this;
}

zDate
zDate::operator++(int)
{
      zDate date(*this);

      FromDayNumber(m_dayno + 1L);
      return date;
}

zDate
zDate::operator--()
{
      FromDayNumber(m_dayno - 1L);
      return *this;
}

/* postfix */
zDate
zDate::operator--(int)
{
      zDate date(*this);

      FromDayNumber(m_dayno - 1L);
      return date;
}

zDate&
zDate::operator-=(int nDays)
{
      FromDayNumber(m_dayno - (long)nDays);
      return *this;
}

zDate&
zDate::operator-=(long nDays)
{
      FromDayNumber(m_dayno - nDays);
      return *this;
}

int
zDate::WeekOfYear() const
{
      zDate first(jan, 1, m_year);

      return 1 + int((m_dayno - first.m_dayno + 1) / 7);
}

int
zDate::WeekOfMonth() const
{
      int abs_mday = m_day + zDate(m_month, 1, m_year).DayOfWeek() - 1;

      return 1 + ((abs_mday - DayOfWeek()) / 7);
}

int
zDate::WeeksInYear(int year)
{
      return zDate(dec, 31, year).WeekOfYear();
}

zDate
zDate::AddWeeks(int nWeeks) const
{
      zDate date(*this);

      date += (long)nWeeks * 7L;
      return date;
}

zDate
zDate::AddYears(int nYears) const
{
      zDate date(*this);
      int   delta = nYears > 0 ? -1 : 1;
      int   year = m_year;
      long  days = 0;

      while( nYears ){
            nYears += delta;
            year   -= delta;
            days   += DaysInYear(year);
      }
      date += (-delta * days);
      return date;
}

zDate::operator long() const
{
      return DayNumber();
}

/*
 * arguably useful routine, you can access the date object as an array,
 * with [0] == day (1..31), [1] == month (1..12), [2] == year - 1900
*/
char
zDate::operator[](int index) const
{
      switch( index ){
            case 0 : return m_day;
            case 1 : return m_month;
            case 2 : return m_year - 1900;
            default: return -1;
      }
}

int
zDate::DaysInMonth() const
{
      return DaysInMonth(m_month, m_year);
}

int
zDate::DaysInYear() const
{
      return DaysInYear(m_year);
}

int
zDate::WeeksInYear() const
{
      return WeeksInYear(m_year);
}

Boolean
zDate::IsValid() const
{
      return IsValid(m_month, m_day, m_year);
}

Boolean
zDate::IsLeapYear() const
{
      return IsLeapYear(m_year);
}

ulong
zDate::MakeDayNumber() const
{
      long days;
      long year  = (long)m_year - 1L;

      // get all days plus all leap years, minus non-leap years
      days = year * 365L + year / 4L - year / 100L + year / 400L;
      // the years before 1582 were all leap if divisible by 4
      if( year > ReformYear ) days += 12;
      else{
            days += year / 100L;
            days -= year / 400L;
      }
      // get the days for the month up to the current one
      for( int i = jan; i < m_month; ++i )
            days += DaysInMonth((month)i, m_year);
      // now add the current days of the month
      days += m_day;
      // now adjust for the 10 missing days (Oct 4 - Oct 15, 1582)
      if( days > ReformDayNumber ) days -= 10L;
      // we have the current day number now
      return days;
}

zDate::week_day
zDate::DayOfWeek() const
{
      const week_day wdays[7] = {sun,mon,tue,wed,thu,fri,sat};

      return wdays[(int)(((m_dayno % 7) + 5) % 7)];
}

void
zDate::FromDayNumber(ulong dayno)
{
      m_dayno = dayno;

      if( dayno > ReformDayNumber ) dayno += 10L;

      m_year = (int)(dayno / 365);
      m_day = (int)(dayno % 365L);

      if( m_year < 1700 ) m_day -= (m_year / 4);
      else{
            m_day -= (m_year / 4);
            m_day += (m_year / 100);
            m_day -= (m_year / 400);
            m_day -= 12;
      }

      while( m_day <= 0 ){
            m_day += (365 + IsLeapYear(m_year));
            m_year--;
      }

      // m_year is the number of elapsed years, add 1 to get current
      m_year += 1;

      // figure out the month and current day too
      for( m_month = jan; m_month <= dec; m_month = (month)(m_month + 1) ){
            int days = DaysInMonth(m_month, m_year);

            if( m_day <= days ) break;
            else m_day -= days;
      }
}

Boolean
zDate::IsDST() const
{
      return IsDST(*this);
}

/*
 * DST (Daylight Savings Time) starts at 2:00a on the first Sunday of
 * April and ends at 2:00a on the last Sunday of October (US rules)
*/

zDate
zDate::BeginDST(int year)
{
      zDate date(BeginDSTMonth, 1, year);

      while( BeginDSTDay != date.DayOfWeek() ) date++;
      return date;
}

zDate
zDate::EndDST(int year)
{
      zDate date(EndDSTMonth, 31, year);

      while( EndDSTDay != date.DayOfWeek() ) date--;
      return date;
}

zDate
zDate::BeginDST() const
{
      return BeginDST(m_year);
}

zDate
zDate::EndDST() const
{
      return EndDST(m_year);
}

Boolean
zDate::IsDST(const zDate &date)
{
      return Boolean( date >= date.BeginDST() && date <= date.EndDST() );
}

zDate::moon_phase
zDate::MoonPhase() const
{
      return MoonPhase(*this);
}

zDate::moon_phase
zDate::MoonPhase(const zDate &date)
{
      ulong phase = date.m_dayno;

      phase *= 9693L;
      phase /= 35780L;
      phase -= 4L;
      phase %= 8L;

      return (moon_phase)phase;
}

zDate
zDate::Easter() const
{
      return Easter(m_year);
}

/*
 * i won't pretend i know exactly how this algorithm works.
 * i used the one specified by the US Naval Observatory
*/
zDate
zDate::Easter(int year)
{
      int c, n, k, i, j, l, m, d;

      c = year / 100;
      n = year - 19 * (year / 19);
      k = (c - 17) / 25;
      i = c - c / 4 - (c - k) / 3 + 19 * n + 15;
      i = i - 30 * (i / 30);
      i = i - (i / 28) * (1 - (i / 28) * (29 / (i + 1)) * ((21 - n) / 11));
      j = year + year / 4 + i + 2 - c + c / 4;
      j = j - 7 * (j / 7);
      l = i - j;
      m = 3 + (l + 40) / 44;
      d = l + 28 - 31 * (m / 4);

      return zDate((month)m, d, year);
}

/*
 * this is a peculiar function - when months are added (or subtracted),
 * what really happens is that the month number is modified (with the
 * appropriate year adjustments too). if the resulting month/day combination
 * is invalid (i.e. Apr 31), the days will spill into the next month (in
 * the case with the example, the new date will be May 1). If we are
 * subtracting months and we end up with an invalid date, the difference
 * will be subtracted from the days (the month stays the same): this means
 * that March 31, 1996 (leap year) minus 1 month = Feb 27, 1996
*/
zDate
zDate::AddMonths(int nMonths) const
{
      int mon  = m_month + nMonths;
      int year = m_year;
      int day  = m_day;
      int mdays;

      while( mon < 1 ){
            mon += 12;
            year--;
      }

      while( mon > 12 ){
            mon -= 12;
            year++;
      }

      mdays = DaysInMonth((month)mon, year);

      if( day > mdays ){
            if( nMonths < 0 ) day = mdays - (day - mdays);
            else{
                  day -= mdays;
                  mon++;
                  if( mon > 12 ){
                        year++;
                        mon = 1;
                  }
            }
      }

      return zDate((month)mon, day, year);
}
