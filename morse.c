/*
**   <<< Morse Code Functions >>>
** 
** Written by Michael M. Dodd, N4CF, and placed in the public domain.
** 
** The morse() function transmits a string in Morse code on the IBM PC's
** speaker.  The speed is set by a program constant (UNIT_TIME).
** 
** There are several other functions in this file, all used by morse(),
** and defined ahead of morse() for convenience.
** 
** The main() function at the end of the file is a test program to send
** the command-line argument string in morse code.  Enclose multiple
** words in quotes.  Example:  morse "hello, world"
** 
** These functions have been compiled and tested in the Small and Large
** memory models using Microsoft C 6.00a.
**
** Modified for ZTC++, TC++, & BC++ by Bob Stout
*/

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>

/*
** These functions turn on and off the CW tone on the PC's speaker.  The
** frequency is specified by the freq argument.
** IMPORTANT!  These functions are highly IBM PC-specific!
*/

#define CLK_FREQ (1193180L)
#define PIO (0x61)
#define CTC_CMD (0x43)
#define CTC_DATA (0x42)
#define SETUP (0xB6)
#define TONE_ON (0x03)
#define TONE_OFF (0xFC)

void note_on (int freq)        /* Turn on the tone.  */
{
      int divisor ;
      int pio_word ;

      divisor = (int)(CLK_FREQ / (long)(freq)) ;
      outp (CTC_CMD, SETUP) ;
      outp (CTC_DATA, divisor & 0xFF) ;
      outp (CTC_DATA, divisor >> 8) ;
      pio_word = inp (PIO) ;
      outp (PIO, pio_word | TONE_ON) ;
}

void note_off (void)           /* Turn off the tone.  */
{
      int pio_word ;

      pio_word = inp (PIO) ;
      outp (PIO, pio_word & TONE_OFF) ;
}

/*
** These functions implement a timing-loop delay.  Because the PC's
** internal clock is too coarse for accurate CW timing, the pause()
** function uses a simple software loop to produce a delay.
** 
** To minimize the effects of CPU clock speed, the calib() function
** returns a number which represents a rough index of the clock speed
** with relation to the standard IBM PC (this is very approximate).
** 
** Calibration is performed only once, when the static fudge_factor is
** zero.  Thereafter, the contents of fudge_factor are used to form a
** delay value.
** 
** IMPORTANT!  These functions are highly IBM PC-specific!
*/

unsigned int calib (void)
{
      unsigned int far *timerLow = (unsigned int far *)(0x046c) ;
      unsigned int lastTime ;
      unsigned int iter ;

      for (lastTime = *timerLow; lastTime == *timerLow;)
            ;

      for (iter = 0, lastTime = *timerLow; lastTime == *timerLow; iter++)
            ;
#if   defined(__ZTC__)
      return ((unsigned int)((125L * ((long)(iter)) + 50L) / 2300L)) ;
#elif defined(__TURBOC__)
      return ((unsigned int)((77L * ((long)(iter)) + 50L) / 2300L)) ;
#else /* assume MSC */
      return ((unsigned int)((100L * ((long)(iter)) + 50L) / 2300L)) ;
#endif
}

void pause (unsigned int amount)
{
      static unsigned int fudge_factor = 0 ;
      unsigned long ul ;

      if (fudge_factor == 0)       /* Calibrate the speed.  */
            fudge_factor = calib () ;

      ul = (unsigned long)(amount) * (long)(fudge_factor) ;
      while (ul--)                 /* Delay.  */
            ;
}

/*
** These functions transmit a dot, a dash, a letter space, and a
** word space.
** 
** Note that a single unit space is automatically transmitted after
** each dot or dash, so the ltr_space() function produces only a
** two-unit pause.
** 
** Also, the word_space() function produces only a four-unit pause
** because the three-unit letter space has already occurred following
** the previous letter.
*/

#define SPACE_MASK (1 << 15)
#define BIT_MASK (0xfe)
#define UNIT_TIME (18)
#define FREQUENCY (1500)

void send_dot (void)           /* Send a dot and a space.  */
{
      note_on (FREQUENCY) ;
      pause (UNIT_TIME) ;
      note_off () ;
      pause (UNIT_TIME) ;
}

void send_dash (void)          /* Send a dash and a space.  */
{
      note_on (FREQUENCY) ;
      pause (UNIT_TIME * 3) ;
      note_off () ;
      pause (UNIT_TIME) ;
}

void ltr_space (void)          /* Produce a letter space.  */
{
      pause (UNIT_TIME * 2) ;
}

void word_space (void)         /* Produce a word space.  */
{
      pause (UNIT_TIME * 4) ;
}


/*
** MORSE () - Transmit a string in Morse code
** 
** This function transmits the string pointed to by the cp argument in
** Morse code on the PC's speaker.  The speed is set by the UNIT_TIME
** constant.
** 
** A static table translates from ASCII to Morse code.  Each entry is
** an unsigned integer, where a zero represents a dot and a one
** represents a dash.  No more than 14 bits may be used.  Setting bit
** 15 produces a word space, regardless of any other pattern.
** 
** The Morse code pattern is taken from bit 0, and is shifted right
** each time an element is sent.  A special "marker bit" follows the
** complete Morse pattern.  This marker bit is tested before
** transmitting each bit; if there are no 1's in bits 1..15, the
** complete character has been sent.
** 
** For example, an "L" would be 0000000000010010, with bit zero
** containing the first dot, bit one the dash, etc.  The marker
** bit is in bit 4.
*/

void morse (char *cp)
{  /*--- MORSE CODE FUNCTION ---*/

      unsigned int c ;
      static unsigned int codes [64] = {
            SPACE_MASK,                      /* Entry 0 = space (0x20)  */
            0, 0, 0, 0, 0, 0, 0, 0,          /* ! " # $  % & " (  */
            0, 0, 0, 115, 49, 106, 41,       /* ) * + , - . /     */
            63, 62, 60, 56, 48, 32, 33, 35,  /* 0 1 2 3 4 5 6 7   */
            39, 47, 0, 0, 0, 0, 0, 76,       /* 8 9 : ; < = > ?   */
            0, 6, 17, 21, 9, 2, 20, 11,      /* @ A B C D E F G   */
            16, 4, 30, 13, 18, 7, 5, 15,     /* H I J K L M N O   */
            22, 27, 10, 8, 3, 12, 24, 14,    /* P Q R S T U V W   */
            25, 29, 19                       /* X Y Z  */
            } ;

      pause (0) ;                  /* Calibrate pause() function.  */

      while ((c = *cp++) != '\0')
      {  /*--- TRANSMIT COMPLETE STRING ---*/

            c = toupper (c) ;          /* No lower-case Morse characters.  */
            c -= ' ' ;                 /* Adjust for zero-based table.  */

            if (c > 58)                /* If out of range, ignore it.  */
                  continue ;

            c = codes[c] ;             /* Look up Morse pattern from table. */

            if (c & SPACE_MASK)        /* If the space bit is set..  */
            {                          /* ..send a word space and go on.  */
                  word_space () ;
                  continue ;
            }

            while (c & BIT_MASK)       /* Transmit one character.  */
            {  /*--- TRANSMIT EACH BIT ---*/
                  if (c & 1)
                        send_dash () ;
                  else  send_dot () ;

                  c >>= 1 ;
            }  /*--- TRANSMIT EACH BIT ---*/

            ltr_space () ;             /* Send a space following character. */

      }  /*--- TRANSMIT COMPLETE STRING ---*/

}  /*--- MORSE CODE FUNCTION ---*/


/*
** This is the test program, which transmits argv[1] in Morse code.
*/

main (int argc, char *argv[])
{
      while (--argc)
            morse (*++argv) ;
      return 0;
}
