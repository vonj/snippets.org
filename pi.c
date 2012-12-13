/*
**  PI.C - Computes Pi to an arbitrary number of digits
**
**  Uses far arrays when/where required so may be compiled in any memory model
**
**  The formula that most use (including the one in the Snippets) is the
**  classic Machin formula, with the Gregory series.
**  
**  pi=16arctan(1/5)-4arctan(1/239)
**  
**  And use the traditional Gregory arctangent series to calculate the
**  arctangents. That's the:
**  
**  arctan(x)=x-(x^3)/3+(x^5)/5-(x^7)/7.....
**  
**  With 1/5 and 1/239, it would be:
**  
**  arctan(x)=1/5-1/(3*5^3)+1/(5*5^5)-1/(7*5^7)...
**  arctan(x)=1/239-1/(3*239^3)+1/(5*239^5)-1/(7*239^7)....
**  
**  Doing the multi-precision isn't too hard, since we don't really need to
**  have a general purpose math package.  We can hardwire it all.
**
**  Due to the % operator, ms[i] < (temp * (239**2)) so
**  temp < 3759 and i < 1879, it fails at the 1879th term which translates to
**  1879 * log10(239**2) == 8941th decimal.
**
**  In practice we get a few more digits, (2 -> 8943th)
*/

#include <stdio.h>
#include <stdlib.h>
#include "big_mall.h"

long kf, ks;
long FAR *mf, FAR *ms;
long cnt, n, temp, nd;
long i, line;
long col, col1;
long loc, FAR stor[4096];

static void PASCAL shift(long FAR *l1, long FAR *l2, long lp, long lmod)
{
      long k;

      k = ((*l2) > 0 ? (*l2) / lmod: -(-(*l2) / lmod) - 1);
      *l2 -= k * lmod;
      *l1 += k * lp;
}

static void PASCAL yprint(long m)
{
      if (cnt<n)
      {
            if (++col == 11)
            {
                  col = 1;
                  if (++col1 == 5)
                  {
                        col1 = 0;
                        printf("\nL%04ld:", ++line);
                        printf("%4ld",m%10);
                  }
                  else  printf("%3ld",m%10);
            }
            else  printf("%ld",m);
            cnt++;
      }
}

static void PASCAL xprint(long m)
{
      long ii, wk, wk1;

      if (m < 8)
      {
            for (ii = 1; ii <= loc; )
                  yprint(stor[(int)(ii++)]);
            loc = 0;
      }
      else
      {
            if (m > 9)
            {
                  wk = m / 10;
                  m %= 10;
                  for (wk1 = loc; wk1 >= 1; wk1--)
                  {
                        wk += stor[(int)wk1];
                        stor[(int)wk1] = wk % 10;
                        wk /= 10;
                  }
            }
      }
      stor[(int)(++loc)] = m;
}

static void PASCAL memerr(int errno)
{
        printf("\a\nOut of memory error #%d\n", errno);
        if (2 == errno)
                BigFree(mf);
        _exit(2);
}

int main(int argc, char *argv[])
{
      int i=0;
      char *endp;

      stor[i++] = 0;
      if (argc < 2)
      {
            puts("\aUsage: PI <number_of_digits>");
            return(1);
      }
      n = strtol(argv[1], &endp, 10);
      mf = BigMalloc((Size_T)(n + 3L), (Size_T)sizeof(long));
      if (NULL == mf)
            memerr(1);
      ms = BigMalloc((Size_T)(n + 3L), (Size_T)sizeof(long));
      if (NULL == ms)
            memerr(2);
      printf("\nApproximation of PI to %ld digits\n", (long)n);
      cnt = 0;
      kf = 25;
      ks = 57121L;
      mf[1] = 1L;
      for (i = 2; i <= (int)n; i += 2)
      {
            mf[i] = -16L;
            mf[i+1] = 16L;
      }
      for (i = 1; i <= (int)n; i += 2)
      {
            ms[i] = -4L;
            ms[i+1] = 4L;
      }
      printf("\nL%04ld: 3.", ++line);
      while (cnt < n)
      {
            for (i = 0; ++i <= (int)n - (int)cnt; )
            {
                  mf[i] *= 10L;
                  ms[i] *= 10L;
            }
            for (i =(int)(n - cnt + 1); --i >= 2; )
            {
                  temp = 2 * i - 1;
                  shift(&mf[i - 1], &mf[i], temp - 2, temp * kf);
                  shift(&ms[i - 1], &ms[i], temp - 2, temp * ks);
            }
            nd = 0;
            shift((long FAR *)&nd, &mf[1], 1L, 5L);
            shift((long FAR *)&nd, &ms[1], 1L, 239L);
            xprint(nd);
      }
      printf("\n\nCalculations Completed!\n");
      BigFree(ms);
      BigFree(mf);
      return(0);
}
