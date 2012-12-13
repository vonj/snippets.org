/*
**  PARSDATE.C - A simple parser to extract dates from strings.
**
**  Original Copyright 1995 by Robert B. Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "datetime.h"

static unsigned  year_norml(unsigned year);
static Boolean_T get_fields(char *str);
static Boolean_T validate(unsigned year, unsigned month, unsigned day);
static Boolean_T isleap (unsigned yr);
static unsigned  get_month(char *month);

static char *fields[3];
static char *month[2][12] = {
      {"January", "February", "March", "April", "May", "June",
       "July", "August", "September", "October", "November", "December"},
      {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}
};

/*
**  parse_date() - Parse a date string into its components
**
**  Arguments: 1 - String to parse
**             2 - Address of year storage
**             3 - Address of month storage
**             4 - Address of day storage
**             5 - Syntax to use
**
**  Returns: 0 For success, non-zero for range errors
**
**  Notes: The following syntaxes are supported:
**
**    Date ordering: USA (month, day, year),
**                   ISO (year, month, day)
**                   EUROPE (day, month, year)
**    Delimiters:    Spaces, commas, dashes, periods, slashes. (" ,-./")
**    Years:         2-digit years assumed to be between 1970 and 2069
**                   4-digit years required otherwise
*/

Boolean_T parse_date(const char *str,
                     unsigned   *year,
                     unsigned   *month,
                     unsigned   *day,
                     Syntax_T   syntax)
{
      unsigned yy, mm, dd;                /* Local data                 */
      char str_copy[512];                 /* Nice and roomy             */

      strcpy(str_copy, str);
      if (Error_ == get_fields(str_copy))
            return Error_;

      switch (syntax)
      {
      case USA:
            yy = atoi(fields[2]);
            dd = atoi(fields[1]);
            if (isalpha(*fields[0]))
            {
                  mm = get_month(fields[0]);
                  if (0 == mm)
                  {
                        return Error_;
                  }
            }
            else  mm = atoi(fields[0]);
            break;

      case ISO:
            yy = atoi(fields[0]);
            dd = atoi(fields[2]);
            if (isalpha(*fields[1]))
            {
                  mm = get_month(fields[1]);
                  if (0 == mm)
                        return Error_;
            }
            else  mm = atoi(fields[1]);
            break;

      case EUROPE:
            yy = atoi(fields[2]);
            dd = atoi(fields[0]);
            if (isalpha(*fields[1]))
            {
                  mm = get_month(fields[1]);
                  if (0 == mm)
                        return Error_;
            }
            else  mm = atoi(fields[1]);
            break;

      default:
            return Error_;
      }
      yy = year_norml(yy);
      if (Error_ == validate(yy, mm, dd))
            return Error_;
      *year  = yy;
      *day   = dd;
      *month = mm;
      return Success_;
}

/*
**  Utility function to split string fields
*/

static Boolean_T get_fields(char *str)
{
      if (NULL == (fields[0] = strtok(str, " ,-./")))
            return Error_;
      if (NULL == (fields[1] = strtok(NULL, " ,-./")))
            return Error_;
      if (NULL == (fields[2] = strtok(NULL, " ,-./")))
            return Error_;
      if (NULL != strtok(NULL, " ,-./"))
            return Error_;
      return Success_;
}

/*
**  Utility function to validate dates
*/

static Boolean_T validate(unsigned year, unsigned month, unsigned day)
{
      unsigned int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

      if (1 > month || 12 < month)
            return Error_;
      if (1 > day || day > (days[month - 1] + (2 == month && isleap(year))))
            return Error_;
      else  return Success_;
}

/*
**  Utility function to detect leap years
*/

static Boolean_T isleap (unsigned yr)
{
   return yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0);
}

/*
**  Utility function to normalize years
*/

static unsigned year_norml(unsigned year)
{
      if (99 < year)
            return year;
      if (69 < year)
            return year + 1900;
      else  return year + 2000;
}

/*
**  Utility function to determine month from string
**
**  (Note: Uses non-standard stricmp(), common to all DOS compilers
*/

static unsigned get_month(char *str)
{
      int ix, iy;                         /* Indexes                    */

      for (ix = 0; ix < 2; ++ix)
      {
            for (iy = 0; iy < 12; ++iy)
            {
                  if (Success_ == stricmp(str, month[ix][iy]))
                        return iy + 1;
            }
      }
      return 0;
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      if (2 > argc)
      {
            puts("Usage: PARSDATE syntax date_string");
            puts("       syntax = 0, 1, or 2");
            return EXIT_FAILURE;
      }
      while (--argc)
      {
            Syntax_T syntax;
            char *str;
            unsigned yy, mm, dd;
            Boolean_T retval;

            if (4 > argc)
                  break;
            syntax = atoi(*++argv);
            str = *++argv;
            printf("str = \"%s\"\n", str);
            retval = parse_date(str, &yy, &mm, &dd, syntax);
            printf("parse_date(%d, \"%s\") returned %d\n"
                  "  date = %u-%u-%u\n\n",
                  syntax, str, retval, mm, dd, yy);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
