// myLine.h
//
// Donated to the public domain; no restrictions on reuse or abuse apply.
// by David Nugent, 7th June, 1993.
// Simple line input class for istream to demonstrate input of a complete
// line rather than whitespace separated tokens (the default for operator<<
// for char* and other built-in types).
// Works by overloading operator>> for a customised class - this functionality
// is easily incorporated into your favourite String class
//

# if !defined(_myLine_h)
# define _myLine_h 1

# define AUTO_GROW 1            // Allow autogrowth of buffer to fit
# define ALLOC_LEN 80           // Standard length & growth increment

    // Class declaration

class myLine
{

  public:

    myLine (short buflen =ALLOC_LEN);
    myLine (char * usebuf, short buflen =ALLOC_LEN);
    ~myLine (void);
                                       // Get buffer address
    char const * buf (void) const { return mybuf; }
                                       // Conversion operators
    char const * operator() (void) const { return mybuf; } // Explicit cast
    operator char const * (void) const { return mybuf; }   // Implicit cast
                                       // istream operator>>
    friend istream & operator>> (istream &, myLine &);

  private:

    short len, xalloc;
    char * mybuf;

};

# endif     // _myLine_h
