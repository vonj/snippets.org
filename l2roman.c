/*
**        L2ROMAN.C  -  Converts long integers to Roman numerals
**
**        Jim Walsh, Dann Corbit, Bob Stout, and Others made this.
**
**        This Program Is Released To The Public Domain
**
** ----------------------------------------------------------------------
** Compiling:
**    If the symbol ALLOW_BAR_NOTATION is defined, then bar notation is
**    allowed.  If the Romans wanted to record one million, they did not
**    write down 1000 M's.  They would right one M with a bar over it.
**    Any Roman numeral with a bar over it is multiplied by 1000.
**    Unfortunately, this is not a standard character in most character sets.
**    If you choose to #define ALLOW_BAR_NOTATION, then you will have to
**    translate the symbols to final form yourself.
*/

#include <stdlib.h>
#include "sniptype.h"

typedef struct tag_RomanToDecimal {
      long  PostValue;
      char *PostLetter;
      long  PreValue;
      char *PreLetter;
} R2D;

/*
**  Set PrefixesAreOK = True_ to enable prefix notation (e.g. 4 = "IV")
**  Set PrefixesAreOK = False_ to disable prefix notation (e.g. 4 = "IIII")
*/

static Boolean_T PrefixesAreOK = True_;

static R2D RomanConvert[] = {
#if defined(ALLOW_BAR_NOTATION)
        /*
        ** A Roman numeral with a bar over it is
        ** that value multiplied by 1000.
        */
        {1000000L, "<M-bar>", 900000L, "<CM-bar>"},
        {500000L, "<D-bar>", 400000L, "<CD-bar>"},
        {100000L, "<C-bar>", 90000L, "<XC-bar>"},
        {50000L, "<L-bar>", 40000L, "<XL-bar>"},
        {10000L, "<X-bar>", 9000L, "<IX-bar>"},
        {5000L, "<V-bar>", 4000L, "<IV-bar>"},
#endif
        {1000L, "M", 900L, "CM"},
        {500L, "D", 400L, "CD"},
        {100L, "C", 90L, "XC"},
        {50L, "L", 40L, "XL"},
        {10L, "X", 9L, "IX"},
        {5L, "V", 4L, "IV"},
        {1L, "I", 1L, "I"}
    };

/*
**  long2roman() - Convert a long integer into roman numerals
**
**  Arguments: 1 - Value to convert
**             2 - Buffer to receive the converted roman numeral string
**             3 - Length of the string buffer
**
**  Returns: Pointer to the buffer, else NULL if error or buffer overflow
*/

char *long2roman(long val, char *buf, size_t buflen)
{
      size_t posn = 0;
      size_t place = 0;

#if !defined(ALLOW_BAR_NOTATION)
      if (val > 3999L)
            return NULL;
#endif
      do
      {
            while (val >= RomanConvert[place].PostValue)
            {
                  posn += sprintf(&buf[posn], "%s",
                                  RomanConvert[place].PostLetter);
                  val -= RomanConvert[place].PostValue;
                  if (posn >= buflen)
                        return NULL;
            }
            if (PrefixesAreOK)
            {
                  if (val >= RomanConvert[place].PreValue)
                  {
                        posn += sprintf(&buf[posn], "%s",
                                        RomanConvert[place].PreLetter);
                        val -= RomanConvert[place].PreValue;
                        if (posn >= buflen)
                              return NULL;
                  }
            }
            place++;
      } while (val > 0);

      return buf;
}

#ifdef TEST

#include <stdio.h>

int main(int argc, char *argv[])
{
      long value;
      char buf[128];

      while (--argc)
      {
            value = atol(*(++argv));
            printf("\n%ld = %s\n", value, long2roman(value, buf, 128));
      }

      return 0;
}

#endif /* TEST */
