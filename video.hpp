// Simple text video control class for DOS & OS/2
// public domain
// by David L. Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet

#if !defined(_video_h)
#define _video_h

typedef unsigned short cell_t;

class video
{
  public:
    video();
    ~video();

    void cls();
    void scroll(cell_t left, cell_t top, cell_t right, cell_t bottom, cell_t lines, char dir =1);
    void gotoxy(cell_t x, cell_t y);
    void put(int ch);
    void put(char const * s);
    void repchr(int ch, int n);

    void maxxy(cell_t & cols, cell_t & rows);
    void wherexy(cell_t & x, cell_t & y);

    void setattr(cell_t c)    { attr = c; }
    void setfill(cell_t c)    { fill = c; }
    void setfg(cell_t c)      { attr = cell_t((attr & 0xf0)|(c & 0xf)); }
    void setbg(cell_t c)      { attr = cell_t((attr & 0x0f)|((c & 0xf) << 4)); }

    cell_t getattr() const    { return attr; }
    cell_t getfg() const      { return cell_t(attr & 0xf); }
    cell_t getbg() const      { return cell_t((attr & 0xf0) >> 4); }

    unsigned short getkey();

  private:
    void adjustcursor(cell_t cols, cell_t lines =0);
    void putcursor();

    cell_t maxx,
           maxy;
    cell_t curx,
           cury;
    cell_t attr,
           fill;
};

#endif

