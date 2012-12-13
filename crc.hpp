/*
**    CRC Calculator by Paul Johnston
**    Version 1.0 Copyright 10/9/96 (September)
**    Explicitly released to the public domain 30-Oct-1996 by the author
*/

/* Custom Hungarian types:
**
** crp  - CRC Polynomial
** crgi - CRC reGister Inital value
** crxo - CRC Xor Out value
** crpt - CRC Precalculated Table
** crr  - is CRC Reversed
** crst - CRC Standard Type
*/

#ifndef PAJ_CRC
#define PAJ_CRC

class PData {
public:
       unsigned long l;
       void *pData;
};

unsigned long Reflect(unsigned long toref, char w);

enum Crst {
       Crc16=0,
       Crc32,
       JamCrc,
       XModemCrc,
       ZModemCrc16,
       ZModemCrc32
};

class CheckCalc {
public:
       virtual unsigned long Add (unsigned long crc, char *sCrc)=0;
       virtual unsigned long Add (unsigned long crc, unsigned char sCrc)=0;
       virtual unsigned long Add (unsigned long crc, PData sCrc)=0;
       virtual unsigned long Init()=0;
};

class CrcCalc : CheckCalc {
private:
       unsigned long acrpt[256], crp, crgi, crxo;
       unsigned char fcrr, crwd;
       void InitCalc (unsigned long crpNew, unsigned long crgiNew,
                  unsigned long crxoNew, unsigned char fcrrNew,
                  unsigned char crwdNew);

public:
       unsigned long Add (unsigned long crc, char *sCrc);
       unsigned long Add (unsigned long crc, unsigned char sCrc);
       unsigned long Add (unsigned long crc, PData sCrc);
       unsigned long Init();
       CrcCalc (unsigned long crpNew, unsigned long crgiNew=0,
            unsigned long crxoNew=0, unsigned char fcrrNew=1,
            unsigned char crwdNew=32);
            CrcCalc (Crst crst);
};

class Crc {
private:
       unsigned long crc;
       CheckCalc *crccalc;

public:
       unsigned long crcGet()    { return crc; };
       void Add (char *sCrc)     { crc=crccalc->Add(crc, sCrc); }
       void Add (char sCrc)      { crc=crccalc->Add(crc, sCrc); }
       void Add (PData sCrc)   { crc=crccalc->Add(crc, sCrc); }
       void NewCrc ()        { crc=crccalc->Init(); }

       operator unsigned long ()   { return crcGet(); }
       void operator << (char *sCrc)  { Add(sCrc); }
       void operator << (char sCrc)  { Add(sCrc); }
       void operator << (PData sCrc)   { Add(sCrc); }
       void operator = (int iNew)   { if (iNew==0) NewCrc(); }

       Crc(CheckCalc *crccalcNew)   { crccalc=crccalcNew; NewCrc(); }
};

#endif // PAJ_CRC
