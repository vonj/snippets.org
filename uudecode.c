/*
** by: John Lots
** patched up for BC++ 3.1 by Alan Eldridge 10/12/92
**     (UUCP: alane@wozzle.linet.org, FIDO: 1:272/38.473)
*/

#include        <stdio.h>
#include        <stdlib.h>
#define         DEC(c)  (char)(((c)-' ')&077)

int main()
{
      int     n;
      char    buf[128],a,b,c,d;

      scanf("begin %o ", &n);
      gets(buf);                              /* filename */
      if (!freopen(buf, "wb", stdout))         /* oops.. */
      {
            perror(buf);
            exit(1);
      }
      while ((n=getchar())!=EOF&&((n=DEC(n))!=0))
      {
            while (n>0)
            {
                  a=DEC(getchar());
                  b=DEC(getchar());
                  c=DEC(getchar());
                  d=DEC(getchar());
                  if (n-->0)
                        putchar((a<<2)|(b>>4));
                  if (n-->0)
                        putchar((b<<4)|(c>>2));
                  if (n-->0)
                        putchar((c<<6)|d);
            }
            n=getchar();                    /* skip \n */
      }
      return 0;
}
