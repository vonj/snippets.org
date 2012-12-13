/*
**  RULER.C(PP) - A utility to create text ruler lines
**
**  R.F. Pels. Dec. 1993. Placed in public domain.
*/

#include <string.h>
#include <stdio.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char * rule_line(char * s,
                 unsigned short len,
                 short units,
                 char * digits,
                 char filler)
{
      /*  If possible, initialize directly with correct value!    */
      short           whichdigit = 0;
      short           digitlen = strlen(digits);
      unsigned short  i;

      memset(s, filler, len);             /*  Fill string with all filler   */
      s[len]  = NUL;                      /*  Tack on an ASCIIZ             */

      for (i = 0; i < len; i += units)
      {
            s[i] = digits[whichdigit];    /*  Put in digit                  */
            whichdigit++;                 /*  Add 1 and reset to 0 if...    */
            whichdigit %= digitlen;       /* ...bigger than length of digits*/
      }
      return s;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

char text[41] = "abcdefghijabcdefhijkabcdefghijabcdefghij", ruler[41];

main(void)
{
      puts(text);
      printf("%s\n\n", rule_line(ruler, 40,   1, "123", ' '));
      puts(text);
      printf("%s\n\n", rule_line(ruler, 40,   1, "12345", ' '));
      puts(text);
      printf("%s\n\n", rule_line(ruler, 40,   1, "1234567890", ' '));
      puts(text);
      printf("%s\n\n", rule_line(ruler, 40,   5, "0123456789", ' '));
      puts(text);
      printf("%s\n\n", rule_line(ruler, 40,  10, "1234567890", ' '));
      return 0;
}

#endif /* TEST */
