/*
**  FMTMONEY.C - Format a U.S. dollar value into a numeric string
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "snipmath.h"

#define Form(s,a) bufptr += sprintf(bufptr, s, a)

static char buf[256], *bufptr;

static char *units[] = {"Zero", "One", "Two", "Three", "Four",
                        "Five", "Six", "Seven", "Eight", "Nine",
                        "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen",
                        "Fifteen", "Sixteen", "Seventeen", "Eighteen",
                        "Nineteen"},

            *tens[]  = {"Twenty", "Thirty", "Forty", "Fifty", "Sixty",
                        "Seventy", "Eighty", "Ninety"};

static void form_group(int, char *);

/*
**  Call with double amount
**  Rounds cents
**  Returns string in a static buffer
*/

char *fmt_money(double amt)
{
      int temp;
      double dummy, cents = modf(amt, &dummy);

      *buf = '\0';
      bufptr = buf;

      temp = (int)(amt/1E12);
      if (temp)
      {
            form_group(temp, "Trillion");
            amt = fmod(amt, 1E12);
      }

      temp = (int)(amt/1E9);
      if (temp)
      {
            form_group(temp, "Billion");
            amt = fmod(amt, 1E9);
      }

      temp = (int)(amt/1E6);
      if (temp)
      {
            form_group(temp, "Million");
            amt = fmod(amt, 1E6);
      }

      temp = (int)(amt/1E3);
      if (temp)
      {
            form_group(temp, "Thousand");
            amt = fmod(amt, 1E3);
      }
      form_group((int)amt, "");

      if (buf == bufptr)
            Form("%s ", units[0]);

      temp = (int)(cents * 100. + .5);
      sprintf(bufptr, "& %02d/100", temp);

      return buf;
}

/*
**  Process each thousands group
*/

static void form_group(int amt, char *scale)
{
      if (buf != bufptr)
            *bufptr++ = ' ';

      if (100 <= amt)
      {
            Form("%s Hundred ", units[amt/100]);
            amt %= 100;
      }
      if (20 <= amt)
      {
            Form("%s", tens[(amt - 20)/10]);
            if (0 != (amt %= 10))
            {
                  Form("-%s ", units[amt]);
            }
                else    Form("%s", " ");
      }
      else if (amt)
      {
            Form("%s ", units[amt]);
      }

      Form("%s", scale);
}

#ifdef TEST

main(int argc, char *argv[])
{
      while (--argc)
      {
            double amt = atof(*(++argv));
            printf("fmt_money(%g) = %s\n", amt, fmt_money(amt));
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
