/*
**  MEMMEM.C - A strstr() work-alike for non-text buffers
**
**  public domain by Bob Stout
**
**  Warning: The memchr() in Borland C/C++ versions *prior* to 4.x is broken!
*/

#include <string.h>
#include "snip_str.h"

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

void *memmem(const void *buf, const void *pattern, size_t buflen, size_t len)
{
#if defined(__TURBOC__) && (__TURBOC__ >= 0x500)

      char *bf = (char *)buf, *pt = (char *)pattern, *p = bf;

      while (len <= (buflen - (p - bf)))
      {
            if (NULL != (p = memchr(p, (int)(*pt), buflen - (p - bf))))
            {
                  if (Success_ == memcmp(p, pattern, len))
                        return p;
                  else  ++p;
            }
            else  break;
      }
      return NULL;

#else /* Borland/Turbo C/C++ version prior to 4.x */

      size_t i, j;
      char *bf = (char *)buf, *pt = (char *)pattern;

      if (len > buflen)
            return (void *)NULL;

      for (i = 0; i <= (buflen - len); ++i)
      {
            for (j = 0; j < len; ++j)
            {
                  if (pt[j] != bf[i + j])
                        break;
            }
            if (j == len)
                  return (bf + i);
      }
      return NULL;

#endif
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>

main()
{
      char buf[13] = "\0""12344567890\x1b";
      char a[3] = "456";
      char b[3] = "\0""12";
      char c[3] = "90\x1b";
      char d[3] = "ABC";
      char e[3] = "0\x1b""\0";
      char f[1] = "\x1b";
      char *ptr;

      if (NULL == (ptr = memmem(buf, a, 13, 3)))
            puts("a not found in buf");
      else  printf("a found in buf at posn %d\n", ptr - buf);

      if (NULL == (ptr = memmem(buf, b, 13, 3)))
            puts("b not found in buf");
      else  printf("b found in buf at posn %d\n", ptr - buf);

      if (NULL == (ptr = memmem(buf, c, 13, 3)))
            puts("c not found in buf");
      else  printf("c found in buf at posn %d\n", ptr - buf);

      if (NULL == (ptr = memmem(buf, d, 13, 3)))
            puts("d not found in buf");
      else  printf("d found in buf at posn %d\n", ptr - buf);

      if (NULL == (ptr = memmem(buf, e, 13, 3)))
            puts("e not found in buf");
      else  printf("e found in buf at posn %d\n", ptr - buf);

      if (NULL == (ptr = memmem(buf, f, 13, 1)))
            puts("f not found in buf");
      else  printf("f found in buf at posn %d\n", ptr - buf);
      
      return 0;
}

#endif /* TEST */
