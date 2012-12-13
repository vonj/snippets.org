/*
**  Originally published as part of the MicroFirm Function Library
**
**  Copyright 1990, Robert B.Stout
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
**
**  Function to locate an unused user interrupt vector.
*/

#include "extkword.h"
#include "snpdosys.h"

#ifdef __ZTC__
 #include <int.h>
#else
 #include <dos.h>
 #ifdef __TURBOC__
  #define GETVECT getvect
 #else /* assume MSC */
  #define GETVECT _dos_getvect
 #endif
 #define FNULL (void (FAR *)())(0L)
#endif

unsigned findIslot(void)
{
#ifdef __ZTC__
      unsigned int_no, seg, ofs;

      for (int_no = 0x60; int_no < 0x6f; ++int_no)
      {
            int_getvector(int_no, &seg, &ofs);
            if (0U == (seg | ofs))
                  return int_no;
      }
#else /* MSC/BC/TC */
      unsigned int_no;

      for (int_no = 0x60; int_no < 0x6f; ++int_no)
      {
            if (FNULL != (void (FAR *)())GETVECT(int_no))
                  return int_no;
      }
#endif
      return 0;
}
