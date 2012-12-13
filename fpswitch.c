/*
**  FPSWITCH.C - Demonstrates using function pointers in lieu of switches
*/

#include <stdlib.h>     /* for NULL */

/*  Declare your functions here     */

char *cpfunc1(int);
char *cpfunc2(int);
char *cpfunc3(int);

void vfunc1(void);
void vfunc2(void);
void vfunc3(void);
void vfunc4(void);

/*
**  Old ways using switch statements
*/

char *oldcpswitch(int select, int arg)
{
      switch (select)
      {
      case 1:
            return cpfunc1(arg);
      
      case 2:
            return cpfunc2(arg);
      
      case 3:
            return cpfunc3(arg);
            
      default:
            return NULL;
      }
}

void oldvswitch(int select)
{
      switch (select)
      {
      case 1:
            vfunc1();
            break;
      
      case 2:
            vfunc2();
            break;
      
      case 3:
            vfunc3();
            break;
      
      case 4:
            vfunc4();
            break;
      }
}

/*
**  Using function pointers
*/

char *newcpswitch(int select, int arg)
{
      char *(*cpfunc[3])(int) = { cpfunc1, cpfunc2, cpfunc3 };

      if (select < 1 || select > 3)
            return NULL;
      return (*cpfunc[select-1])(arg);
}

void newvswitch(int select)
{
      void (*vfunc[4])(void) = { vfunc1, vfunc2, vfunc3, vfunc4 };

      if (select > 0 && select < 5)
            (*vfunc[select-1])();
}
