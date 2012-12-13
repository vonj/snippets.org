/* --------------------------------------------------------------------
   Module:     ntstream.cpp
   Subject:    Share-Aware File Streams
   Author:     Heinz Ozwirk
   Started:    30.05.1993 15:30:32
   Modified:   31.05.1993 10:33:35
   --------------------------------------------------------------------
   Description: public domain from the FidoNet C++ echo
   --------------------------------------------------------------------
   History:    (insert new entries at top of list)
   dd.mm.yyyy/ho description
   -------------------------------------------------------------------- */


/* --- Includes ------------------------------------------------------- */
#include "ntstream.h"
#include <fcntl.h>
#include <io.h>

/* --- Defines -------------------------------------------------------- */
/* --- Constants ------------------------------------------------------ */
/* --- Types ---------------------------------------------------------- */
/* --- Prototypes ----------------------------------------------------- */
/* --- Global Variables ----------------------------------------------- */
/* --- Local Variables ------------------------------------------------ */
/* --- Implementation ------------------------------------------------- */

static int newMode(int m)
{
   int how = 0;
   if (m & nfstream::sh_compat)        how |= SH_COMPAT;
   else if (m & nfstream::sh_none)     how |= SH_DENYRW;
   else if (m & nfstream::sh_read)
      {
      if (m & nfstream::sh_write)      how |= SH_DENYNO;
      else                             how |= SH_DENYWR;
      }
   else if (m & nfstream::sh_write)    how |= SH_DENYRD;
   else                                how |= SH_DENYRW;

   if (m & ios::out)
      {
      if (m & ios::in)
         how |= O_RDWR;
      else
         how |= O_WRONLY;
      if (!(m & ios::nocreate))
         {
         how |= O_CREAT;
         if (m & ios::noreplace)
            how |= O_EXCL;
         }
         if (m & ios::trunc)
            how |= O_TRUNC;
      }
    else
      {
      how |= O_RDONLY;
      }

   if (m & ios::binary)
      how |= O_BINARY;
   else
      how |= O_TEXT;

   if (m & ios::app)
      how |= O_APPEND;

   return how;
}

nfstream::nfstream(const signed char *name, int mode, int prot)
   :  fstream()
{
   open(name, mode, prot);
}

nfstream::nfstream(const unsigned char *name, int mode, int prot)
   :  fstream()
{
   open(name, mode, prot);
}

void nfstream::open(const signed char *name, int mode, int prot)
{
   int howM = newMode(mode);
   fd = ::open((const char *)name, howM, prot);
   if (fd == -1)
      setstate(failbit);
   else
      attach(fd);
}

void nfstream::close()
{
   if (fd != -1)
      {
      fstream::close();
      ::close(fd);
      fd = -1;
      }
}

nifstream::nifstream(const signed char *name, int mode, int prot)
   :  ifstream()
{
   open(name, mode, prot);
}

nifstream::nifstream(const unsigned char *name, int mode, int prot)
   :  ifstream()
{
   open(name, mode, prot);
}

void nifstream::open(const signed char *name, int mode, int prot)
{
   int howM = newMode(mode);
   fd   = ::open((const char *)name, howM, prot);
   if (fd == -1)
      setstate(failbit);
   else
      attach(fd);
}

void nifstream::close()
{
   if (fd != -1)
      {
      ifstream::close();
      ::close(fd);
      fd = -1;
      }
}

nofstream::nofstream(const signed char *name, int mode, int prot)
   :  ofstream()
{
   open(name, mode, prot);
}

nofstream::nofstream(const unsigned char *name, int mode, int prot)
   :  ofstream()
{
   open(name, mode, prot);
}

void nofstream::open(const signed char *name, int mode, int prot)
{
   int howM = newMode(mode);
   fd   = ::open((const char *)name, howM, prot);
   if (fd == -1)
      setstate(failbit);
   else
      attach(fd);
}

void nofstream::close()
{
   if (fd != -1)
      {
      ofstream::close();
      ::close(fd);
      fd = -1;
      }
}

/* --- End of File ---------------------------------------------------- */
