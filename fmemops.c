/*
**  FMEMOPS.C - Emulate MSC's far memory functions in ZTC++ & early BC++
**
**  Original Copyright 1988-1992 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#if defined(__ZTC__) && !defined(__SC__)

#include <stdlib.h>
#include <dos.h>
#include "fmemops.h"

/*
**  Don't #include <string.h> to avoid incompatible prototypes
*/

#if __cplusplus
 extern "C" {
#endif

void CDECL movedata(unsigned,unsigned,unsigned,unsigned,size_t);

#if __cplusplus
 }
#endif

typedef unsigned char FAR *FarBytePtr;

void FAR * _fmemcpy(void FAR *dest, void FAR *src, size_t count)
{
      movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dest), FP_OFF(dest), count);
      return dest;
}

void FAR * _fmemmove(void FAR *dest, void FAR *src, size_t count)
{
      void FAR *target =  dest;
      FarBytePtr to = (FarBytePtr)dest, from = (FarBytePtr)src;

      if (src >= dest)
            _fmemcpy(dest, src, count);
      else  for (to += count, from += count; count; --count)
                  *--to = *--from;
      return target;
}

void FAR * _fmemset(void FAR *dest, int ch, size_t count)
{
      void FAR *target =  dest;
      FarBytePtr to = (FarBytePtr)dest;

      for ( ; count; --count)
            *to++ = (unsigned char) ch;
      return target;
}

#endif /* __ZTC__ */
