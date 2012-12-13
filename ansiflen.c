/*
**  FLENGTH.C - a simple function using all ANSI-standard functions
**              to determine the size of a file.
**
**  Public domain by Bob Jarvis.
*/

#include "snipfile.h"

#if defined(__cplusplus) && __cplusplus /* C++ version follows */

 #include <fstream.h>

 long flength(char *fname)
 {
       long length = -1L;
       ifstream ifs;

       ifs.open(fname, ios::binary);
       if (ifs)
       {
             ifs.seekg(0L, ios::end) ;
             length = ifs.tellg() ;
       }
       return length;
 }

#else /* Straight C version follows */

 long flength(char *fname)
 {
       long length = -1L;
       FILE *fptr;

       fptr = fopen(fname, "rb");
       if(fptr != NULL)
       {
             fseek(fptr, 0L, SEEK_END);
             length = ftell(fptr);
             fclose(fptr);
       }

       return length;
 }

#endif /* C++ */

#ifdef TEST

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      char *ptr;
      long len;

      while (--argc)
      {
            len = flength(ptr = *(++argv));
            if (-1L == len)
                  printf("\nUnable to get length of %s\n", ptr);
            else  printf("\nLength of %s = %ld\n", ptr, len);
      }
      return 0;
}

#endif /* TEST */
