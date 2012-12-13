/*
**  SOUND.H
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#ifndef SOUND__H
#define SOUND__H

#if defined(__ZTC__)
 #include <int.h>
 #undef int_on
 #undef int_off
#elif defined(__TURBOC__)
 #define int_on         enable
 #define int_off        disable
 #ifndef inp
  #define inp           inportb
 #endif
 #ifndef outp
  #define outp          outportb
 #endif
#else /* assume MSC/QC */
 #include <conio.h>
 #define int_on         _enable
 #define int_off        _disable
 #define getvect        _dos_getvect
 #define setvect        _dos_setvect
#endif

/* defines for mktone() update parameter: */

#define ON 0          /* turn the speaker on */
#define UPDATE 1      /* sound is on, just change freq */
#define TOGGLE 2      /* for delay use, turn on, then off */

/* port equates */

#define SCNTRL 0x61   /* sound control port */
#define SOUNDON 0x03  /* bit mask to enable speaker */
#define SOUNDOFF 0xfc /* bit mask to disable speaker */
#define C8253 0x43    /* port address to control 8253 */
#define SETIMER 0xb6  /* tell 8253 to expect freq data next */
#define F8253 0x42    /* frequency address on 8253 */

/* frequency equates (musical scale) */
/* digit in label is octave number, S indicates Sharp (#) */

#define C0      36489
#define CS0     34445
#define D0      32512
#define DS0     30673
#define E0      28961
#define F0      27329
#define FS0     25804
#define G0      24351
#define GS0     22981
#define A0      21694
#define AS0     20473
#define B0      19326

#define C1      18244
#define CS1     17218
#define D1      16251
#define DS1     15340
#define E1      14480
#define F1      13668
#define FS1     12899
#define G1      12175
#define GS1     11493
#define A1      10847
#define AS1     10238
#define B1      9663

#define C2      9121
#define CS2     8609
#define D2      8126
#define DS2     7670
#define E2      7239
#define F2      6833
#define FS2     6450
#define G2      6088
#define GS2     5746
#define A2      5424
#define AS2     5119
#define B2      4832

#define C3      4561
#define CS3     4305
#define D3      4063
#define DS3     3835
#define E3      3620
#define F3      3417
#define FS3     3225
#define G3      3044
#define GS3     2873
#define A3      2712
#define AS3     2560
#define B3      2416

#define C4      2280
#define CS4     2152
#define D4      2032
#define DS4     1917
#define E4      1810
#define F4      1708
#define FS4     1612
#define G4      1522
#define GS4     1437
#define A4      1356
#define AS4     1280
#define B4      1210

#define C5      1140
#define CS5     1076
#define D5      1016
#define DS5     959
#define E5      905
#define F5      854
#define FS5     806
#define G5      761
#define GS5     718
#define A5      678
#define AS5     640
#define B5      604

#define C6      570
#define CS6     538
#define D6      508
#define DS6     479
#define E6      449
#define F6      427
#define FS6     403
#define G6      380
#define GS6     359
#define A6      339
#define AS6     320
#define B6      302

#define C7      285
#define CS7     269
#define D7      254
#define DS7     240
#define E7      226
#define F7      214
#define FS7     202
#define G7      190
#define GS7     180
#define A7      169
#define AS7     160
#define B7      151

#define C8      143

#define REST    0

typedef struct
{
        unsigned int      freq;
        unsigned int      duration;
} NOTE;

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

void  mktone(int, int, unsigned),
      dosound(int),
      soundon(void),
      soundoff(void),
      playb_close(void);

int   playb_note(unsigned, unsigned);

NOTE *playb_open(unsigned);

#if defined(__cplusplus) && __cplusplus
 }
#endif

#endif /* SOUND__H */
