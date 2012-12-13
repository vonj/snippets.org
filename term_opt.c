/*
**
**  This was written under AIX 3.2.5.1
**  (Cripes just a few more numbers please.)
**
**  Terminal functions
**
**  Option 0 - Turns the ECHO on.
**         1 - Turns the ECHO off.
**         2 - Waits forever for keyboard activity.
**
**  Public Domain *IX terminal functions.
**  Slung together by Steve Poole for RBS's SNIPPETS.
**
*/

#define _POSIX_SOURCE 1

#include <termios.h>
#include <sys/types.h>
#include "unxconio.h"

int term_option(option)
int  option;
{
  struct termios attributes;

    switch(option)
    {
/*
**  Turn echo on
*/
      case 0:
             if(tcgetattr(STDIN_FILENO,&attributes) != 0)
               return (-1);
             attributes.c_lflag |= ECHO;
             if(tcsetattr(STDIN_FILENO,TCSANOW,&attributes) != 0)
               return (-1);
             break;
/*
**  Turn echo off
*/
      case 1:
             if(tcgetattr(STDIN_FILENO,&attributes) != 0)
               return (-1);
             attributes.c_lflag &= ~(ECHO);
             if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&attributes) != 0)
               return (-1);
             break;
/*
**  Wait forever for the keyboard to be touched. (AHHHHHH!!!!!!!)
*/
      case 2:
             if(tcgetattr(STDIN_FILENO,&attributes) != 0)
               return (-1);
             attributes.c_lflag    &= ~(ICANON);
             attributes.c_cc[VMIN]  = 1;
             attributes.c_cc[VTIME] = 1;
             if(tcsetattr(STDIN_FILENO,TCSANOW,&attributes) != 0)
               return (-1);
             break;
/*
**  Don't be a bozo, call it with something
*/
      default:
              printf("Error in terminal options routine, "
                    "BAD OPTION %d\n",option);
              return(-1);
              break;
    }
}

#ifdef TEST

main(argc,argv)
int  argc;
char *argv[];
{
  int  option,istat;
  char buffer[2];

  option = atoi(argv[1]);
  istat = term_option(option);
  istat = read(STDIN_FILENO,&buffer,1);
  if(istat < 0)
    printf("Error on READ\n");
  return 0;
}

#endif /* TEST */
