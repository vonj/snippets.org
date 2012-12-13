/*
**  Public domain demo by Ray Gardner, 7 dec 88
**
**  Here's an old programming trick that (I bet) will be new to at least a
**  few of you out there, even some "old hands".  I don't remember where I
**  saw this; it might have been Jon Bentley's "Programming Pearls" column in
**  Communications of the ACM.
**
**  Have you ever wanted to exchange two adjacent areas of storage which
**  might be of two different lengths?  There are some tricky and complicated
**  "efficient" methods to do this without using a lot of extra temporary
**  storage.  But there is also an old and simple way: Assume that the buffer
**  looks like this:
**
**     |...... head .......|.................. tail .................|
**
**  You reverse the head, reverse the tail, then reverse the entire buffer.
**  That's all there is to it.  It will leave you with:
**
**     |.................. tail .................|...... head .......|
**
**  Here's code:
*/

#include <stdlib.h>
#include "memrev.h"

/*
**  reverse "count" bytes starting at "buf"
*/

void memrev(char  *buf, size_t count)
{
      char *r;

      for (r = buf + count - 1; buf < r; buf++, r--)
      {
            *buf ^= *r;
            *r   ^= *buf;
            *buf ^= *r;
      }
}

/*
**  swap "head" bytes with "tail" bytes at "buf"
*/

void aswap(char  *buf, size_t head, size_t tail)
{
      memrev(buf, head);
      memrev(buf + head, tail);
      memrev(buf, head + tail);
}
