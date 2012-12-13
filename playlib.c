/*
**  PLAYLIB.C
**
**  Public domain for TC only by Lynn R. Lively
**  Modified by Bob Stout
*/

#include <dos.h>
#include <stdlib.h>
#include "pchwio.h"
#include "sound.h"

#ifndef __ZTC__
 static void (INTERRUPT FAR *n_oldtmint) (void);
#endif

#define TIMER_TICK_INTR 0x1c

static NOTE *     n_buff;
static unsigned   n_buff_sz;
static NOTE *     n_head;
static NOTE *     n_tail;
static unsigned   play_duration;
static unsigned   play_freq;

/*
** Add note to note buff. Return = 1 (note added), 0 (Out of note buff)
*/

int playb_note (unsigned freq, unsigned duration)
{
      if (++n_tail == (n_buff + n_buff_sz))
            n_tail = n_buff;

      if (n_tail == n_head)
      {
            --n_tail;
            return (0);
      }

      n_tail->freq     = freq;
      n_tail->duration = duration;

      return (1);
}

/*
** ISR for background music.
*/

#ifndef __ZTC__
 static void INTERRUPT FAR play_intr (void)
#else
 static int play_intr (struct INT_DATA *idp)
#endif
{
      int_off ();

#ifndef __ZTC__
      (*n_oldtmint) ();                   /* Call Old timer interrupt.  */
#else
      int_prev(idp);
#endif

      if (play_duration == 0)
      {
            soundoff ();

            if (++n_head == (n_buff + n_buff_sz))
                  n_head = n_buff;

            if (n_head == n_tail)
            {
                  --n_head;
                  int_on ();
                  return;
            }

            play_duration = n_head->duration;
            if (0 != (play_freq = n_head->freq))
                  soundon();
            dosound (play_freq);
      }
      else  --play_duration;

      int_on ();

#ifdef __ZTC__
      return 1;                           /* Don't chain                */
#endif
}

/*
** Call this function to init background music. buff_sz is number of
** notes in the note buffer. Returns pointer to buff or NULL if
** out of heap space.
*/

NOTE * playb_open (unsigned buff_sz)
{
      n_buff =
      n_head =
      n_tail =  (NOTE *) calloc (buff_sz, sizeof (NOTE));

      if (n_buff != (NOTE *) NULL)
      {
            n_buff_sz     = buff_sz;

            play_duration =
            play_freq     = 0;
            
#ifdef __ZTC__
            int_intercept(TIMER_TICK_INTR, play_intr, 256);
#else
            n_oldtmint    = getvect (TIMER_TICK_INTR);
            setvect (TIMER_TICK_INTR, play_intr);
#endif
      }
      return (n_buff);
}

/*
** Return things to normal and free allocated space.
*/

void playb_close (void)
{
      soundoff ();
#ifndef __ZTC__
      setvect (TIMER_TICK_INTR, n_oldtmint);
#else
      int_restore(TIMER_TICK_INTR);
#endif
      free (n_buff);
}
