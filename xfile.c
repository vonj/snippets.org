/*  xfile.c -- implementation for fast line buffered files
**
**  Currently (Sat  06-15-1991) XFILEs are for reading CR-LF terminated lines
**  from MS-DOS text files.  Period.  It's not that the method can't be used
**  as well for output buffering, or (in some form) for binary files, it's
**  that such are handled fast enough to suit me already, whereas text mode
**  input performance leaves me wishing for more speed.  This attempts to
**  solve that problem.
**
**  Sun  06-16-1991 -- CR-LF accepted, but so is bare LF now; the extracted
**  line does NOT have a NEWLINE at the end anymore (which will likely be
**  a mixed blessing...)
**
**  The code should be fairly portable: if/when I get around to polishing it
**  (and that won't be until I've used it some and am sure it's stable) I'll
**  be aiming for near-ANSI portability; for now I'm not pushing so very hard
**  for that.
**
**  The semantics are a bit odd: the lines are returned in a buffer that the
**  XFILE owns, and may be altered by a call to xgetline or xclose.  For
**  applications that resent this, XFILEs probably aren't a big win anyway,
**  but there might be some cases where using XFILE and copying (some) lines
**  is still a good idea.  The performance with long lines is good: it can
**  handle lines the size of the buffer, though it may truncate up to one
**  QUANTUM less one bytes "early": this depends on the location of the start
**  of the line in the buffer when we begin scanning.  In practice, XBUFSIZE
**  is probably larger than you'd set for a line buffer size anyway...
**
**  INTERNALS:
**
**  Reading the first buffer's worth at open time makes the EOF case easier to
**  detect.
**
**  TO DO:
**
**  clean up xgetline!
*/

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "xfile.h"

#if !defined(__ZTC__) && !defined(__TURBOC__)
 static int DOS_OPEN(const char *name, int mode, ...)
 {
       int hdl;

       if (0 == _dos_open(name, mode, &hdl))
             return hdl;
       else  return -1;
 }

 static int READ(int fd, void *buf, size_t len)
 {
       unsigned count;

       if (0 == _dos_read(fd, buf, len, &count))
             return count;
       else  return -1;
 }
#endif

#ifndef XBUFN               /* set default # of quanta in buffer, allow -D */
 #define XBUFN 8
#endif

#define QUANTUM 512
#define XBUFSIZE (XBUFN * QUANTUM)


/*  xopen -- allocate and open an XFILE
**
**  NB: currently I'm designing these for READ-ONLY TEXT FILES only: the xopen
**      interface may have to be changed...
**
**  returns pointer to XFILE of opened file or null pointer on error
**
**  ? should it leave a better error description somewhere ?
*/

XFILE *xopen(char const *name)
{
      XFILE *f = malloc(sizeof(XFILE) + XBUFSIZE + 1);
      int n;

      if (f == NULL)
            goto error0;
      f->buf = (char *)f + sizeof(XFILE);

      if ((f->fd = DOS_OPEN(name, O_RDONLY)) < 0)
            goto error1;

      if ((n = READ(f->fd, f->buf, XBUFSIZE)) < 0)
            goto error2;

      f->buf[n] = 0;
      f->nextChar = f->buf;
      return f;

error2:
      CLOSE(f->fd);
error1:
      free(f);
error0:
      return NULL;
}


/*
**  xclose -- close and deallocate an XFILE
*/

void xclose(XFILE *f)
{
      CLOSE(f->fd);
      free(f);
}


/*
**  xgetline -- get the next text line into memory
**
**  returns a pointer to the line (a NUL-terminated string) or a null pointer
*/

char *xgetline(XFILE *f)
{
      char *s = f->nextChar, *p;
      int n;

      for (p = s; *p != 0; ++p)
      {
            if (*p == '\n')
            {
                  if (s < p && p[-1] == '\r')
                        p[-1] = 0;
                  else  *p = 0;
                  f->nextChar = p + 1;
                  return s;
            }
      }

      /*
      **  end of line not found in buffer -- p points to the sentinel NUL
      */

      if (p == f->buf)                    /* iff empty, EOF */
            return NULL;

      /*
      **  move prefix of line to bottom of buffer
      */

      if (s != f->buf)
      {
            for (p = f->buf; (*p = *s) != 0; ++p, ++s)
                  ;
            s = f->buf;
      }

      n = XBUFSIZE - (p - f->buf);

      if (n < QUANTUM)                    /* insufficient room, break line */
      {
            f->nextChar = p;
            return s;
      }

      n = (n / QUANTUM) * QUANTUM;        /* quantize: count to read */
      n = READ(f->fd, p, n);

      /*
      **  read error is sort of ignored here... same return as EOF.
      **  we'll see if this proves to be sufficient...
      */

      if (n < 0)
      {
            f->nextChar = f->buf;
            f->buf[0] = 0;
            return NULL;
      }

      p[n] = 0;

      for ( ; *p != 0; ++p)
      {
            if (*p == '\n')
            {
                  if (s < p && p[-1] == '\r')
                        p[-1] = 0;
                  else  *p = 0;
                  ++p;
                  break;
            }
      }

      f->nextChar = p;
      return p == s ? NULL : s;
}
