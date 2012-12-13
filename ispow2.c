#include <stdio.h>
#include "snipmath.h"

int ispow2(int x)
{
      return! ((~(~0U>>1)|x)&x -1) ;
}

#ifdef TEST

int main(void)
{
      int i;

      for (i = 0; i < 256; ++i)
            printf("%3d: %d\n", i, ispow2(i));
      return 0;
}

#endif /* TEST */
