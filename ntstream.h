/* --------------------------------------------------------------------
   Module:     ntstream.h
   Subject:    Share-Aware File Streams
   Author:     Heinz Ozwirk
   Started:    30.05.1993 15:22:54
   Modified:   31.05.1993 10:28:24
   --------------------------------------------------------------------
   Description: public domain from the FidoNet C++ echo
   --------------------------------------------------------------------
   History:    (insert new entries at top of list)
   dd.mm.yyyy/ho description
   -------------------------------------------------------------------- */

#if !defined __NETSTREAM_H
#define __NETSTREAM_H

/* --- Includes ------------------------------------------------------- */
#include <fstream.h>
#include <share.h>

/* --- Defines -------------------------------------------------------- */
/* --- Constants ------------------------------------------------------ */
/* --- Types ---------------------------------------------------------- */

class nfstream: public fstream
   {
   public:
      enum
         {
         sh_compat = 1 << 15,
         sh_none   = 1 << 14,
         sh_read   = 1 << 13,
         sh_write  = 1 << 12
         };

      nfstream(): fstream() {};
      nfstream(const signed char *name, int mode, int prot =
filebuf::openprot);
      nfstream(const unsigned char *name, int mode, int prot =
filebuf::openprot);
      nfstream(int fd): fstream(fd) {};
      nfstream(int fd, char *buffer, int mode)
         : fstream(fd, buffer, mode) {};
      ~nfstream() { close(); }

      void open(const signed char *name, int mode, int prot =
filebuf::openprot);
      void open(const unsigned char *name, int mode, int prot =
filebuf::openprot)
         {
         open((const signed char *) name, mode, prot);
         };
      void close();

   private:
      int fd;
   };

class nifstream: public ifstream
   {
   public:
      enum
         {
         sh_compat = 1 << 15,
         sh_none   = 1 << 14,
         sh_read   = 1 << 13,
         sh_write  = 1 << 12
         };

      nifstream(): ifstream() {};
      nifstream(const signed char *name, int mode, int prot =
filebuf::openprot);
      nifstream(const unsigned char *name, int mode, int prot =
filebuf::openprot);
      nifstream(int fd): ifstream(fd) {};
      nifstream(int fd, char *buffer, int mode)
         : ifstream(fd, buffer, mode) {};
      ~nifstream() { close(); }

      void open(const signed char *name, int mode, int prot =
filebuf::openprot);
      void open(const unsigned char *name, int mode, int prot =
filebuf::openprot)
         {
         open((const signed char *) name, mode, prot);
         };
      void close();

   private:
      int fd;
   };

class nofstream: public ofstream
   {
   public:
      enum
         {
         sh_compat = 1 << 15,
         sh_none   = 1 << 14,
         sh_read   = 1 << 13,
         sh_write  = 1 << 12
         };

      nofstream(): ofstream() {};
      nofstream(const signed char *name, int mode, int prot =
filebuf::openprot);
      nofstream(const unsigned char *name, int mode, int prot =
filebuf::openprot);
      nofstream(int fd): ofstream(fd) {};
      nofstream(int fd, char *buffer, int mode)
         : ofstream(fd, buffer, mode) {};
      ~nofstream() { close(); }

      void open(const signed char *name, int mode, int prot =
filebuf::openprot);
      void open(const unsigned char *name, int mode, int prot =
filebuf::openprot)
         {
         open((const signed char *) name, mode, prot);
         };
      void close();

   private:
      int fd;
   };

/* --- Prototypes ----------------------------------------------------- */
/* --- External Variables --------------------------------------------- */

#endif

/* --- End of File ---------------------------------------------------- */
