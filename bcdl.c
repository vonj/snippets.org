/*-----------------01-10-95 02:38pm-----------------
--  BCD conversion routines, released to the public
--  domain by Jeff Dunlop
--------------------------------------------------*/

#include "sniptype.h"         /* For True_, False_    */
#include "snipmath.h"

static int BCDLen = 8;

void SetBCDLen(int n)
{
      BCDLen = n;
}

long BCDtoLong(char *BCDNum)
{
      int i, high, low;
      Boolean_T neg;

      long total = 0;

      for (i = 0; i < BCDLen - 1; i++)
      {
            low    = 0x0f & BCDNum[i];
            high   = (0xf0 & BCDNum[i]) >> 4;
            total *= 10;
            total += high;
            total *= 10;
            total += low;
      }
      high   = (BCDNum[BCDLen - 1] & 0xf0) >> 4;
      low    = BCDNum[BCDLen - 1] & 0x0f;
      total *= 10;
      total += high;

      neg = (low == 0x0c) ? 1 : -1;

      return total * neg;
}

void LongtoBCD(long num, char BCDNum[])
{
      int i, high, low;
      Boolean_T neg = False_;

      if ( num < 0 )
      {
            neg = True_;
            num = 0 - num;
      }

      low  = neg ? 0x0d : 0x0c;
      high = (int)(num % 10);
      num /= 10;
      BCDNum[BCDLen - 1] = (char)((high << 4) | low);

      for (i = BCDLen - 2; i >= 0; i--)
      {
            low  = (int)(num % 10);
            num /= 10;
            high = (int)(num % 10);
            num /= 10;
            BCDNum[i] = (char)((high << 4) | low);
      }
}

#ifdef TEST

#include <stdio.h>

void show_BCD(char c[]);

int main(void)
{
      long a = 12345678L,
           b;
      char c[10];

      SetBCDLen(10);

      LongtoBCD(a, c);
      show_BCD(c);
      b= BCDtoLong(c);
      printf("Value is %ld\n", b);

      a = -a;
      LongtoBCD(a, c);
      show_BCD(c);
      b= BCDtoLong(c);
      printf("Value is %ld\n", b);

      return 0;
}

void show_BCD(char c[])
{
      int i;

      printf("BCD: ");
      for (i = 0; i < 10; ++i)
      {
            printf("[%d]", c[i] >> 4);
            if (9 == i)
                  printf("<sign>");
            printf("[%d]", c[i] &0xf);
      }
      puts("");
}

#endif /* TEST */
