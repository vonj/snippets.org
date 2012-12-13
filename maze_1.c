/*
   This program makes 10x10 mazes and prints them on the screen.  No
   promise of portability is made, but it does seem to work on NS GNX
   C.

   Public Domain by Jonathan Guthrie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UP  1
#define DN  2
#define LT  4
#define RT  8

int  addelem(int, int [12][12], int *, int, int);
void openwall(int [12][12], int, int);
void writemaze(int [12][12]);

main()
{
      int i, j, base;
      int search[150], array[12][12];

      for(i=1 ; i<11 ; ++i)
      {
            array[i][0] = -1;
            array[i][11] = -1;
            array[0][i] = -1;
            array[11][i] = -1;
            for(j=1 ; j<11 ; ++j)
                  array[i][j] = 0;
      }

      srand((int)time(NULL));
      i = rand() % 10 + 1;
      j = rand() % 10 + 1;
      base = addelem(0, array, search, i, j);
      array[i][j] = RT + RT;      /* Not a valid value */
      while(0 < base)
      {
            i = rand() % base;
            j = search[i];
            search[i] = search[--base];
            i = j % 100;
            j /= 100;
            openwall(array, i, j);
            base = addelem(base, array, search, i, j);
      }

      writemaze(array);
      return 0;
}


int addelem(int base, int maze[12][12], int *search, int row, int col)
{
      if(0 == maze[row-1][col])
      {
            search[base++] = row + col * 100 - 1;
            maze[row-1][col] = -DN;
      }
      else if(0 > maze[row-1][col])
            maze[row-1][col] -= DN;

      if(0 == maze[row+1][col])
      {
            search[base++] = row + col * 100 + 1;
            maze[row+1][col] = -UP;
      }
      else if(0 > maze[row+1][col])
            maze[row+1][col] -= UP;

      if(0 == maze[row][col-1])
      {
            search[base++] = row + col * 100 - 100;
            maze[row][col-1] = -RT;
      }
      else if(0 > maze[row][col-1])
            maze[row][col-1] -= RT;

      if(0 == maze[row][col+1])
      {
            search[base++] = row + col * 100 + 100;
            maze[row][col+1] = -LT;
      }
      else if(0 > maze[row][col+1])
            maze[row][col+1] -= LT;

      return base;
}


void openwall(int maze[12][12], int row, int col)
{
      int directions, max, direction, temprow, tempcol, temp, back;

      directions = -maze[row][col];

      max = 0;
      if(directions & UP)
      {
            temp = rand();
            if(temp > max)
            {
                  max = temp;
                  direction = UP;
                  back = DN;
                  temprow = row - 1;
                  tempcol = col;
            }           
      }

      if(directions & DN)
      {
            temp = rand();
            if(temp > max)
            {
                  max = temp;
                  direction = DN;
                  back = UP;
                  temprow = row + 1;
                  tempcol = col;
            }
      }

      if(directions & LT)
      {
            temp = rand();
            if(temp > max)
            {
                  max = temp;
                  direction = LT;
                  back = RT;
                  temprow = row;
                  tempcol = col - 1;
            }
      }

      if(directions & RT)
      {
            temp = rand();
            if(temp > max)
            {
                  max = temp;
                  direction = RT;
                  back = LT;
                  temprow = row;
                  tempcol = col + 1;
            }
      }
    
      maze[row][col] = direction;
      maze[temprow][tempcol] += back;
}

void writemaze(int maze[12][12])
{
      int i, j;

      puts("*********************");
      for(i=1 ; i<11 ; ++i)
      {
            putchar('*');
            for(j=1 ; j<11 ; ++j)
            {
                  putchar(' ');
                  if(maze[i][j] & RT)
                        putchar(' ');
                  else  putchar('*');
            }
            putchar('\n');
            for(j=1 ; j<11 ; ++j)
            {
                  putchar('*');
                  if(maze[i][j] & DN)
                        putchar(' ');
                  else  putchar('*');
            }
            puts("*");
      }
}
