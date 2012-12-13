/*
** A quick "life" (2-d cellular automaton) implementation done in Turbo C 2.0 
** on the spur-of-the-moment by Jonathan Guthrie 9/20/1992 and donated to the 
** public domain.
**
** In keeping with the guidelines of the C_ECHO, this program has been tested,
** and does seem to operate properly.
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
 
#ifndef random
 #define random(num) (int)(((long)rand()*(num))/RAND_MAX)
#endif
 
/*
**  From VIDPORT.C, also in SNIPPETS
*/
 
void GotoXY(int col, int row);
void ClrScrn(int vattrib);
 
#ifndef randomize
 #define randomize() srand(((unsigned int)time(NULL))|1)
#endif

#define     ROWS        24
#define     COLS        80
#define     GENERATIONS 10

int civ1[ROWS][COLS], civ2[ROWS][COLS];

void update_generation(int old[ROWS][COLS], int new[ROWS][COLS])
{
      int i, j, count;

      for (i = 0; i < ROWS; ++i)
      {
            for (j = 0; j < COLS; ++j)
            {
                  count = old[(i + ROWS - 1) % ROWS][(j + COLS - 1) % COLS] + 
                        old[(i + ROWS - 1) % ROWS][j] +
                        old[(i + ROWS - 1) % ROWS][(j + 1) % COLS] +
                        old[i][(j + COLS - 1) % COLS] +
                        old[i][(j + 1) % COLS] +
                        old[(i + 1) % ROWS][(j + COLS - 1) % COLS] +
                        old[(i + 1) % ROWS][j] +
                        old[(i + 1) % ROWS][(j + 1) % COLS];

                  switch(count)
                  {
                  case 0:
                  case 1:
                  case 4:
                  case 5:
                  case 6:
                  case 7:
                  case 8:
                        new[i][j] = 0;
                        break;

                  case 2:
                        new[i][j] = old[i][j];
                        break;

                  case 3:
                        new[i][j] = 1;
                        break;
                  }

                  GotoXY(j+1, i+1);
                  putch(new[i][j] ? '*' : ' ');
            } 
      }
}


void initialize(void)
{
      int i, j;

      randomize();
      ClrScrn(7);

      for (i = 0; i < ROWS; ++i)
      {
            for (j = 0; j < COLS; ++j)
            {
                  civ1[i][j] = random(2); 
                  GotoXY(j+1, i+1);
                  putch(civ1[i][j] ? '*' : ' ');
            } 
      }
}


int main(void)
{
      int i;

      initialize();
      for (i = 0; i < GENERATIONS; ++i)
      {
            update_generation(civ1, civ2);
            update_generation(civ2, civ1);
      }
      return EXIT_SUCCESS;
}
