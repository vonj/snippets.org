/*
** XMSTEST.C
**
** Test extended memory routines in file XMS.C
**
** Released to the public domain by Cliff Rhodes with no guarantees
** of any kind.
*/

#include <stdio.h>
#include <stdlib.h>

#include "xms.h"

#define BufSize            30000
#define BlockSize  (long) 130000L

int main(void)
{
      int i;
      unsigned int handle1, handle2;       /* XMS handles */
      char *buf1, *buf2;

      if ((i = XMSinit()) == 0)
      {
            printf("Extended Memory Manager not found, terminating program\n");
            return 1;
      }
      i = XMSversion();
      printf("Extended Memory Manager Version %d.%d is installed\n",
             i >> 8, (i & 0xff));

      printf("There are %ld extended memory bytes available\n", XMScoreleft());

      buf1 = malloc(BufSize);
      buf2 = malloc(BufSize);
      if (buf1 == NULL || buf2 == NULL)
      {
            printf("Error allocating conventional memory\n");
            return 1;
      }
      for (i = 0; i < BufSize; i++)   /* Fill buf1 with some values */
      {
            buf1[i] = i % 255;
            buf2[i] = 0;
      }
      handle1 = XMSalloc(BlockSize);
      if (handle1 == 0)
      {
            printf("Error allocating XMS memory for handle 1\n");
            return 1;
      }
      printf("Handle 1 is %u and represents %ld bytes\n", handle1, BlockSize);
      printf("There are %ld extended memory bytes available\n", XMScoreleft());

      handle2 = XMSalloc(BlockSize);
      if (handle2 == 0)
      {
            XMSfree(handle1);
            printf("Error allocating XMS memory for handle 2\n");
            return 1;
      }
      printf("Handle 2 is %u and represents %ld bytes\n", handle2, BlockSize);
      printf("There are %ld extended memory bytes available\n", XMScoreleft());

      /* Copy data from DOS buf1 to XMS memory of handle1 */

      if (DOStoXMSmove(handle1, 0L, buf1, (long) BufSize) != BufSize)
            printf("DOStoXMS copy failed\n");

      /* Copy XMS handle1 data to XMS handle2 */

      if (XMSmemcpy(handle2, 0L, handle1, 0L, BlockSize) != BlockSize)
            printf("XMS copy failed\n");

      /* Copy XMS handle2 data to DOS buf2 */

      if (XMStoDOSmove(buf2, handle1, 0L, (long) BufSize) != BufSize)
            printf("XMStoDOS copy failed\n");

      /* buf1 data should now == buf2 data */

      for (i = 0; i < BufSize; i++)
      {
            if (buf1[i] != buf2[i])
            {
                  printf("*** ERROR: Mismatch in DOS buffers at "
                         "location %d ***\n", i);
                  break;
            }
      }
      free(buf1);
      free(buf2);

      XMSfree(handle2);
      printf("There are %ld bytes available after freeing handle 2\n",
             XMScoreleft());

      XMSfree(handle1);
      printf("There are %ld bytes available after freeing handle 1\n",
             XMScoreleft());

      return 0;
}
