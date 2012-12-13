// Base class for screen interpreter classes
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#if !defined(_scrinterp_h)
#define _scrinterp_h
#include "video.hpp"

class istream;

class scrinterp
{
  public:
    scrinterp(video & v);
    virtual ~scrinterp();

    void display(istream & is);
    virtual void putch(int ch);
    virtual void reset();
    virtual void flushbuf();// Output accumulated buffer

    void flusheverychar(char state =1)    { flushevery = state; }

  protected:
    static const int CBUFSZ;
    video & vs;             // Video i/o device
    char * chrbuf;          // Output buffer
    char chridx;            // Length of data in output buffer
    char flushevery;        // Flush after every chr
    char insertmode;        // Insert mode

    void putbuf(int ch);    // Put chr into buffer
};

#endif

