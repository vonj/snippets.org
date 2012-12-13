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
#ifndef __DATE_INC
#define __DATE_INC

typedef unsigned long ulong;
typedef enum { False, True } Boolean;
#include <time.h>

class zDate
{
public:
      enum month {
            jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
      };

      enum week_day {
            mon = 1, tue, wed, thu, fri, sat, sun
      };

      enum moon_phase {
            new_moon, waxing_crescent, first_quater, waxing_gibbous,
            full_moon, waning_gibbous, third_quater, waning_crescent
      };

      zDate();
      zDate(month month, int day, int year);
      zDate(int dayOfYear, int year);
      zDate(const zDate &aDate);
      zDate(ulong nDayNumber);
      zDate(const struct tm *date);

      zDate             AddMonths(int nMonths) const;
      zDate             AddWeeks(int nWeeks) const;
      zDate             AddYears(int nYears) const;
      int               Age(const zDate &aDate) const;
      zDate             BeginDST() const;
      static zDate      BeginDST(int aYear);
      int               Day() const;
      ulong             DayNumber() const;
      week_day          DayOfWeek() const;
      int               DayOfYear() const;
      int               DaysInMonth() const;
      static int        DaysInMonth(month aMonth, int aYear);
      int               DaysInYear() const;
      static int        DaysInYear(int year);
      zDate             Easter() const;
      static zDate      Easter(int year);
      zDate             EndDST() const;
      static zDate      EndDST(int aYear);
      Boolean           IsDST() const;
      static Boolean    IsDST(const zDate &date);
      Boolean           IsLeapYear() const;
      static Boolean    IsLeapYear(int year);
      Boolean           IsValid() const;
      static Boolean    IsValid(month aMonth, int aDay, int aYear);
      month             Month() const;
      moon_phase        MoonPhase() const;
      static moon_phase MoonPhase(const zDate &date);
                                operator long() const;
      Boolean           operator!=(const zDate &aDate) const;
      zDate             operator+(int nDays) const;
      zDate             operator+(long nDays) const;
      zDate             operator++();
      zDate             operator++(int);
      zDate&            operator+=(int nDays);
      zDate&            operator+=(long nDays);
      long              operator-(const zDate &aDate) const;
      zDate             operator-(int nDays) const;
      zDate             operator-(long nDays) const;
      zDate             operator--();
      zDate             operator--(int);
      zDate&            operator-=(int nDays);
      zDate&            operator-=(long nDays);
      Boolean           operator<(const zDate &aDate) const;
      Boolean           operator<=(const zDate &aDate) const;
      zDate&            operator=(const zDate &aDate);
      Boolean           operator==(const zDate &aDate) const;
      Boolean           operator>(const zDate &aDate) const;
      Boolean           operator>=(const zDate &aDate) const;
      char              operator[](int index) const;
      static void       SetBeginDST(month aMonth, week_day aWeekDay);
      static void       SetEndDST(month aMonth, week_day aWeekDay);
      static zDate      Today();
      int               WeekOfMonth() const;
      int               WeekOfYear() const;
      int               WeeksInYear() const;
      static int        WeeksInYear(int year);
      int               Year() const;

      // Pope Gregor XIII's reform cancelled 10 days:
      // the day after Oct 4 1582 was Oct 15 1582
      static const int      ReformYear;
      static const month    ReformMonth;
      static const ulong    ReformDayNumber;

protected:
      // Daylight Savings Time Month and Day of Week
      static month    BeginDSTMonth;
      static week_day BeginDSTDay;
      static month    EndDSTMonth;
      static week_day EndDSTDay;

protected:
      zDate Set(month aMonth, int aDay, int aYear);
      ulong MakeDayNumber() const;
      void  FromDayNumber(ulong nDayNumber);

private:
      month m_month;
      int   m_day;
      int   m_year;
      ulong m_dayno;
};

/*
 * inline functions that belong to the zDate class
*/

inline int
zDate::Year() const
{
      return m_year;
}

inline int
zDate::Day() const
{
      return m_day;
}

inline zDate::month
zDate::Month() const
{
      return m_month;
}

inline ulong
zDate::DayNumber() const
{
      return m_dayno;
}

#endif /* __DATE_INC */
