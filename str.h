//
// Simple string class
// Public domain
//
// Written by david nugent
// davidn@csource.pronet.com
// 3:632/348@fidonet
//

# if !defined( _str_h )
# define _str_h 1
# if !defined( _Bool_defined )
enum Bool { False, True };
# endif

# if defined( __BORLANDC__ ) && ( __BORLANDC__ <= 0x3000 )
# define PLACEMENT_NEW_BUG
# define SIGNED_CHAR_BUG
# endif

// Define to 0 for much smaller class
#define VIRTUAL_DESTRUCTOR  0
#if VIRTUAL_DESTRUCTOR
#define __VIRTUAL   virtual
#else
#define __VIRTUAL
#endif

struct refstr
{
    short _size;
    short _length;
    short _refs;
    unsigned short _flags;

    enum rsflags { ICASE=1 };

    refstr (short length, short size, unsigned short flgs =0)
        : _length(length), _size(size), _refs(1), _flags(flgs)
        {}
    ~refstr (void) {}
    unsigned short flags() const
    {
        return _flags;
    }
    void setf(unsigned short f)
    {
        _flags = (unsigned short)(_flags | f);
    }
    void resetf(unsigned short f)
    {
        _flags = (unsigned short)(_flags & ~f);
    }
# if !defined( PLACEMENT_NEW_BUG )
    void * operator new(unsigned sz, short allocsz);
# endif
    char * ptr (void)
    {
        return ((char *)this) + sizeof(refstr);
    }
};




class str
{

  public:

        // constructors/destructors

    str (void);
    str (char const * s, short len =-1);
    str (unsigned char const * s, short len =-1);
# if !defined( SIGNED_CHAR_BUG )
    str (signed char const * s, short len =-1);
# endif
    str (int val, int radix =10);
    str (unsigned int val, int radix =10);
    str (short val, int radix =10);
    str (unsigned short val, int radix =10);
    str (long val, int radix =10);
    str (unsigned long val, int radix =10);
    str (char c);
    str (unsigned char c);
# if !defined( SIGNED_CHAR_BUG )
    str (signed char c);
# endif
    str (str const & s);
    __VIRTUAL ~str (void);

    str & clear(void);

        // assignment

    str & operator= (str const & s);
    str & operator= (char const * s);
    str & operator= (char c);

    str & operator= (unsigned char const * s)
    {
        return operator= ((char const *)s);
    }
# if !defined( SIGNED_CHAR_BUG )
    str & operator= (signed char const * s)
    {
        return operator= ((char const *)s);
    }
# endif

        // primitive members

    short length (void) const
    {
        return strdata->_length;
    }

    short size (void) const
    {
        return strdata->_size;
    }

        // operators

    str & operator<< (char const * s);              // concatenate
    str & operator<< (unsigned char const * s);
# if !defined( SIGNED_CHAR_BUG )
    str & operator<< (signed char const * s);
# endif
    str & operator<< (str const & s);
    str & operator<< (int val);
    str & operator<< (unsigned int val);
    str & operator<< (short val);
    str & operator<< (unsigned short val);
    str & operator<< (long val);
    str & operator<< (unsigned long val);
    str & operator<< (char c);
    str & operator<< (unsigned char c);
# if !defined( SIGNED_CHAR_BUG )
    str & operator<< (signed char c);
# endif

    char const & operator[] (short pos) const;
    char & operator[] (short pos);

    char * c_ptr (void) const            // not necessarily NUL terminated!
    {                                                // Use with caution...
        return strdata->ptr();
    }
    char const * c_str (void) const    // return char*
    {
        char * buf = c_ptr();
        buf[strdata->_length] = 0;
        return buf;
    }
    unsigned char const * u_str (void) const
    {
        return (unsigned char const *)c_str();
    }
# if !defined( SIGNED_CHAR_BUG )
    signed char const * s_str (void) const
    {
        return (signed char const *)c_str();
    }
# endif

    int copy(char * dest, short maxlen =-1) const;

        // manipulators

    short insert (short pos, char const * s, short len =-1);
    short insert (short pos, str const & s)
    {
        return insert (pos, s.c_ptr(), s.length());
    }
    short insert (short pos, unsigned char const * s, short len =-1)
    {
        return insert (pos, (char const *)s, len);
    }
# if !defined( SIGNED_CHAR_BUG )
    short insert (short pos, signed char const * s,
                    short len =-1)
    {
        return insert (pos, (char const *)s, len);
    }
# endif
    short insert (short pos, char c)
    {
        return insert (pos, &c, 1);
    }
    short insert (short pos, unsigned char c)
    {
        return insert (pos, (char const *)&c, 1);
    }
# if !defined( SIGNED_CHAR_BUG )
    short insert (short pos, signed char c)
    {
        return insert (pos, (char const *)&c, 1);
    }
# endif

    short remove (short pos =0, short len =-1);
    short replace (short pos, char const * s, short clen =-1, short len =-1);
    short replace (short pos, str & s, short clen =-1)
    {
        return replace (pos, s.c_ptr(), clen, s.length());
    }
    short replace (short pos, unsigned char const * s, short clen =-1, short len =-1)
    {
        return replace (pos, (char const *)s, clen, len);
    }
# if !defined( SIGNED_CHAR_BUG )
    short replace (short pos, signed char const * s, short clen =-1, short len =-1)
    {
        return replace (pos, (char const *)s, clen, len);
    }
# endif
    short replace (short pos, char c, short clen =-1)
    {
        return replace (pos, &c, clen, 1);
    }
    short replace (short pos, unsigned char c, short clen =-1)
    {
        return replace (pos, (char const *)&c, clen, 1);
    }
# if !defined( SIGNED_CHAR_BUG )
    short replace (short pos, signed char c, short clen =-1)
    {
        return replace (pos, (char const *)&c, clen, 1);
    }
# endif

    str & left (short len, char padch =' ');
    str & right (short len, char padch =' ');
    str & mid (short pos, short len, char padch =' ');

    str substr(short start, short len =-1) const;

    short removech (char const * clist ="\r\n");
    short countch (char const * clist);

    Bool operator== (str const & s) const
    {
        return (_compare(s) == 0) ? True : False;
    }
    Bool operator== (char const * s) const
    {
        return (_compare(s) == 0) ? True : False;
    }
    Bool operator== (unsigned char const * s) const
    {
        return (_compare(s) == 0) ? True : False;
    }
# if !defined( SIGNED_CHAR_BUG )
    Bool operator== (signed char const * s) const
    {
        return (_compare(s) == 0) ? True : False;
    }
#endif

    Bool operator!= (str const & s) const
    {
        return (_compare(s) != 0) ? True : False;
    }
    Bool operator!= (char const * s) const
    {
        return (_compare(s) != 0) ? True : False;
    }
    Bool operator!= (unsigned char const * s) const
    {
        return (_compare(s) != 0) ? True : False;
    }
# if !defined( SIGNED_CHAR_BUG )
    Bool operator!= (signed char const * s) const
    {
        return (_compare(s) != 0) ? True : False;
    }
#endif

    Bool operator< (str const & s) const
    {
        return (_compare(s) < 0) ? True : False;
    }
    Bool operator< (char const * s) const
    {
        return (_compare(s) < 0) ? True : False;
    }
    Bool operator< (unsigned char const * s) const
    {
        return (_compare(s) < 0) ? True : False;
    }
# if !defined( SIGNED_CHAR_BUG )
    Bool operator< (signed char const * s) const
    {
        return (_compare(s) < 0) ? True : False;
    }
#endif

    Bool operator<= (str const & s) const
    {
        return (_compare(s) <= 0) ? True : False;
    }
    Bool operator<= (char const * s) const
    {
        return (_compare(s) <= 0) ? True : False;
    }
    Bool operator<= (unsigned char const * s) const
    {
        return (_compare(s) <= 0) ? True : False;
    }
# if !defined( SIGNED_CHAR_BUG )
    Bool operator<= (signed char const * s) const
    {
        return (_compare(s) <= 0) ? True : False;
    }
#endif

    Bool operator> (str const & s) const
    {
        return (_compare(s) > 0) ? True : False;
    }
    Bool operator> (char const * s) const
    {
        return (_compare(s) > 0) ? True : False;
    }
    Bool operator> (unsigned char const * s) const
    {
        return (_compare(s) > 0) ? True : False;
    }
# if !defined( SIGNED_CHAR_BUG )
    Bool operator> (signed char const * s) const
    {
        return (_compare(s) > 0) ? True : False;
    }
#endif

    Bool operator>= (str const & s) const
    {
        return (_compare(s) >= 0) ? True : False;
    }
    Bool operator>= (char const * s) const
    {
        return (_compare(s) >= 0) ? True : False;
    }
    Bool operator>= (unsigned char const * s) const
    {
        return (_compare(s) >= 0) ? True : False;
    }
# if !defined( SIGNED_CHAR_BUG )
    Bool operator>= (signed char const * s) const
    {
        return (_compare(s) >= 0) ? True : False;
    }
#endif

    int compare (str const & s) const
    {
        return _compare(s);
    }
    int compare (char const * s) const
    {
        return _compare(str(s));
    }
    int compare (unsigned char const * s) const
    {
        return _compare(str(s));
    }
# if !defined( SIGNED_CHAR_BUG )
    int compare (signed char const * s) const
    {
        return _compare(str(s));
    }
#endif

    short strstr (str const & s) const
    {
        return _strstr(s);
    }
    short strstr (char const * s) const
    {
        return _strstr(str(s));
    }
    short strstr (unsigned char const * s) const
    {
        return _strstr(str(s));
    }
# if !defined( SIGNED_CHAR_BUG )
    short strstr (signed char const * s) const
    {
        return _strstr(str(s));
    }
#endif

    void setflags (unsigned short flags);
    void resetflags (unsigned short flags);

    void setcase (Bool s =True)
    {
        if (s)
            setflags(refstr::ICASE);
        else
            resetflags(refstr::ICASE);
    }

    static void setdefaultcase (Bool s = True)
    {
        default_flags = (s == False)
                        ? (unsigned short)(default_flags | refstr::ICASE)
                        : (unsigned short)(default_flags & ~refstr::ICASE);
    }

  protected:

    static unsigned short default_flags;
    refstr * strdata;

    // Check to see if big enough for size
    int _chksize (short sz =0);
    int _concat (char const * s, short len =-1);
    int _concat (str const & s)
    {
        return _concat (s.c_ptr(), s.length());
    }
    int _concat (char ch)
    {
        return _concat (&ch, 1);
    }
    int _concat (unsigned char const * s, short len =-1)
    {
        return _concat ((char const *)s, len);
    }
# if !defined( SIGNED_CHAR_BUG )
    int _concat (signed char const * s, short len =-1)
    {
        return _concat ((char const *)s, len);
    }
# endif

    int _compare (str const s) const;
    short _strstr (str const s) const;

        // Common constructor

    void _strinit (char const * s =0, short slen =0,
                    short siz =-1, unsigned short flgs =default_flags);
    void _strinit (unsigned long val, Bool positive, int radix);

};


inline str
left (str s, short len, char padch =' ')
{
    return s.left(len, padch);
}

inline str
right (str s, short len, char padch =' ')
{
    return s.right(len, padch);
}

inline str
mid (str s, short pos, short len, char padch =' ')
{
    return s.mid(pos, len, padch);
}

inline int
compare(str s, str b)
{
    return s.compare(b);
}

inline int
compare(str s, char const * b)
{
    return s.compare(b);
}

inline int
compare(str s, unsigned char const * b)
{
    return s.compare(b);
}

# if !defined( SIGNED_CHAR_BUG )
inline int
compare(str s, signed char const * b)
{
    return s.compare(b);
}
# endif

class ostream;
class istream;

ostream & operator<< (ostream & os, str const & s);
istream & operator>> (istream & is, str & s);

# endif
