/*************************************************************************

   REGIT.C - A very simple registration key generator. Uses simple XOR
   manipulations of a string to create a key.

   It is NOT foolproof, but it will work.

   Donated to the Public Domain by Craig Morrison 12 May 1994, use,
   abuse, fold, spindle or mutilate anyway you see fit.

*************************************************************************/

#include "regkey.h"

/*************************************************************************

    REGIT accepts one argument on the command line; The string you want
    to use to generate a key from. It outputs the generated key in both
    decimal and hexadecimal form. Spaces in the argument should have the
    '_' character used in their place, they get translated below.

*************************************************************************/

int main(int argc, char *argv[])
{
      long keyval = (long)XOR_PRIME;
      long key;
      char *p;
      char buf[128];

      if (argc>1)
      {
            strcpy(buf, argv[1]);
            p = buf;
            while(*p)
            {
                  if (*p=='_')
                        *p = ' ';

                  key = (long) toupper(*p);
                  key ^= (long)XOR_CRYPT;
                  keyval ^= key;
                  p++;
            }
            keyval ^= (long)XOR_POST_CRYPT;
            printf("Key value = %08lX hex, %lu decimal.\n", keyval, keyval);
      }
      return 0;
}
