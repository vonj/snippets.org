/*
**  UCLOCK.C
**
**  Contains routines to perform microsecond accuracy timing
**  operations.
**
**  Adapted from free sources originally by:
**  Kris Heidenstrom (kheidens@actrix.gen.nz)
**
**  For additional information, ftp:
**  oak.oakland.edu//SimTel/msdos/info/pctim*.zip.
**
**  Modified by Bob Stout (see UCLOCK_.ASM)
*/

#include <stddef.h>
#include <assert.h>
#include "uclock.h"
#include "cast.h"
#include "snpdosys.h"

/*
**  usec_clock()
**
**  An analog of the standard clock() function, usec_clock() returns a
**  uclock_t value (defined in UCLOCK.H) which represents the number
**  of clock ticks past midnight. Analogous to CLK_TCK is UCLK_TCK, the
**  number which a usec_clock() reading must be divided by to yield
**  a number of seconds.
**
**  Parameters: 1 - Pointer to a Uclock_T structure to be filled in. If
**                  NULL, an auto buffer is used.
**
**  Returns: If NULL is passed, returns uclock_t ticks since last midnight.
**           If a Uclock_T pointer was passed, returns -1.0.
**
**  Side effects: Reprograms timer channel 0 to mode 2. This is a benign
**                change with no known harmful effects on any normally
**                PC-compatible hardware. Note that although most early
**                PC's, XT's, and AT's typically run timer channel 0 in
**                mode 3, later model AT compatibles including many 386,
**                486, and Pentium systems already use mode 2.
**
**  Notes: Under Windows or OS/2 (HW_TIMER must be turned on), results are
**         generally unreliable due to the fact that there can be a
**         considerable and unpredictable lag between the time when the
**         virtual machine rolls over the virtual CTC registers the time
**         when the BIOS data area is updated. As a compromise, usec_clock()
**         detects when running in a non-DOS environment and retries until
**         it achieves monotonically increasing readings. This results in
**         an effective resolution much coarser than the .8381 uSec.
**         achieved under DOS. Although the result will still contain
**         20-bit data, the actual resolution will be lower and dependent
**         upon system speed, loading, and other overhead factors.
*/

uclock_t usec_clock(Uclock_T *now)
{
      static int init = 0;
      Uclock_T buf, *ptr;

      if (NULL == now)
            ptr = &buf;
      else  ptr = now;

      if (!init)                          /* Only reprogram CTC once!   */
      {
            SetMode2_();
            init = -1;
      }

      Uclock_(ptr);

      /*
      ** If Win or OS/2, ticks may lag CTC rollover so wait for
      ** monotonically increasing readings.
      */

      if (DOS != get_os())
      {
            static Uclock_T last;

            while (1)
            {
                  if ((last.ticks > ptr->ticks) ||
                        ((last.ticks == ptr->ticks) &&
                        (last.count > ptr->count)))
                  {
                        ++(ptr->ticks);
                        last.ticks = ptr->ticks;
                        last.count = ptr->count;
                        Uclock_(ptr);
                  }
                  else
                  {
                        last.ticks = ptr->ticks;
                        last.count = ptr->count;
                        break;
                  }
            }
      }

      return ((NULL == now) ? uclock_cnvrt(ptr) : -1.0);
}

/*
**  uclock_diff()
**
**  Subtracts one Uclock_T value from another.
**
**  Parameters: (All passed as pointers to Uclock_T structures)
**              1 - First time (time to subtract)
**              2 - Second time
**              3 - Difference or NULL
**
**  Returns : Difference converted to a uclock_t if parameter 3 is NULL, else
**            returns -1.0 and difference is returned in structure pointed to
**            by parameter 3.
**
**  Note: uclock_diff() properly handles elapsed times spanning midnight,
**        and is therefore preferable to simply subtracting uclock_t values.
**        If you must subtract uclock_t values, convert negative results to
**        positive by adding (UCLK_RLVR+1).
*/

uclock_t uclock_diff(Uclock_T *Start, Uclock_T *Stop, Uclock_T *Diff)
{
      Uclock_T buf, *ptr = ((NULL == Diff) ? &buf : Diff);

      /*
      ** Add rollover constant if the day has changed
      */

      if (Start->ticks <= Stop->ticks)
            ptr->ticks = Stop->ticks - Start->ticks;
      else  ptr->ticks = Stop->ticks + 0x001800B0L - Start->ticks;

      ptr->count = Stop->count - Start->count;
      if (Stop->count < Start->count)
            --(ptr->ticks);

      return ((NULL == Diff) ? uclock_cnvrt(ptr) : -1.0);
}

/*
**  uclock_cnvrt()
**
**  Converts from Uclock_T structure to a uclock_t value.
**
**  Parameters: 1 - Pointer to Uclock_T structure to convert.
**
**  Returns: uclock_t value.
*/

uclock_t uclock_cnvrt(Uclock_T *ticks)
{
      return (uclock_t)(ticks->count) + (uclock_t)(65536.0 * ticks->ticks);
}

/*
**  usec_delay()
**
**  Delays a specified number of microseconds.
**
**  Parameters: 1 - Delay time in microseconds.
**
**  Returns: Nothing
**
**  Note: Uses integer math in a tight loop to maximize resolution.
*/

void usec_delay(unsigned long usecs)
{
      Uclock_T tm0, tm1, diff;
      unsigned long et;

      assert(usecs < 40904449L);          /* Avoid 40.9 sec. overflow */

      Uclock_(&tm0);
      do
      {
            Uclock_(&tm1);
            uclock_diff(&tm0, &tm1, &diff);

            /*
            ** Convert ticks to uSecs. using integer math
            */

            et = CAST(unsigned long, diff);

            et *= 88L;
            et /= 105L;
      } while (usecs > et);
}

#ifdef TEST

/*
**  Test main() adapted from Sample program #15 by K. Heidenstrom
**  as cited above.
*/

#include <stdio.h>      /* Needed for printf()              */
#include <stdlib.h>     /* Needed for exit()                */
#include <conio.h>      /* Needed for kbhit() & getch()     */

main(void)
{
      Uclock_T ts, ts1, ts2;
      unsigned int sched = 0;
      unsigned int ch;

      puts("Press any key to get timestamp; "
            "press <Esc> for continuous test\n\n");

      do
      {
            ch = getch();                             /* Get a keypress */
            usec_clock(&ts);                          /* Get timestamp  */

            printf("Absolute timestamp: 0x%08lX%04X units of 0.8381 us\n",
                  ts.ticks, ts.count);
            printf("...representing %f seconds since last midnight\n\n",
                  uclock_cnvrt(&ts) / UCLK_TCK);
      } while ((ch & 0xff) != '\x1b');

      puts("\nProgram is now performing continuous timestamp test\n");
      puts("Press <Esc> to exit\n");

      ts1.ticks = ts.ticks;
      ts1.count = ts.count;

      while (1)
      {
            ts2.ticks = ts1.ticks;
            ts2.count = ts1.count;
            ts1.ticks = ts.ticks;
            ts1.count = ts.count;
            usec_clock(&ts);

            printf("0x%08lX%04X\r",
                  ts.ticks, ts.count);

            if ((ts.ticks < ts1.ticks) || ((ts.ticks == ts1.ticks) &&
                  (ts.count < ts1.count)))            /* Went backwards? */
            {
                  printf("Timestamp went backwards: 0x%08lX%04X, "
                        "0x%08lX%04X, then 0x%08lX%04X\n",
                        ts2.ticks, ts2.count,
                        ts1.ticks, ts1.count,
                        ts.ticks, ts.count);
            }
            ++sched;
            if (!(sched & 0xff))
            {
                  if (kbhit())
                  {
                        if ('\x1b' == (getch() & 0xff))
                              break;
                  }
            }
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
