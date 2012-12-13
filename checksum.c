/*
**  CHECKSUM.C - Compute the checksum of a file
**
**  public domain demo by Bob Stout
*/

#include "crc.h"

unsigned checksum(void *buffer, size_t len, unsigned int seed)
{
      unsigned char *buf = (unsigned char *)buffer;
      size_t i;

      for (i = 0; i < len; ++i)
            seed += (unsigned int)(*buf++);
      return seed;
}

#ifdef TEST

#include <stdio.h>

main()
{
      FILE *fp;
      size_t len;
      char buf[4096], *file = "CHECKSUM.C";

      if (NULL == (fp = fopen(file, "rb")))
      {
            printf("Unable to open %s for reading\n", file);
            return -1;
      }
      len = fread(buf, sizeof(char), sizeof(buf), fp);
      printf("%d bytes read\n", len);
      printf("The checksum of %s is %#x\n", file, checksum(buf, len, 0));
      return 0;
}

#endif /* TEST */
