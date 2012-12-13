/*
**  FPTR_ADD.C
**
**  Add any add any value to a far pointer and returns the result as a
**  normalized far pointer.
**
**  Public Domain by Soleil Lapierre
*/

#include "snpdosys.h"
#include "mk_fp.h"


void FAR *addptr (char FAR *p, unsigned long num)
{
      unsigned seg,off;
 
      seg = FP_SEG(p); off = FP_OFF(p);
      seg += off>>4;   off &= 0x000F;

      off += (unsigned)(num&0x0000000fL);

      seg += off>>4;   off &= 0x000F;
      seg += (unsigned)num>>4;

      return(MK_FP(seg,off));
}

/*
**  Normalize a far pointer
*/

void FAR *farnormal(void FAR *ptr)
{
      unsigned long base, para;

      base = ((unsigned long)(ptr) & 0xffff000fL);
      para = ((unsigned long)(ptr) & 0x0000fff0L);
      para <<= 12;
      return (void FAR *)(base + para);
}
