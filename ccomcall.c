/*
**  Call undocumented Int 2Eh to invoke COMMAND.COM
**
**  demo by Bob Stout
**
**  NOTES: Dangerous code - will abort batch files in progress and
**         occasionally have other undesirable effects.
**
**         Requires INT2E.ASM
*/

#include <string.h>
#include <stdlib.h>
#include "int2e.h"

void C_Com_Call(char *string)
{
      char *buf;

      buf = (char *)malloc(strlen(string) + 3);
      strcat(strcpy(&buf[1], string), "\r");
      buf[0] = (char)strlen(&buf[1]);
      _Int_2E(buf);
      free(buf);
}

#ifdef TEST

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif

#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      C_Com_Call(argv[1]);
      return 0;
}

#endif /* TEST */
