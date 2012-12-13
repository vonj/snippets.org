/*
**  Public domain by Jeff Dunlop
*/

#ifndef FTIME__H
#define FTIME__H

#include <dos.h>
#include <time.h>

#if !defined(__TURBOC__) && !defined(__SC__) && !defined(__POWERC)

struct ftime /* As defined by Borland & Symantec */
{
    unsigned    ft_tsec  : 5;   /* Two second interval */
    unsigned    ft_min   : 6;   /* Minutes */
    unsigned    ft_hour  : 5;   /* Hours */
    unsigned    ft_day   : 5;   /* Days */
    unsigned    ft_month : 4;   /* Months */
    unsigned    ft_year  : 7;   /* Year */
};

int getftime (int, struct ftime *);
int setftime (int, struct ftime *);

#else
 #include <io.h>
#endif

void   ftime2tm(struct ftime *ft, struct tm *time);
time_t ftime2time(struct ftime *ft);

#endif /* FTIME__H */
