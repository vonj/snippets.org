/* 
**  sstrdel() - Delete multiple substrings
**
**  public domain by Shamim Islam
**
**  Usage: sstrdel(char * s,char * del0,del1...deln)
**
**  Remarks: sstrdel takes a string s, and removes all occurrences of
**           the substrings del0, del1, ... deln
**  
**  Return:  sstrdel returns a pointer to the string s, unless s is NULL.
**           sstrdel will return a NULL for this exception.
**
**  Comment: Use sstrdel to remove a list of substrings from a string.
**
**           sstrdel matches the largest substring for deletion, if more than
**           one substring matches a particular portion of the string s.
**
**  NOTE:    The last element in the variable substring list MUST be NULL
**           or your program will have a high likelihood of hanging.
*/

#include <stdarg.h>
#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *sstrdel(char *s, ...)
{
      /* Find out how many arguments are present */

      int c = 0;
      va_list ap, hold;

      if (s == NULL)
            return NULL;
      va_start(ap, s);
      memcpy(&hold, &ap, sizeof(va_list));
      while (va_arg(ap, char*) != NULL)
            c++;
      va_end(ap);
      if (c)
      {
            /* Assign pointers  */

            char *r = s,*n = s; 
            char *p;
            int len, i;

            /* Copy next character to result */
            /* And then check for matches if */
            /* not at end of string          */

            while ((*r = *n) != 0)
            {
                  int l = 0;

                  /* Substitute for va_start(ap,s) */

                  memcpy(&ap, &hold, sizeof(va_list));
                  for (i = 0; i < c; i++)
                  {
                        /* Initialise the pointer and the length    */

                        len = strlen(p = va_arg(ap, char*));

                        /* Compare ONLY if we haven't found one yet */
                        /* or if this one is bigger than the one we */
                        /* found AND this arg has a length > 0      */

                        if(len > 0 && (l == 0 || len> l) &&
                              strncmp(n, p, len) == 0)
                        {
                              l = len;
                        }
                  }
                  va_end(ap);
                  if (l)
                        n += l;
                  else  n++, r++;
            }
      }
      return s;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>

main()
{
      char *testr = "The quick brown fox jumps over the lazy dog.";
      char *s1 = "quick", *s2 = "over", *s3 = "lazy";
      char *s4 = "he", *s5 = "xyz";

      printf("sstrdel(\"%s\",\"%s\",\"%s\",\"%s\")\n returned ",
            testr, s1, s2, s3);
      printf("\"%s\"\n", sstrdel(testr, s1, s2, s3, NULL));

      printf("sstrdel(\"%s\",\"%s\",\"%s\")\n returned ",
            testr, s4, s5);
      printf("\"%s\"\n", sstrdel(testr, s4, s5, NULL));
      return 0;
}

#endif /* TEST */
