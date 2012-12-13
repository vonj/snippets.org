// Myio.h
// Specialised I/O class to demonstrate use of C++ iostream
// facilities in a customised environment
// Written by David L Nugent, June 1993.
//

# if !defined(_Myio_h)
# define _Myio_h 1

    // Forward declare classes

class Myio;
class Mystreambuf;
class Mystreambase;
class Mystream;

    // Forward declare iostream classes

class iostream;

    //
    // class Myio
    // This is a simplistic class which simply fields
    // input and output to a simulated stream device.
    //
    // In fact, it doesn't really do much at all other
    // than read input from and send output to a
    // circular queue, as though talking via a loopback
    // pipe to itself.
    //


class Myio
{
    friend class Mystreambuf;

  public:

    Myio (int sz =2048);                    // sz = buffer size to allocate
    virtual ~Myio (void);

    iostream & stream (void);               // Return (or create) stream

    int readok (void) const;                // Underflow check
    int writeok (void) const;               // Overflow check
    int gcount (void) const;                // Get # of chrs last read
    int pcount (void) const;                // Get # of chrs last written
    int count (void) const;                 // Get # of chrs in buffer
    int size (void) const;                  // Get size of buffer
    int dump (void) const;                  // Debugging - dumps buffer

    int write (char const * buf, int len);  // Put data into 'pipe'
    int read (char * buf, int max);         // Read data from our 'pipe'

  private:

    enum
    {
        overflow    = 0x0001,   // Last write only partial
        underflow   = 0x0002    // Last read only partial
    };

    unsigned stat;              // Last read/write status
    int _pcount;                // Last write count
    int _gcount;                // Last read count
    int bufsize;                // Size of our buffer
    int bufchars;               // Chrs in buffer now
    int bufidx;                 // Index into buffer (next put)
    char * bufaddr;             // Pointer to buffer
    Mystream * mystream;        // Stream associated with this object

};

inline int
Myio::readok (void) const
    {   return ((stat & Myio::underflow) == 0); }

inline int
Myio::writeok (void) const
    {   return ((stat & Myio::overflow) == 0);  }

inline int
Myio::gcount (void) const
    {   return _gcount;     }

inline int
Myio::pcount (void) const
    {   return _pcount;     }

inline int
Myio::count (void) const
    {   return bufchars;    }

inline int
Myio::size (void) const
    {   return bufsize;     }

# endif     // _Myio_h
