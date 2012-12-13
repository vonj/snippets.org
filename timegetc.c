/*
**  TIMEGETC.C - waits for a given number of seconds for the user to press
**               a key.  Returns the key pressed, or EOF if time expires
**
**  by Bob Jarvis
*/

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include "snipkbio.h"


int timed_getch(int n_seconds)
{
      time_t start, now;

      start = time(NULL);
      now = start;

      while(difftime(now, start) < (double)n_seconds && !kbhit())
      {
            now = time(NULL);
      }

      if(kbhit())
            return getch();
      else  return EOF;
}

#ifdef TEST

main()
{
      int c;

      printf("Starting a 5 second delay...\n");

      c = timed_getch(5);

      if(c == EOF)
            printf("Timer expired\n");
      else  printf("Key was pressed, c = '%c'\n", c);
      return 0;
}

#endif /* TEST */
