/*
** GETSTRNG.C -- Demonstration of dynamic memory allocation to
**               receive string of unknown length.
**
** Ron Sires 1/31/89, released to the public domain.
** Bob Stout 2/18/93, modified to use a static buffer
*/
 
#include <stdlib.h>
#include <stdio.h>
#include "editgets.h"
 
#define BLOCKSIZ    16
 
char *getstring(void)
{
      int    newchar;
      size_t i;
      static size_t bufsize = 0;
      static char  *buffer  = NULL;

      /* Get chars from keyboard and put them in buffer.  */

      for (i = 0; ((newchar = getchar()) != EOF) && (newchar != '\n')
            && (newchar != '\r'); ++i )
      {
            if (i + 1 > bufsize)
            {
                  /* If buffer is full, resize it. */

                  if (NULL == (buffer = realloc(buffer, bufsize + BLOCKSIZ)))
                  {
                        puts("\agetstrng() - Insufficient memory");

                        /* Add terminator to partial string */

                        if (buffer)
                              buffer[i] = '\0';
                        return buffer;
                  }
                  bufsize += BLOCKSIZ;
            }
            buffer[i] = (char) newchar;
      }
      buffer[i] = '\0';   /* Tack on a null-terminator. */
      return buffer;
}

#ifdef TEST

#include <string.h>

int main(void)
{
      char *string;

      puts("Enter strings of any length or <Enter> to quit\n");
      do
      {
            string = getstring();
            printf("You entered:\n\"%s\"\n\n", string);
      } while (strlen(string));
      free(string);
      return EXIT_SUCCESS;
}

#endif /* TEST */
