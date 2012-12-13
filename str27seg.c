/*
**  STR27SEG.C - Convert numeric strings to 7-segment strings.
**
**  Public domain by Bob Stout
**
**  Input:  A string (NUL-delimited char array) containing only digits
**          ('0' - '9' chars).
**
**  Output: The same string with each digit converted to a 7-segment
**          representation. Returns NULL on error.
*/

#include <stdio.h>
#include <ctype.h>
#include "numcnvrt.h"

#define CAST(new_type,old_object) (*((new_type *)&old_object))
#define DISP(str) fputs((str), stdout)

/*
**  Define the bit significance
**
**     a
**    ---
**   |   |
**  f|   |b
**   | g |
**    ---
**   |   |
**  e|   |c
**   |   |
**    ---
**     d
*/

struct Seg7disp Seg7digits[10] = {
      { 1, 1, 1, 1, 1, 1, 0 },      /* 0 */
      { 0, 1, 1, 0, 0, 0, 0 },      /* 1 */
      { 1, 1, 0, 1, 1, 0, 1 },      /* 2 */
      { 1, 1, 1, 1, 0, 0, 1 },      /* 3 */
      { 0, 1, 1, 0, 0, 1, 1 },      /* 4 */
      { 1, 0, 1, 1, 0, 1, 1 },      /* 5 */
      { 1, 0, 1, 1, 1, 1, 1 },      /* 6 */
      { 1, 1, 1, 0, 0, 0, 0 },      /* 7 */
      { 1, 1, 1, 1, 1, 1, 1 },      /* 8 */
      { 1, 1, 1, 1, 0, 1, 1 }       /* 9 */
};

char *str27seg(char *string)
{
      char *str;
      int ch;

      for (str = string ; *str; ++str)
      {
            if (!isdigit(*str))
                  return NULL;
            ch = CAST(int, Seg7digits[*str - '0']);
            *str = (char)(ch & 0xff);
      }
      return string;
}

#ifdef TEST

main()
{
      char tstrng[] = "0123456789", *segs;

      printf("str27seg() returned %p", segs = str27seg(tstrng));
      if (segs)
      {
            char *ptr;
            struct Seg7disp ch;
            int i;

            puts(" & produced:\n");
            for (ptr = segs ; *ptr; ++ptr)
            {
                  ch = CAST(struct Seg7disp, *ptr);
                  if (ch.seg_a)
                        DISP(" --- ");
                  else  DISP("     ");
                  DISP("  ");
            }
            puts("");
            for (i = 0; i < 3; ++i)
            {
                  for (ptr = segs ; *ptr; ++ptr)
                  {
                        ch = CAST(struct Seg7disp, *ptr);
                        if (ch.seg_f)
                              DISP("|   ");
                        else  DISP("    ");
                        if (ch.seg_b)
                              DISP("|");
                        else  DISP(" ");
                        DISP("  ");
                  }
                  puts("");
            }
            for (ptr = segs ; *ptr; ++ptr)
            {
                  ch = CAST(struct Seg7disp, *ptr);
                  if (ch.seg_g)
                        DISP(" --- ");
                  else  DISP("     ");
                  DISP("  ");
            }
            puts("");
            for (i = 0; i < 3; ++i)
            {
                  for (ptr = segs ; *ptr; ++ptr)
                  {
                        ch = CAST(struct Seg7disp, *ptr);
                        if (ch.seg_e)
                              DISP("|   ");
                        else  DISP("    ");
                        if (ch.seg_c)
                              DISP("|");
                        else  DISP(" ");
                        DISP("  ");
                  }
                  puts("");
            }
            for (ptr = segs ; *ptr; ++ptr)
            {
                  ch = CAST(struct Seg7disp, *ptr);
                  if (ch.seg_d)
                        DISP(" --- ");
                  else  DISP("     ");
                  DISP("  ");
            }
            puts("");
      }
      else  puts("\n");

      return 0;
}

#endif /* TEST */
