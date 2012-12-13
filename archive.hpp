/*
**  ARCHIVE.HPP
**
**   Written by Jari Laaksonen (2:221/360.20), 2 Sep 1994
**   Based on the code by Heinz Ozwirk and David Gersic.
**
**   Free for all participants of the C_ECHO & other conferences where
**   this code is posted. Released to SNIPPETS by the authors.
**
**   See the file WhichArc.DOC about archive types and file offsets
**   for self extracting archives.
*/

#include <stdio.h>

// add new type identifiers here if you need to separate e.g. different
// LHA files. Add your new types also in the Arctypes array in the TEST part
// and give them new descriptions.

typedef enum
{
      ERROR = -1, UNKNOWN = 0,
      ARC,      ARC6,     PAK,     ZOO,     HPK,    RAR,    UC2,    SQZ,
      ARJ,      LARC,     LHARC,   LHA,     ZIP,    ZIP2,
      SFXARC,   SFXARC6,  SFXPAK,
      SFXARJ,   SFXLHARC, SFXLHA,  SFXZIP,  SFXZIP2,
      EXE,
      COUNT_ARCTYPES
} ARCTYPE;

const int BUFFSIZE = 128;

class Archive
{
protected:
      int  type, sfxtype;
      long offset;
      char *fingerprint;

public:
      Archive (long offs, char *fp, int t = UNKNOWN, int t2 = UNKNOWN) :
                  offset (offs), fingerprint (fp), type(t), sfxtype (t2) { };

      int Scan  (FILE *fp, char *szBuff, long flen);
      virtual int Check (char *szBuff);
};

class LhaArchive : public Archive
{
public:
      LhaArchive (long offs, char *fp) : Archive (offs, fp) { };
      int Check (char *szBuff);
};

class PakArchive : public Archive
{
public:
      PakArchive (long offs = 0) : Archive (offs, "") { };
      int Check (char *szBuff);
};
