/*
**  ASCII <=> EBCDIC conversion functions
*/

#ifndef A2E__H
#define A2E__H

unsigned char ASCIItoEBCDIC(const unsigned char c);   /* A2E.C          */
unsigned char EBCDICtoASCII(const unsigned char c);   /* A2E.C          */

extern int ascii2ebcdic[256];                         /* Toascii.C      */
extern int ebcdic2ascii[256];                         /* Toascii.C      */

#endif /* A2E__H */
