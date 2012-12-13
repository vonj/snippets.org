/*
**  LINE.C
**  Simple filter to add line-numbers to files
**
**  public domain demo by Bob Stout
**
**  Syntax:
**  LINE may be used at the end of a command-line or in the middle, e.g.
**
**  <type|cat> myfile.c | line | more
**  add line-numbers and show myfile.c one screen at a time
**
**  or 
**
**  <type|cat> myfile.c | line > lpt1
**  print listing with line numbers
**
**  or simply
**
**  line < myfile.c > file.out
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
      static unsigned long linenumber = 0;
      int ch, newline = 1;

      while (EOF != (ch = getchar()))
      {
            if (newline)
            {
                  printf("%06lu: ", ++linenumber);
                  newline = 0;
            }
            putchar(ch);
            if ('\n' == ch)
                  newline = 1;
      }
      return EXIT_SUCCESS;
}
