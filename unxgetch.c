/*
**  UNXGETCH.C - Non-blocking console input function for Unix/Posix.
**
**  public domain by Bob Stout using Steve Poole's term_option().
*/

#include "unxconio.h"

int getch()
{
      int istat, key;
      char buf[2];

      if (0 > term_option(1))
            return EOF;
      if (0 > term_option(2))
            return EOF;
      istat = read(STDIN_FILENO,&buf,1);
      if (istat < 0)
            key = EOF;
      else  key = (int)buf[0];
      term_option(0);
      return key;
}

#ifdef TEST

#include <ctype.h>

main()
{
      int key;

      printf("Press any key to continue...");
      fflush(stdout);
      key = getch();
      printf("\n\nYou pressed \"%c\"\n", toupper(key));
      return 0;
}

#endif /* TEST */
