/*
**  STRINGIZ.H - Macros to use the ANSI/ISO preprocessor "stringize" macro
*/

#ifndef STRINGIZ__H
#define STRINGIZ__H

#define stringize(y) #y
#define add_quotes(x) stringize (x)

#ifdef TEST

#include <stdio.h>
#include <string.h>

#define FILENAME  MYFILE.XYZ

int main()
{
      char tmp[80];

      strcpy(tmp, "File: ");
      strcat(tmp, add_quotes(FILENAME));
      printf("%s\n", tmp);
      return 0;
}

#endif /* TEST */

#endif /* STRINGIZ__H */
