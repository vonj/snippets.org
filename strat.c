/*
**  strat.c  10-5-91  Robert Mashlan, public domain
**
**   Interface functions to DOS 3.0+ set allocation strategy
**   and get allocation strategy functions via int 21h,
**   function 58h.
**
**   By setting the dos allocation strategy to LAST_FIT_LOW
**   before using DOS the set handle count function int 21h,
**   function 67h, DOS will allocate memory for the extended
**   file handle table at the end of free memory instead of
**   after the last heap allocation, with the benefit of
**   allowing the heap manager make further contiguous
**   allocations from the operating system.
**
*/

#include <stdlib.h>
#include <dos.h>
#include "strat.h"

/*
**   Gets dos memory allocation strategy via function 0x58.
**   Returns  allocation strategy, else returns -1 and sets
**   _doserrno on error.
*/

int get_alloc_strat(void)
{
      union REGS r;

      r.x.ax = 0x5800;              /* DOS "get allocation strategy" */
      int86(0x21,&r,&r);
      if (r.x.cflag)                /* error? */
      {
            _doserrno = r.x.ax;     /* save error code */
            return -1;
      }
      else  return r.x.ax;
}

/*
**   Sets DOS memory allocation strategy
**   returns allocation strategy on success,
**   else returns -1 and sets _doserrno on error
*/

int set_alloc_strat( int strat )
{
      union REGS r;

      r.x.ax = 0x5801;              /* DOS "set allocation strategy" */
      r.x.bx = strat;
      int86(0x21,&r,&r);
      if (r.x.cflag)                /* error? */
      {
            _doserrno = r.x.ax;     /* save error code */
            return -1;
      }
      _doserrno = 0;
      return strat;
}

/*
**   Uses dos function 67h to increase open file handle count.
**   Returns -1 and sets _doserrno on error, 0 otherwise.
*/

int set_handle_count( unsigned nhandles )
{
      union REGS r;

      r.x.ax = 0x6700;
      r.x.bx = nhandles;
      int86(0x21,&r,&r);
      if(r.x.cflag)
      {
            _doserrno = r.x.ax;
            return -1;
      }
      _doserrno = 0;
      return 0;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

/*
**  returns maximum number of files that can be open
*/

int handle_count(void)
{
      int handles[500];
      int i, result;

      /* try allocating as many file handles as possible */

      for (i = 0; i < 500; i++)
      {
            if( (handles[i]=open("NUL",O_WRONLY)) == -1 )
                  break;
      }
      result = i;

      /* close all files opened */

      for (i--; i >= 0; i--)
            close(handles[i]);
      return result;
}


/*
**   Memory test, returns number of kilobytes that
**   can be allocated before failure.
*/

int memtest(void)
{
      static void *mem[1024];
      int i,result;

      /* try allocating as many 1K blocks as possible */

      for(i=0;i<1024;i++)
      {
            if( (mem[i]=malloc(1024)) == NULL )
                  break;
      }
      result = i;                               /* save result */

      /* free memory allocated */

      for(i--; i >= 0; i--)
            free(mem[i]);
      return result;
}

#define checkdoserror(f) \
   ((f==-1)?printf("%s failed, doserror = %#02x\n",#f,_doserrno):0)

int main(void)
{
      int strat;

      /* do pre-test diagnostics */

      printf("allocated %d Kbytes before failure\n",memtest());
      printf("opened %d files\n",handle_count());

      strat = get_alloc_strat();  /* save current allocation strategy */
      checkdoserror(set_alloc_strat(LAST_FIT_LOW));
      puts("setting handle count to 50, with changed allocation strategy");
      checkdoserror(set_handle_count(50));
      checkdoserror(set_alloc_strat(strat)); /* restore strategy */

      /* do post-test diagnostics */

      printf("allocated %d Kbytes before failure\n",memtest());
      printf("opened %d files\n",handle_count());

      return 0;
}

#endif /* TEST */
