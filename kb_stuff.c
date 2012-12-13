/*
**  KB_STUFF.C - Functions to stuff characters and/or strings into a PC's
**               (BIOS) keyboard buffer.
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <dos.h>
#include "sniptype.h"
#include "pchwio.h"
#include "snipkbio.h"

static unsigned head, tail, start, end;
static int idx = 0;
static unsigned short keystack[16][2];

/*
**  ungetkey()
**
**  Stuffs characters into the keyboard buffer.
**
**  Parameters: 1 - Extended character to stuff
**
**  Returns: Success_ or EOF
**
**  Note: This function assumes that the keyboard buffer is in
**        the normal (for IBM) location of 40:1E.
**
*/

int ungetkey(unsigned short key)
{
      int count;

      head  = Peekw(0x40, 0x1a);
      tail  = Peekw(0x40, 0x1c);
      start = Peekw(0x40, 0x80);
      end   = Peekw(0x40, 0x82);

      count = tail - head;
      if (0 > count)
            count += (16 * sizeof(unsigned));
      count >>= 1;

      if (15 > count)
      {
            disable();
            keystack[idx][0] = Peekw(0x40, tail);
            keystack[idx][1] = tail;
            Pokew(0x40, tail, key);
            tail += sizeof(unsigned);
            if (end <= tail)
                  tail = start;
            Pokew(0x40, 0x1c, tail);
            enable();
            return key;
      }
      return EOF;
}

/*
**  KB_stuff()
**
**  Stuffs strings into the keyboard buffer.
**
**  Parameters: 1 - String to stuff
**
**  Returns: Success_ if successful
**           Error_   in case of error, plus keyboard buffer is
**                    restored
**
**  Note: This function assumes that the keyboard buffer is in
**        the normal (for IBM) location of 40:1E.
*/

int KB_stuff(char *str)
{
      int ercode = Success_;

      idx = 0;
      while (*str)
      {
            if (EOF == ungetkey((unsigned)(*str++)))
            {
                  disable();
                  while (0 <= --idx)
                  {
                        tail = keystack[idx][1];
                        Pokew(0x40, tail, keystack[idx][0]);
                  }
                  Pokew(0x40, 0x1c, tail);
                  enable();
                  ercode = Error_;
                  break;
            }
            else  ++idx;
      }
      idx = 0;
      return ercode;
}
