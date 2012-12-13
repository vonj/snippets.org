// Myio.cpp
// Simple I/O class to demonstrate use of C++ iostream
// facilities in a customised environment
// Written by David L Nugent, June 1993

# include <iostream.h>
# include <string.h>
# include "Myio.h"
# include "Mystream.h"

Myio::Myio (int sz)
    : bufsize(sz), bufchars(0), bufidx(0),
      bufaddr(new char[bufsize]), mystream(0)
{}

Myio::~Myio (void)
{
    delete bufaddr;
    delete mystream;
}

iostream &
Myio::stream (void)
{
    if (!mystream)      // Create a stream if required
        mystream = new Mystream(this);
    return *mystream;
}

int         // Simple write function into a circular buffer
Myio::write (char const * buf, int len)
{
    int avail = (bufsize - bufchars);   // See how many fit
    if (len > avail)
    {
        len = avail;
        stat |= Myio::overflow;         // Only partial write
    }
    else
        stat &= ~Myio::overflow;
    avail = bufsize - bufidx;           // Calculate room at end
    if (avail > len)
        avail = len;
    if (avail)
    {
        memcpy (bufaddr + bufidx, buf, avail);
        bufidx += avail;                // Update the put index
        buf += avail;                   // And the input pointer
    }
    if (bufidx >= bufsize)              // Wrap buffer to start
        bufidx = 0;
    avail = len - avail;                // See if there is any more to go
    if (avail)
    {
        memcpy (bufaddr + bufidx, buf, avail);
        bufidx += avail;                // Update the put index
    }
    bufchars += len;
    return (_pcount = len);
}

int         // Simple read function from a circular buffer
Myio::read (char * buf, int len)
{
    if (len > bufchars)                 // Adjust for available bytes
    {
        len = bufchars;
        stat |= Myio::underflow;        // Got an underflow (partial read)
    }
    else
        stat &= ~Myio::underflow;       // Clear underflow flag
    int startidx = bufidx - bufchars;   // Determine start get position
    if (startidx < 0)
        startidx += bufsize;            // Adjust for wrap
    int avail = bufsize - startidx;     // Check room at end of buffer
    if (avail > len)                    // Adjust down if necessary
        avail = len;
    if (avail)                          // Copy first section
    {
        memcpy (buf, bufaddr + startidx, avail);
        startidx += avail;              // Adjust start index
        buf += avail;                   // Adjust output pointer
    }
    if (startidx >= bufsize)            // Wrap buffer to start
        startidx = 0;
    avail = len - avail;                // See if there is any more to go
    if (avail)                          // If so, copy the rest
        memcpy (buf, bufaddr + startidx, avail);
    bufchars -= len;                    // Adjust character count
    return (_gcount = len);
}

Myio &
operator<< (Myio & m, char const * ptr)
{
    m.write (ptr, strlen (ptr));
    return m;
}

int
Myio::dump (void) const
{
    if (bufchars)
    {
        char * tmp = new char[bufchars + 2];
        int idx = bufidx - bufchars;
	  int i;

	  if (idx < 0)
            idx += bufsize;
        for (i = 0; i < bufchars; )
        {
            if (idx >= bufsize)
                idx = 0;
            tmp[i++] = bufaddr[idx++];
        }
        if (i)
        {
            if (tmp[i-1] != '\n')   // Terminate with NL
                tmp[i++] = '\n';
            tmp[i] = 0;
            cout << "---\n"
                 << tmp
                 << "---\n";
        }
        delete tmp;
    }
    return bufchars;
}
