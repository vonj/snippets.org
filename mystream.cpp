// Mystream.cpp
// Implementation of ios interface classes for Myio
//
// Written by David L. Nugent
//

# include <iostream.h>
# include "Mystream.h"
# if defined(_MSC_VER)
#  include <memory.h>
# else
#  include <stdlib.h>
# endif

    // Mystreambuf constructor
    // This simply initialises the base class streambuf
    // (it is initially empty with no buffer allocated)
    // and register the Myio object
    // Note: we _could_ set the stream unbuffered here,
    // which is useful for stdio handles, so that the
    // streambuf functions overflow() and underflow()
    // get called on every character rather than when
    // the streambuf buffer is full

Mystreambuf::Mystreambuf (Myio * mPtr)
    : streambuf (), mptr(mPtr)
{
//  unbuffered(1);  // Uncomment to make unbuffered
}

    // Mystreambuf()
    // Called when streambuf owned buffer is full
    // or when stream is flushed
    // Our job here is to empty the streambuf
    // write buffer and reset the 'put' pointers.

int
Mystreambuf::overflow (int c)
{
    int written;

        // Handle unbuffered stream

    if (unbuffered())       // Handle the simple case first
    {
        if (c == EOF)   // Special case, this only flushes
            return 0;
        char ch = char(c);  // Write the byte directly
        written = mptr->write (&ch, 1);
        return (written) ? c : EOF;
    }

        // Handle buffered stream

    if (!base())        // Need to allocate a buffer
        allocate();

    if (base())         // Test for memory allocation error
    {
        char * ep = base() + (blen() / 2);
        if (!pbase())   // Set put pointers if not set up
            setp (base(), ep);
        int bytes = pptr() - pbase();   // Bytes to write
        if (bytes)
        {
            written = mptr->write (pbase(), bytes);
            if (!written)
                return EOF;
            bytes += written;
            if (bytes)  // Some is still waiting to be written
                memcpy (base(), base() + written, bytes);
        }
        setp (base() + bytes, ep);  // Reset 'put' pointers
        return (c == EOF) ? 0 : sputc (c);  // Put pending chr in buf
    }
    return EOF;
}

    //
    // underflow() indicates that the input queue
    // is empty and needs more data
    //

int
Mystreambuf::underflow (void)
{
    int bytes;

        // Handle an unbuffered stream

    if (unbuffered())
    {
        bytes = mptr->read (&_back[1], 1);
        if (!bytes)
        {
            setg (0, 0, 0);
            return EOF;
        }
        setg (_back, _back + 1, _back + 2);
        return (unsigned char)_back[1];
    }

        // Handle a buffered stream

    if (!base())        // Need to allocate a buffer
        allocate();

    if (base())
    {
        char * gp = base() + blen() / 2;
        if (gptr() >= egptr())
        {                   // Read into the buffer from stream
            overflow ();    // Flush output in case we need it
            bytes = mptr->read (gp + 1, blen() / 2 - 1);
            setg (gp, gp + 1, gp + bytes + 1);
        }
        if (gptr() < egptr())   // Get from buffer
            return (unsigned char) *gptr();
    }
    return EOF;
}

    //
    // sync() needs to empty both put and get
    // buffered. It will do this by calling
    // overflow and simply resetting the get
    // pointers to their default location.
    //

int
Mystreambuf::sync (void)
{
    if (!unbuffered())
    {
        overflow ();                // Force output
        char * gp = base();
        setp (gp, gp + blen() / 2);
        gp = base() + blen() / 2;
        setg (0, 0, 0);
    }
    return 0;
}
