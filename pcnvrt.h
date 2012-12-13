/*
**  demo code for converting Pascal strings to/from C strings
**
**  public domain by Bob Stout
*/

#include <string.h>
#include "sniptype.h"

#define P2Cconvert(s) do \
            {BYTE n = *(s); memmove((s), &(s)[1], n); s[n] = '\0';} while(0)
#define C2Pconvert(s) do\
            {int n = strlen(s); memmove(&(s)[1], (s), n); *(s) = n;} while(0)

#if (0)                             /* Demo code fragment follows */

      char string[81];

      fgets(string, 81, inFile);    /* get 80-char C string       */
      C2Pconvert(string);           /* convert it in place        */
      P2Cconvert(string);           /* convert back               */

#endif
