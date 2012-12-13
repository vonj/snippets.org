// Myiodemo.cpp
// This is a trivial program which uses the myio loopback class
// to demonstrate the basics on writing an io interface using
// the AT&T C++ iostream classes.
// The program simply provides the ability to selectively add
// to or read from a Myio instance and display information to
// assist in understanding how it all works.
//

# include "Mystream.h"      // This includes Myio.h and iostream.h
# include "myLine.h"
# include <conio.h>         // For getch()
# include <ctype.h>         // For toupper()
# include <string.h>

# define NL char('\n')

    // Let's do the "application is a class" trick

class myApplication
{
            // Defines a pointer to member function type
            // used for dispatching the menu

    typedef void (myApplication::*pvf) (void);

  public:

    myApplication (void) : mio() {}
    int execute (void);

  private:

    iostream & stream (void) { return mio.stream(); }
    int domenu (void);
    void send (void);
    void read (void);
    void disp (void);
    void peek (void);
    void flsh (void);
    void stat (void);

    pvf choice;     // Function called to execute
    Myio mio;       // IO object

};


void
myApplication::disp (void)
{
    cout << "Mystream status:" << NL
         << "Chrs in output buffer = " << stream().rdbuf()->out_waiting() << NL

         << "Chrs in  input buffer = " << stream().rdbuf()->in_avail()    << NL

         << "Myio object status = "
            << mio.count() << char('/') << mio.size()
            << " LastWrite=" << (mio.writeok() ? "OK" : "Incomplete")
            << " LastRead=" << (mio.readok() ? "OK" : "EOF")
         << endl;
}

    // Request a line and send it to the IO device

void
myApplication::send (void)
{
    cout << NL << "Enter text to write - press <ENTER> when done\n:";
    myLine L;
    cin >> L;
    int l = strlen(L);
    if (!l)
        cerr << "Nothing entered." << endl;
    else
    {
        cout << "Writing '"
             << L
             << char('\'')
             << endl;
        stream() << L << NL;    // Send the entered data, NL terminated
        cout << "Chrs written to Myio object = " << (l + 1) << NL;
        disp ();
    }
}

void
myApplication::read (void)
{
    cout << NL << "Reading a line from object:" << NL;
    myLine L;
    mio.stream().clear();
    mio.stream() >> L;
    int l = strlen(L);
    if (!l)
    {
        cout << "Nothing read." << endl;
        mio.stream().clear();       // Clear EOF status
    }
    else
    {
        cout << "Read '"
             << L
             << char('\'')
             << endl;
        cout << "Chrs read from Myio object = " << (l + 1) << NL;
        disp ();
    }
}

void
myApplication::flsh (void)
{
    cout << NL << "Flushing stream" << endl;
    stream() << flush;
    disp ();
}

void
myApplication::stat (void)
{
    cout << NL << "Myio object buffer dump:" << NL;
    mio.dump();
    disp ();
    stream().rdbuf()->dbp();    // Dump stream info
}

int
myApplication::domenu (void)
{
    cout << NL
         << "W)rite  R)ead  D)ump  F)lush  Q)uit\n"
         << "Select: "
         << flush;      // Need to flush here for portability
    int key;
    for (;;)
    {
        key = getch ();
        switch (toupper(key))
        {
        case 'W': choice = &myApplication::send;    break;
        case 'R': choice = &myApplication::read;    break;
        case 'D': choice = &myApplication::stat;    break;
        case 'F': choice = &myApplication::flsh;    break;
        case 'Q': key = 0;                          break;
        default:
            continue;
        }
        cout << char(key) << endl;
        break;
    }
    return key;
}

int                         // This is really the application
myApplication::execute (void)
{
    while (domenu ())
        (this->*choice) ();
    return 0;
}

int
main (void)
{
    myApplication Demo;     // Declare the application
    return Demo.execute (); // go for it!
}
