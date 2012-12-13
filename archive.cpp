/*
**  ARCHIVE.CPP
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

#include "archive.hpp"
#include "whicharc.h"

int Archive::Scan (FILE *fp, char *szBuff, long flen)
{
      if (offset > flen)
            return 0;

      int rc;

      rc = SeekFile (fp, szBuff, offset);
      if (rc > 0)
            rc = Check (szBuff);

      return (rc > UNKNOWN ? sfxtype : rc);
}

// Generic fingerprint check.
// LHA and PAK/ARC needs more complicated checking
// If some new archiver needs different check algorithm, derive a new
// class from class Archive and write a new Check() function.

int Archive::Check (char *szBuff)
{
      char *p;

      p = fingerprint;
      while (*p)
      {
            if (*p != *szBuff)
                  return UNKNOWN;
            p++;
            szBuff++;
      }

      return type;
}

int LhaArchive::Check (char *szBuff)
{
      int i, c;

      for (c = 0, i = szBuff[0]; i--; c += (szBuff + 2)[i])
            ;
      if (
          ((unsigned char) (c & 0x00FF)) == szBuff[1]
          && szBuff[2] == fingerprint[0]
          && szBuff[3] == fingerprint[1]
          && szBuff[4] == fingerprint[2]
          && szBuff[6] == fingerprint[3]
         )
      {
            if (szBuff[5] > '1')
            {
                  sfxtype = SFXLHA;
                  return LHA;
            }
            else
            {
                  sfxtype = SFXLHARC;
                  return LHARC;
            }
      }
      else  return UNKNOWN;
}

int PakArchive::Check (char *szBuff)
{
      if (szBuff[0] == 0x1A)
      {
            if (szBuff[1] == 0x0A || szBuff[1] == 0x0B)
            {
                  sfxtype = SFXPAK;
                  return PAK;
            }
            else if (szBuff[1] > 0x14)
            {
                  sfxtype = SFXARC6;
                  return ARC6;
            }
            else
            {
                  sfxtype = SFXARC;
                  return ARC;
            }
      }
      else  return UNKNOWN;
}
