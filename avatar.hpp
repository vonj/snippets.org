// Class to display AVATAR/0+
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#if !defined(_avatar_h)
#define _avatar_h
#include "scrintrp.hpp"

class avatar : public scrinterp
{
  typedef void (avatar::*pdisp)(int ch);
  public:
    avatar(video & v);

    virtual void putch(int ch);
    virtual void reset();

  private:
    pdisp dispfunc;         // Display function (state)

    int counter;
    int arg1;
    int rptlen;
    char rptbuf[256];
    cell_t area[5];

    void rptchar(int ch);
    void setvideo(int ch);
    void setattr(int ch);
    void setpos(int ch);
    void rptseq(int ch);
    void scroll(int ch);
    void clrarea(int ch);
    void setarea(int ch);

    void resetattr(int ch);
};

#endif


