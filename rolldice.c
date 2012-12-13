/*_|_|   ROLLDICE.C
_|_|_|   A function to roll a definable number of dice (1 - 100),
_|_|_|   with a definable number of sides (2 to 255).
_|_|_|   No warrantees or guarantees are given or implied.
_|_|_|   Released to   PUBLIC DOMAIN   by Kurt Kuzba.  (8/25/96)
*/

#include "rolldice.h"                     /* Also includes stdlib.h     */

static int rnd(int min, int max)
{
      long t = (long)rand() * (long)(max - min + 1);
      return min + (int)(t / RAND_MAX);
}

char *roll_dice(int d, int s)
{
      char *result = malloc(d * s + 1);
      int shuffle_loop, die, side, temp, random;

      if (result == NULL)
            return NULL;
      *(result + d * s) = '\0';

      /* initialize your dice */

      for (die = 0; die < d; die++)
      {
            for (side = 0; side < s; side++)
                  *(result + die * s + side) = (char)(side + 1);
      }
      for (shuffle_loop = 0; shuffle_loop < 16; shuffle_loop ++)
      {
            /* 'roll' your dice     */

            for (die = 0; die < d; die++)
            {
                  for (side = 0; side < s; side++)
                  {
                        random = rnd(1, s) - 1;
                        temp = *(result + die * s + side);
                        *(result + die * s + side) =
                              *(result + die * s + random);
                        *(result + die * s + random) = (char)temp;
                  }
            }
      }
      for (die = 0; die < d; die++)    /* Make an 'upside' string */
            *(result + die) = *(result + die * s);
      *(result + d) = '\0';           /* Trim str for # of dice  */
      return result;
}

#ifdef TEST

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <conio.h>

int main(int c, char *v[])
{
      time_t t;
      int dice, sides;
      char *d;

      srand((unsigned)time(&t));
      dice = atoi(v[1]);
      d = strchr(v[1], 'd');
      sides = atoi(++d);
      if (d == NULL || dice < 1 || dice > 100 || sides < 2 || sides > 255)
            return !!puts("USAGE EX: ROLLDICE 2d6   (rolls 2 6-sided dice)");
      d = roll_dice(dice, sides);
      if (d == NULL)
            return !puts("Malloc Failure.");
      dice = sides = 0;
      while (d[dice])
      {
            sides += d[dice];
            printf("%d ", d[dice++]);
      }
      printf("\nYour total = %d\n", sides);
      free(d);   /* Make sure you free your dice after every usage!! */
      return 0;  /* Otherwise, you will leak away all your memory!!! */
}

#endif /* TEST */

/*_|_|   end   ROLLDICE.C   */
