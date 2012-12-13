// Mystream.h
// iostream interface for class Myio
// Defines the following classes:
//  Mystreambuf     derived from streambuf - buffer management & I/O interface
//  Mystreambase    base class used for initialisation & object reference
//  Myiostream      customised iostream, derived from iostream/Mystreambase
//
// Written by David L Nugent, June 1993
//

# if !defined(_Mystream_h)
# define _Mystream_h 1
# include <iostream.h>
# include "Myio.h"

    //
    // Mystreambuf
    // This is the class which does all the actual I/O
    // handling and (optional) buffer management
    //

class Mystreambuf : public streambuf
{

  public:

    Mystreambuf (Myio * mPtr);

  protected:

    virtual int overflow (int = EOF);
    virtual int underflow ();
    virtual int sync ();

  private:

    Myio * mptr;    // Points to the Myio instance to
                    // which this stream is attached
    char _back[2];  // Holder for putback

};


class Mystreambase : public virtual ios
{

  public:

    Mystreambase (Myio * mPtr);
    Mystreambuf * rdbuf (void);

  protected:

    Mystreambuf mystreambuf;

};

inline
Mystreambase::Mystreambase (Myio * mPtr)
    : mystreambuf (mPtr)
{}

inline Mystreambuf *
Mystreambase::rdbuf (void)
    {   return &mystreambuf;    }


class Mystream : public Mystreambase, public iostream
{

  public:

    Mystream (Myio * mPtr);
    ~Mystream (void);
};

    //
    // class Mystream constructor
    // This uses Mystreambase to set up the Mystreambuf
    // which can then be used to initialise iostream.
    //

inline
Mystream::Mystream (Myio * m)
    : Mystreambase (m), iostream (rdbuf())
{}

inline
Mystream::~Mystream (void)
    {}

# endif     // _Mystream_h
