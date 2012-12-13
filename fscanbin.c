/* fscanbin.c -- scan binary fields via format string
**
**  public domain by Ray Gardner   Englewood, Colorado   11/29/89
**
**  Usage: fscanbin(FILE *fp, char *format, ...)
**
**  where format string contains specifiers:
**   -ddd  means skip ddd bytes
**   i     means read a 16-bit int
**   l     means read a 32-bit int
**   sddd  means read a character string of up to ddd bytes
**           reads up to a nul byte if ddd is zero or missing
**   cnnn  means read a character field of nnn bytes (not nul-terminated)
**           reads one byte if nnn is zero or missing
*/

#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "fscanbin.h"

#define SWAP16 0
#define SWAP32 0

int fscanbin (FILE *fp, char *format, ...)
{
      va_list argp;
      unsigned char *p;
      unsigned k;
      int c;
      char *charp;
      WORD *WORDp;
      DWORD *DWORDp;
      int bytes_read;

      bytes_read = 0;
      va_start(argp, format);
      for ( p = (unsigned char *)format; *p; )
      {
            switch( *p & 0xFF )
            {
            case '-':
                  for ( k = 0, c = *++p; isdigit(c); c = *++p )
                        k = 10 * k + c - '0';
                  if ( k == 0 )
                        k = 1;
                  if ( fseek(fp, (long)k, SEEK_CUR) )
                        return -2;     /* i/o error */
                  bytes_read += k;
                  break;

            case 'i':
                  WORDp = va_arg(argp, WORD *);
                  if ( fread((void *)WORDp, sizeof(WORD), 1, fp) != 1 )
                        return -2;     /* i/o error */
#if SWAP16
                  WORDswap(WORDp);
#endif
                  p++;
                  bytes_read += sizeof(WORD);
                  break;

            case 'l':
                  DWORDp = va_arg(argp, DWORD *);
                  if ( fread((void *)DWORDp, sizeof(DWORD), 1, fp) != 1 )
                        return -2;     /* i/o error */
#if SWAP32
                  DWORDswap(DWORDp);
#endif
                  p++;
                  bytes_read += sizeof(DWORD);
                  break;

            case 's':
                  charp = va_arg(argp, char *);
                  for ( k = 0, c = *++p; isdigit(c); c = *++p )
                        k = 10 * k + c - '0';
                  do
                  {
                        c = getc(fp);
                        if ( c == EOF )
                              return -2;
                        *charp++ = (char)c;
                        bytes_read++;
                  } while ( c && (k == 0 || --k) );
                  break;

            case 'c':
                  charp = va_arg(argp, char *);
                  for ( k = 0, c = *++p; isdigit(c); c = *++p )
                        k = 10 * k + c - '0';
                  if ( k == 0 )
                        k = 1;
                  if ( fread((void *)charp, sizeof(char), k, fp) != k )
                        return -2;     /* i/o error */
                  bytes_read += k;
                  break;

            default:
                  return -1; /* bad format */
            }
      }
      va_end(argp);
      return bytes_read;
}
