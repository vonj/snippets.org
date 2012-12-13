/*
**  Header for SNIPPETS date & time function, excluding scalar dates
*/

#ifndef DATETIME__H
#define DATETIME__H

#include "sniptype.h"

/*
**  Julian day number functions from JDN_L.C
*/

long ymd_to_jdnl(int y, int m, int d, int julian);
void jdnl_to_ymd(long jdn, int *yy, int *mm, int *dd, int julian);

/*
**  Add times from ADDTIME.C
*/

int add_time(unsigned basehrs, unsigned basemins, unsigned basesecs,
             int spanhrs, int spanmins, int spansecs,
             unsigned *hrs, unsigned *mins, unsigned *secs, int *days);

/*
**  Determine the date of Easter for any given year
*/

void easter(int year, int *easter_month, int *easter_day);


/*
**  Moon phase from MOON_AGE.C
*/

int moon_age(int month, int day, int year);

/*
**  Date parser from PARSDATE.C
*/

typedef enum {USA, ISO, EUROPE} Syntax_T;

Boolean_T parse_date(const char *str, unsigned *year, unsigned *month,
                     unsigned *day, Syntax_T syntax);

/*
**  Time parser from PARSTIME.C
*/

Boolean_T parse_time(const char *str, unsigned *hours, unsigned *mins,
                     unsigned *secs);


#endif /* DATETIME__H */
