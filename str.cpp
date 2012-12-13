//
// Implements simple string class 'str'
//

# include   "str.h"
# include   <string.h>
# include   <ctype.h>
# include   "snip_str.h"                  /* For stristr()  */

#if defined(_MSC_VER)
 #include <memory.h>
#elif defined(__TURBOC__)
 #include <mem.h>
#else
 #include <string.h>
#endif

# include   <stdlib.h>
# if defined( _MSC_VER )
#  pragma warning(disable:4505)
# endif

# define STDLEN 32

extern "C" void * malloc (unsigned sz);
extern "C" void free (void * ptr);


# if defined( PLACEMENT_NEW_BUG )

inline void *
operator new (unsigned sz, short allocsz)
{
    return malloc (sz + allocsz);
}

# else

void *
refstr::operator new (unsigned sz, short allocsz)
{
    return malloc (sz + allocsz);
}

# endif

unsigned short str::default_flags = 0;

str::str(void)
{
    _strinit();
}

str::str(char const * s, short len)
{
    _strinit(s, len, -1);
}

str::str(unsigned char const * s, short len)
{
    _strinit((char const *)s, len, -1);
}

# if !defined( SIGNED_CHAR_BUG )
str::str (signed char const * s, short len)
{
    _strinit((char const *)s, len, -1);
}
# endif

str::str (char c)
{
    _strinit (&c, 1, -1);
}

str::str (int val, int radix)
{
    Bool    positive;

    if (val >= 0)
        positive = True;
    else
    {
        positive = False;
        val = -val;
    }
    _strinit((unsigned long)val, positive, radix);
}

str::str (unsigned int val, int radix)
{
    _strinit((unsigned long)val, True, radix);
}

str::str (short val, int radix)
{
    Bool    positive;

    if (val >= 0)
        positive = True;
    else
    {
        positive = False;
        val = short(-val);
    }
    _strinit((unsigned long)val, positive, radix);
}

str::str (unsigned short val, int radix)
{
    _strinit((unsigned long)val, True, radix);
}

str::str (long val, int radix)
{
    Bool positive;

    if (val >= 0L)
        positive = True;
    else
    {
        positive = False;
        val = -val;
    }
    _strinit((unsigned long)val, positive, radix);
}

str::str (unsigned long val, int radix)
{
    _strinit(val, True, radix);
}

str::str (unsigned char c)
{
    _strinit ((char const *)&c, 1, -1);
}

# if !defined( SIGNED_CHAR_BUG )
str::str (signed char c)
{
    _strinit ((char const *)&c, 1, -1);
}

# endif

str::str (str const & s)
  : strdata(s.strdata)
{
    ++strdata->_refs;
}

str::~str (void)
{
    if (!--strdata->_refs)
        delete strdata;
}

void
str::_strinit (char const * s, short len, short siz, unsigned short flgs)
{
    if (len < 0)
        len = (short) ((s) ? strlen (s) : 0);
    if (siz < 0)
        siz = STDLEN;
    if (siz < short(len + 1))
        siz = short(len + 1);
    strdata = new(siz) refstr(len, siz, flgs);
    if (s && len)
        memcpy (c_ptr(), s, len);
}

void
str::_strinit (unsigned long val, Bool positive, int radix)
{
    char    buf[32], * p = buf;

    if (!positive)
        *p = '-';
    ultoa(val, p, radix);
    _strinit(buf, -1, 0);
}

str &
str::clear(void)
{
    if (strdata->_refs == 1)
        strdata->_length = 0;
    else
    {
        --strdata->_refs;
        _strinit(0, False, -1);
    }
    return *this;
}

        // Called whenever string is to be modified or grown
int
str::_chksize (short sz)
{
    refstr * old = 0;
    if (strdata->_refs > 1) // Need to dup memory
        --strdata->_refs;   // Dec existing string reference
    else if (sz >= size())
        old = strdata;
    else
        return 0;
    _strinit (c_ptr(), length(), sz);
    delete old;
    return 1;
}

str &
str::operator= (str const & s)
{
    if (&s != this)
    {
        if (!--strdata->_refs)
            delete strdata;
        strdata = s.strdata;
        ++strdata->_refs;
    }
    return *this;
}

str &
str::operator= (char const * s)
{
    if (s != c_ptr())
    {
        short len = (short) strlen (s);
        _chksize (short(len + 1));
        memcpy (c_ptr(), s, len);
        strdata->_length = len;
    }
    return *this;
}

str &
str::operator= (char c)
{
    _chksize (2);
    *c_ptr() = c;
    strdata->_length = 1;
    return *this;
}

int
str::copy(char * dst, short maxlen) const
{
    if (maxlen == -1)
        maxlen = short(length() + 1);
    short len = short(maxlen - 1);
    if (len > length())
        len = length();
    if (len > 0)
        memcpy(dst, c_ptr(), len);
    if (len >= 0)
        dst[len] = '\0';
    return len;
}

short
str::insert (short pos, char const * s, short len)
{
    if (len < 0)
        len = (short) strlen (s);
    if (len)
    {
        short leng = strdata->_length;
        if (pos < 0 || pos > leng)
            pos = leng;
        _chksize (short(leng + len + 1));
        char * buf = c_ptr();
        if (pos < leng)
            memmove (buf + pos + len, buf + pos, leng - pos);
        memcpy (buf + pos, s, len);
        strdata->_length += len;
    }
    return length();
}

short
str::remove (short pos, short len)
{
    if (pos >= 0 && pos < length())
    {
        short leng = strdata->_length;
        if (len < 0 || (pos + len) > leng)
            len = short(leng - pos);
        if (len)
        {
            _chksize (0);
            char * buf = c_ptr();
            memcpy (buf + pos, buf + pos + len, leng - (pos + len));
            strdata->_length -= len;
        }
    }
    return length();
}

short
str::replace (short pos, char const * s, short clen, short len)
{
    if (pos >= 0)
    {
        short leng = strdata->_length;
        if (clen < 0 || (pos + clen) > leng)
            clen = short(leng - pos);
        if (len < 0)
            len = (short) strlen (s);
        if (pos > leng)
            pos = leng;
        _chksize (short(leng - clen + len + 1));
        char * buf = c_ptr();
        if (clen != len && clen)
            memmove (buf + pos + len, buf + pos + clen,
                     leng - (pos + clen - len));
        if (len)
            memcpy (buf + pos, s, len);
        strdata->_length += short(len - clen);
    }
    return length();
}

str &
str::left (short len, char padch)
{
    if (len < 0)
        return right (short(-len), padch);
    short leng = strdata->_length;
    if (len != leng)
    {
        _chksize (short(len + 1));
        if (len > leng)
            memset (strdata->ptr() + leng, padch, len - leng);
        strdata->_length = len;
    }
    return *this;
}

str &
str::right (short len, char padch)
{
    if (len < 0)
        return left(-1, padch);
    short leng = strdata->_length;
    if (len != leng)
    {
        _chksize (short(len + 1));
        if (len > leng)
        {
            char * buf = strdata->ptr();
            memmove (buf + len - leng, buf, leng);
            memset (buf, padch, len - leng);
        }
        strdata->_length = len;
    }
    return *this;
}

str &
str::mid (short pos, short len, char padch)
{
    if (pos <= 0)
        return left(len, padch);
    short leng = strdata->_length;
    if (pos > leng)
        pos = leng;
    if (leng < len)         // Are we padding?
    {
        _chksize (short(len + 1));
        char * buf = strdata->ptr();
        short nlen = short((len - (leng - pos)) / 2);
        if (nlen > 0)
        {
            memmove (buf, buf + pos, leng - pos);
            memset (buf + leng - pos, padch, nlen);
            strdata->_length -= short(pos - nlen);
        }
    }
    return right (len, padch);
}

str
str::substr(short start, short len) const
{
    if (start < 0)
        start = short(length() + start);
    if (start < 0 || start >= strdata->_length)
        return str();   // Empty
    if (len < 0 || (short(start + len) > strdata->_length))
        len = short(strdata->_length - start);
    return str(c_ptr() + start, len);
}


int
str::_concat (char const * s, short len)
{
    if (len < 0)
        len = (short) strlen (s);
    if (len > 0)
    {
        // Special case - are we concatenating ourselves??
        if (strdata->_refs == 1 &&  // No danger if we'll be reallocated anyway
            s >= c_ptr() &&         // Refers to us, or substring of us
            s <= (c_ptr() + length()))
        {   // This is handled separately, since we do not wish
            // to pass this heinous overhead onto all cases,
            // especially when this particular case is so rare...
            str tmpstr(s, len);                 // Copy this string first
            _chksize(short(len + length() + 1));
            memcpy(c_ptr() + length(), tmpstr.c_ptr(), len);
        }
        else
        {
            _chksize (short(len + length() + 1));
            memcpy (c_ptr() + length(), s, len);
        }
        strdata->_length += len;
    }
    return length();
}

str &
str::operator<< (char const * s)    // concatenate
{
    _concat (s);
    return *this;
}

str &
str::operator<< (unsigned char const * s)
{
    _concat ((char const *)s);
    return *this;
}

# if !defined( SIGNED_CHAR_BUG )
str &
str::operator<< (signed char const * s)
{
    _concat ((char const *)s);
    return *this;
}
# endif

str &
str::operator<< (str const & s)
{
    _concat (s);
    return *this;
}

str &
str::operator<< (int val)
{
    _concat (str(val));
    return *this;
}

str &
str::operator<< (unsigned int val)
{
    _concat (str(val));
    return *this;
}

str &
str::operator<< (short val)
{
    _concat (str(val));
    return *this;
}

str &
str::operator<< (unsigned short val)
{
    _concat (str(val));
    return *this;
}

str &
str::operator<< (long val)
{
    _concat (str(val));
    return *this;
}

str &
str::operator<< (unsigned long val)
{
    _concat (str(val));
    return *this;
}

str &
str::operator<< (char c)
{
    _concat (c);
    return *this;
}

str &
str::operator<< (unsigned char c)
{
    _concat (c);
    return *this;
}

# if !defined( SIGNED_CHAR_BUG )
str &
str::operator<< (signed char c)
{
    _concat (c);
    return *this;
}
# endif

    // String is never modified in this version

char const &
str::operator[] (short pos) const
{
    if (pos < 0)            // Negative index addresses from eos
        pos = short(strdata->_length + pos);
    if (pos >= strdata->_length)
    {
        char * buf = c_ptr() + length();
        *buf = 0;
        return *buf;
    }
    return c_ptr()[pos];
}

    // ... but here it may be

char &
str::operator[] (short pos)
{
    if (pos < 0)                       // Negative index addresses from eos
        pos = short(strdata->_length + pos);
    if (pos < 0)                     // Any cleaner way without exceptions?
        pos = strdata->_length;
    if (pos < strdata->_length)
        _chksize(0);
    else
    {
        _chksize(short(pos + 2));
        ::memset(c_ptr() + length(), ' ', pos - strdata->_length + 1);
        strdata->_length = short(pos+1);
    }
    return c_ptr()[pos];
}

int
str::_compare(str const s) const
{
    if ((strdata->flags() & refstr::ICASE) ||
        (s.strdata->flags() & refstr::ICASE))
        return stricmp(c_str(), s.c_str());
    return strcmp(c_str(), s.c_str());
}

#if 0
extern "C" char *
stristr(char const * s, char const * u)
{
    while (*s)
    {
        int     i = -1;
        do
            if (u[++i] == '\0')
                return (char *)s;
        while (toupper(s[i]) == toupper(u[i]));
        ++s;
    }
    return 0;
}
#endif

short
str::_strstr(str const s) const
{
    char    *p;
    if (!(strdata->flags() & refstr::ICASE) &&
        !(s.strdata->flags() & refstr::ICASE))
        p = ::strstr(c_str(), s.c_str());
    else
        p = ::stristr(c_str(), s.c_str());
    return short((p) ? (p - strdata->ptr()) : -1);
}


short
str::removech (char const * clist)
{
    short result = 0;
    if (*clist)
    {
        char * buf, * sub, * bas;
        bas = buf = sub = strdata->ptr();
        short nlen = strdata->_length;
        for (short i = 0; i < nlen; ++i)
        {
            if (strchr (clist, *buf) == 0)
            {
                if (result)
                    *sub = *buf;
                ++sub;
            }
            else if (!result++)
            {
                _chksize (0);
                buf = strdata->ptr() + (buf - bas);
                sub = strdata->ptr() + (sub - bas);
                bas = strdata->ptr();
            }
            ++buf;
        }
        strdata->_length = short(nlen - result);
    }
    return result;
}

short
str::countch (char const * clist)
{
    short result = 0;
    if (*clist)
    {
        char * buf = strdata->ptr();
        short nlen = strdata->_length;
        for (short i = 0; i < nlen; ++i, ++buf)
            if (strchr (clist, *buf) != 0)
                ++result;
    }
    return result;
}

void
str::setflags (unsigned short flags)
{
    if ((strdata->flags() & refstr::ICASE) != flags)
    {
        _chksize(0);        // Dupe string if necessary
        strdata->setf(flags);
    }
}

void
str::resetflags (unsigned short flags)
{
    if ((strdata->flags() & flags) != 0)
    {
        _chksize(0);        // Dupe string if necessary
        strdata->resetf(flags);
    }
}


#include    <iostream.h>

ostream &
operator<< (ostream & os, str const & s)
{
    os << s.c_str();
    return os;
}

istream &
operator>> (istream & is, str & s)
{
    s.clear();
    while (is.good())
    {
        int     c;
        char    buf[256];

        is.get(buf, sizeof buf);
        s << buf;
        if ((c = is.get()) == '\n' || c == EOF)
            break;
        is.putback((char)c);
    }
    return is;
}
