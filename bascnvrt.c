/*
**  BASCNVRT.C - Convert between number bases
**
**  public domain demo by Bob Stout
*/

#include <stdlib.h>
#include "extkword.h"
#include "numcnvrt.h"

/*
**  Calling parameters: 1 - Number string to be converted
**                      2 - Buffer for the  converted output
**                      3 - Radix (base) of the input
**                      4 - Radix of the output
**
**  Returns: Pointer to converted output
**
**  Uses LTOSTR.C from SNIPPETS
*/

char *base_convert(const char *in, char *out, size_t len, int rin, int rout)
{
      long n;
      char *dummy;

      n = strtol(in, &dummy, rin);
      return ltostr(n, out, len, rout);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
      int rin, rout;
      char buf[40];

      if (4 > argc)
      {
            puts("Usage: BASCNVRT <number> <base_in> <base_out>");
            return(-1);
      }
      rin  = atoi(argv[2]);
      rout = atoi(argv[3]);
      printf("%s (base %d) = %s (base %d)\n", argv[1], rin,
            base_convert((const char *)argv[1], buf, 40, rin, rout), rout);
      return 0;
}

#endif /* TEST */
