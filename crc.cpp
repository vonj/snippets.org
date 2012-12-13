/*
**    CRC Calculator by Paul Johnston
**    Version 1.0 Copyright 10/9/96 (September)
**    Explicitly released to the public domain 30-Oct-1996 by the author
*/

#include "crc.hpp"

unsigned long Reflect(unsigned long toref, char w)
{
       unsigned long v=0;

       for (int i = 1; i < (w + 1); i++)
       {
             if (toref & 1L)
                   v |= 1L << (w-i);
             toref>>=1;
       }
       return v;
}


unsigned long CrcCalc::Add (unsigned long crc, char *sCrc)
{
      crc^=crxo;
      if (fcrr)
      {
            while (*sCrc)
                  crc = (crc >> 8) ^ acrpt[(crc & 0xFF) ^ *sCrc++];
      }
      else
      {
            while (*sCrc)
                  crc = (crc << 8) ^ acrpt[((crc>>(crwd-8))&0xFF) ^ *sCrc++];
      }
      return (crc^crxo)&(-1l>>(32-crwd));
}

unsigned long CrcCalc::Add (unsigned long crc, PData sCrc)
{
      crc^=crxo;
      if (fcrr)
      {
            while (sCrc.l--)
                  crc = (crc >> 8) ^ acrpt[(crc & 0xFF) ^
                        *((unsigned char *)sCrc.pData)++];
      }
      else
      {
            while (sCrc.l--)
                  crc = (crc << 8) ^ acrpt[((crc>>(crwd-8))&0xFF) ^
                        *((unsigned char *)sCrc.pData)++];
      }
      return (crc^crxo)&(-1l>>(32-crwd));
}

unsigned long CrcCalc::Add (unsigned long crc, unsigned char sCrc)
{
      crc^=crxo;
      if (fcrr)
            crc = (crc >> 8) ^ acrpt[(crc & 0xFF) ^ sCrc];
      else  crc = (crc << 8) ^ acrpt[((crc>>(crwd-8))&0xFF) ^ sCrc];
      return (crc^crxo)&(-1l>>(32-crwd));
}

unsigned long CrcCalc::Init()
{
      return fcrr ? Reflect(crgi, crwd)^crxo : crgi^crxo;
}

CrcCalc::CrcCalc (Crst crst)
{
      switch(crst)
      {
      case Crc16:       InitCalc (0x1021l,     0xFFFFl,  0l, 0, 16); break;
      case Crc32:       InitCalc (0x04C11DB7l,     -1l, -1l, 1, 32); break;
      case JamCrc:      InitCalc (0x04C11DB7l,     -1l,  0l, 1, 32); break;
      case XModemCrc:   InitCalc (0x8408l,          0l,  0l, 1, 16); break;
      case ZModemCrc16: InitCalc (0x1021l,          0l,  0l, 0, 16); break;
      case ZModemCrc32: InitCalc (0x04C11DB7l,     -1l, -1l, 1, 32); break;
      }
}

CrcCalc::CrcCalc (unsigned long crpNew,
                  unsigned long crgiNew,
                  unsigned long crxoNew,
                  unsigned char fcrrNew,
                  unsigned char crwdNew)
{
      InitCalc (crpNew, crgiNew, crxoNew, fcrrNew, crwdNew);
}

void CrcCalc::InitCalc (unsigned long crpNew,
                        unsigned long crgiNew,
                        unsigned long crxoNew,
                        unsigned char fcrrNew,
                        unsigned char crwdNew)
{
      crp=crpNew;
      crgi=crgiNew;
      crxo=crxoNew;
      fcrr=fcrrNew;
      crwd=crwdNew;

      // Calculate look-up table

      for (unsigned long i = 0; i <= 0xFF; i++)
      {
            acrpt[i]=(fcrr ? Reflect(i,8) : i)<<(crwd-8);
            for (unsigned long j = 0; j < 8; j++)
                  acrpt[i]=(acrpt[i]<<1)^(acrpt[i]&(1l<<(crwd-1)) ? crp : 0);
            if (fcrr) acrpt[i]=Reflect(acrpt[i],crwd);
      }
}
