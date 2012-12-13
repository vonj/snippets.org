/*
**  Form a command string for ANSI.SYS to set a given video attribute
**
**  Public domain demo by Bob Stout
*/

#include <string.h>

#include "scrnmacs.h"

static void add_str(char *, char *);

/*
**  Example:
**   Video attribute of yellow text on blue background = BG_(BLUE_)+YELLOW_
*/

char *make_ansi(int vatr)
{
        static char string[40];

        static char *fore[8] = {"30","34","32","36","31","35","33","37"};
        static char *back[8] = {"40","44","42","46","41","45","43","47"};

        strcpy(string, "\033[");
        if (vatr == 0x07)
                strcat(string, "0");
        else
        {
                if (vatr & 0x80)
                        add_str(string, "5");
                if (vatr & 0x08)
                        add_str(string, "1");
                add_str(string, fore[vatr & 0x07]);
                add_str(string, back[(vatr & 0x70) >> 4]);
        }
        strcat(string, "m");
        return string;
}

static void add_str(char *string1, char *string2)
{
        char last_char;

        last_char = string1[strlen(string1) - 1];
        if (last_char != '[')
                strcat(string1, ";");
        strcat(string1, string2);
}
