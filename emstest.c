/*
** EMSTEST.C
**
** Test EMS functions found in ems.c
**
** Released to the public domain by Cliff Rhodes with no guarantees
** of any kind.
*/

#include <stdio.h>
#include <dos.h>

#include "ems.h"

int main(void)
{
      const char str[] = "This is just a test string!";
      int i, ver, handle1, handle2;
      unsigned int baseaddress;
      char far *cp;

      /* Get the segment for mapping expanded memory into address space */

      if((baseaddress = EMSbaseaddress()) == 0)
      {
            printf("Expanded memory manager not found, "
                   "terminating program.\n");
            return 1;
      }

      /* Get the version number an the number of pages available */

      ver = EMSversion();
      printf("EMM Version %d.%d loaded\n", ver >> 4, ver & 0x0f);
      printf("There are %d pages available\n", EMSpages());

      /* Allocate a few pages */

      handle1 = EMSalloc(3);   /* Allocate 3 pages */
      if(handle1 < 0)
      {
            printf("EMM allocation failed on handle 1\n");
            return 1;
      }
      handle2 = EMSalloc(1);   /* Allocate 1 page */
      if(handle2 < 0)
      {
            EMSfree(handle1);
            printf("EMM allocation failed on handle 2\n");
            return 1;
      }
      printf("There are %d pages available after allocating 4\n", EMSpages());

      /* Map the allocated pages into the address space */

      if(EMSmap(0, handle1, 1) < 0)            /* Mapped page 1 into bank 0 */
            printf("Error mapping handle 1\n");
      if(EMSmap(3, handle2, 0) < 0)            /* Mapped page 0 into bank 3 */
            printf("Error mapping handle 2\n");
      else
      {
            /* Write test string into beginning of handle2 */

            cp = (char far *) (MK_FP(baseaddress, 3 * EMS_PAGE_SIZE));
            for(i = 0; str[i]; i++)
                  cp[i] = str[i];
            cp[i] = '\0';

            /* Read back test string and look for a match */

            cp = (char far *) (MK_FP(baseaddress, 3 * EMS_PAGE_SIZE));
            for(i = 0; str[i]; i++)
            {
                  if(cp[i] != str[i])
                  {
                        printf("Data mismatch at character %d\n", i);
                        break;
                  }
            }
      }
      EMSfree(handle1);
      EMSfree(handle2);

      return 0;
}
