// myLine.cpp
//
// 13 Jun 93   Init array[0] = NUL in case it is reference before use
//             memcpy() adjusted to also copy terminating NUL from is.get()
//             when extending buffer
//

# include <iostream.h>
# include "myLine.h"
# if defined(_MSC_VER)
#  include <memory.h>
# else
#  include <stdlib.h>
# endif

    // Class implementation

myLine::myLine (short buflen)
    : len(buflen), mybuf(new char[len]), xalloc(1)
{
    mybuf[0] = 0;
}

myLine::myLine (char * usebuf, short buflen)
    : len(buflen), mybuf(usebuf), xalloc(0)
{
    mybuf[0] = 0;
}

myLine::~myLine (void)
{
    if (xalloc)
        delete [] mybuf;
}


istream &
operator>> (istream & is, myLine & l)
{
# if AUTO_GROW
    if (!l.xalloc)              // It's not my buf, so it can't be grown
    {
# endif
        is.get (l.mybuf, l.len);
        if (is.peek() == '\n')
            is.get();           // Remove newline from stream
# if AUTO_GROW
    }
    else
    {
        int idx = 0;
        l.mybuf[0] = 0;     // Terminate in case is.good() isn't
        for (int eol = 0; !eol && is.good(); )
        {
            int toget = l.len - idx;
            is.get (l.mybuf + idx, toget);
            int chrs = is.gcount();
            if (is.peek() == '\n')
            {
                ++eol;       // Must be eol or eof
                is.get();    // Clear newline
            }
            else if (chrs)
            {                // Extend buffer
                idx += chrs; // Add to index
                l.len = short(l.len + ALLOC_LEN);
                char * tmp = new char[l.len];
                memcpy (tmp, l.mybuf, idx + 1);
                delete [] l.mybuf;
                l.mybuf = tmp;
            }
        }
    }
# endif
    return is;
}
