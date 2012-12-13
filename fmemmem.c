/*
**  FMEMMEM.C - A strstr() work-alike for large non-text buffers
**
**  public domain by Bob Stout
*/

#include "snip_str.h"
#include "snpdosys.h"                     /* For addptr() & farnormal() */

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

void FAR *fmemmem(const void FAR *buf,
                  const void FAR *pattern,
                  long            buflen,
                  long            len)
{
      long i, j;
      char FAR *bf = (char FAR *)buf, FAR *pt = (char FAR *)pattern;

      if (len > buflen)
            return (void FAR *)NULL;

      for (i = 0L; i <= (buflen - len); ++i)
      {
            for (j = 0L; j < len; ++j)
            {
                  /*
                  ** Not all compilers support huge pointers the same way
                  ** (or at all!), so add the offsets to the pointers,
                  ** normalize them, then dereference them.
                  */

                  char FAR *pp = addptr(pt, j);       /* &pt[j]		*/
                  char FAR *bb = addptr(bf, i + j);   /* &bf[i + j]     */

                  if (*pp != *bb)                     /* pt[j]==bf[i+j] */
                        break;
            }
            if (j == len)
                  return addptr(bf, i);
      }
      return (void FAR *)NULL;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

#include <stdio.h>

main()
{
      char FAR buf[13] = "\0""12344567890\x1b";
      char FAR a[3] = "456";
      char FAR b[3] = "\0""12";
      char FAR c[3] = "90\x1b";
      char FAR d[3] = "ABC";
      char FAR e[3] = "0\x1b""\0";
      char FAR f[1] = "\x1b";
      char FAR *ptr;
      long lp, lb;

      if (NULL == (ptr = fmemmem(buf, a, 13L, 3L)))
            puts("a not found in buf");
      else
      {
            lp = (long)farnormal(ptr);
            lb = (long)farnormal(buf);
            printf("a found in buf at posn %ld\n", lp - lb);
      }

      if (NULL == (ptr = fmemmem(buf, b, 13L, 3L)))
            puts("b not found in buf");
      else
      {
            lp = (long)farnormal(ptr);
            lb = (long)farnormal(buf);
            printf("b found in buf at posn %ld\n", lp - lb);
      }

      if (NULL == (ptr = fmemmem(buf, c, 13L, 3L)))
            puts("c not found in buf");
      else
      {
            lp = (long)farnormal(ptr);
            lb = (long)farnormal(buf);
            printf("c found in buf at posn %ld\n", lp - lb);
      }

      if (NULL == (ptr = fmemmem(buf, d, 13L, 3L)))
            puts("d not found in buf");
      else
      {
            lp = (long)farnormal(ptr);
            lb = (long)farnormal(buf);
            printf("d found in buf at posn %ld\n", lp - lb);
      }

      if (NULL == (ptr = fmemmem(buf, e, 13L, 3L)))
            puts("e not found in buf");
      else
      {
            lp = (long)farnormal(ptr);
            lb = (long)farnormal(buf);
            printf("e found in buf at posn %ld\n", lp - lb);
      }

      if (NULL == (ptr = fmemmem(buf, f, 13L, 1L)))
            puts("f not found in buf");
      else
      {
            lp = (long)farnormal(ptr);
            lb = (long)farnormal(buf);
            printf("f found in buf at posn %ld\n", lp - lb);
      }
      
      return 0;
}

#endif /* TEST */
