// Class to display ANSI (msdos ANSI.SYS) files
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#if !defined(_ansisys_h)
#define _ansisys_h
#include "scrintrp.hpp"

class ansisys : public scrinterp
{
  typedef void (ansisys::*pdisp)(int ch);
  public:
    ansisys(video & v);

    virtual void putch(int ch);
    virtual void reset();

  private:
    pdisp dispfunc;         // Display function (state)

    void esc(int ch);       // ESC function handler
    void seq(int ch);       // Parser for ESC sequences

    cell_t  savx,             // Saved position
            savy;

    int evalargs(cell_t * plist);
};

#endif

