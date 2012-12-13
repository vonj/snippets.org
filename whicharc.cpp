/*
** WhichArc.CPP
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
#include <string.h>

#include "archive.hpp"
#include "whicharc.h"


/*
**  Definitions of normal archives, excluding LHA archives
*/

#define ARCS (sizeof(Arcs)/sizeof(Arcs[0]))

Archive Arcs[] = {
      Archive (0,  "PK\x03\x04",    ZIP),
      Archive (0,  "\x60\xEA",      ARJ),
      Archive (0,  "ZOO",           ZOO),
      Archive (0,  "HPAK",          HPK),
      Archive (0,  "Rar!\x1a\x7\0", RAR),
      Archive (0,  "UC2\x1a",       UC2),
      Archive (0,  "HLSQZ",         SQZ)
};

/*
**  Definitions of LHA archives
*/

#define LHARCS (sizeof(LhArcs)/sizeof(LhArcs[0]))

LhaArchive LhArcs[] = {
      LhaArchive (0, "-lz-"),
      LhaArchive (0, "-lh-")
};

/*
**  Definitions of self-extracting archives, excluding LHA & PAK archives
*/

#define SFXARCS (sizeof(SfxArcs)/sizeof(SfxArcs[0]))

Archive SfxArcs[] = {
      Archive (0x31F0, "PK\x03\x04\x0A", ZIP,   SFXZIP),
      Archive (0x31F0, "PK\x03\x04\x14", ZIP2,  SFXZIP2),
      Archive (0x3CCB, "PK\x03\x04",     ZIP2,  SFXZIP2),
      Archive (0x3D9A, "PK\x03\x04",     ZIP2,  SFXZIP2),

      // PkZip 1.02 for OS/2 family mode
      Archive (0x6ED6, "PK\x03\x04\x0A", ZIP,   SFXZIP),

      // PkZip 1.02 for OS/2 native mode
      Archive (0x5901, "PK\x03\x04\x0A", ZIP,   SFXZIP),

      Archive (0xBBA,  "PK\x03\x04\x0A", ZIP,   SFXZIP),
      Archive (0xBBA,  "PK\x03\x04\x14", ZIP2,  SFXZIP2),

      // Zip 2.0.1 for OS/2
      Archive (0x3CE4, "PK\x03\x04\x14", ZIP2,  SFXZIP2),

      Archive (0x39BA, "\x60\xEA",       ARJ,   SFXARJ),  // arj 2.22
      Archive (0x1511, "\x60\xEA",       ARJ,   SFXARJ),  // arj 2.22 sfxjr
      Archive (0x3A0A, "\x60\xEA",       ARJ,   SFXARJ),  // arj 2.30
      Archive (0x14D1, "\x60\xEA",       ARJ,   SFXARJ),  // arj 2.30 sfxjr
      Archive (0x3B02, "\x60\xEA",       ARJ,   SFXARJ),  // arj 2.41
      Archive (0x4F4,  "\x60\xEA",       ARJ,   SFXARJ)   // arj 2.41 sfxjr
};

/*
**  Definitions of self-extracting LHA archives
*/

#define SFXLHARCS (sizeof(SfxLhArcs)/sizeof(SfxLhArcs[0]))

LhaArchive SfxLhArcs[] = {
      LhaArchive (0x653,  "-lh-"),
      LhaArchive (0x799,  "-lh-"),
      LhaArchive (0x665,  "-lh-"),
      LhaArchive (0x664,  "-lh-"),
      LhaArchive (0x59B1, "-lh-")   // LH for OS/2 2.22
};

/*
**  Definitions of self-extracting PAK archives
*/

#define SFXPAKS (sizeof(SfxPaks)/sizeof(SfxPaks[0]))

PakArchive SfxPaks[] = {
      PakArchive (0x261E),
      PakArchive (0x1AD3)
};


/*
**  Functions begin here
*/

#ifdef __cplusplus
   extern "C" {
#endif

int SeekFile (FILE *fp, char *header, long pos)
{
      memset (header, 0, BUFFSIZE);
      fseek (fp, pos, SEEK_SET);
      return fread (header, 1, BUFFSIZE - 1, fp);
} // SeekFile


int WhichArc (char *szFileName)
{
      char  header[BUFFSIZE];

      memset (header, 0, sizeof (header));
      FILE *fp = fopen (szFileName, "rb");
      if (fp == NULL)
            return ERROR;                    // error opening file
      int n = fread (header, 1, BUFFSIZE - 1, fp);

      if (n <= 0)                        // error reading from file
      {
            fclose (fp);
            return ERROR;
      }

      int i, rc;

      if (header[0]=='M' && header[1]=='Z')   // some sort of .EXE file
      {
            fseek (fp, 0, SEEK_END);
            long flen = ftell (fp);

            for (i = 0; i < SFXARCS; i++)
            {
                  rc = SfxArcs[i].Scan (fp, header, flen);
                  if (rc)
                  {
                        fclose (fp);
                        return rc;
                  }
            }

            for (i = 0; i < SFXLHARCS; i++)
            {
                  rc = SfxLhArcs[i].Scan (fp, header, flen);
                  if (rc)
                  {
                        fclose (fp);
                        return rc;
                  }
            }

            for (i = 0; i < SFXPAKS; i++)
            {
                  rc = SfxPaks[i].Scan (fp, header, flen);
                  if (rc)
                  {
                        fclose (fp);
                        return rc;
                  }
            }

            fclose (fp);
            return EXE;
      }

      // if file is not .EXE file, define just general types
      // for different archives. Offset parameter == 0 because
      // we do not scan files, just read header from the file start

      for (i = 0; i < ARCS; i++)
      {
            rc = Arcs[i].Check (header);
            if (rc)
            {
                  fclose (fp);
                  return rc;
            }
      }

      for (i = 0; i < LHARCS; i++)
      {
            rc = LhArcs[i].Check (header);
            if (rc)
            {
                  fclose (fp);
                  return rc;
            }
      }

      PakArchive Pak;

      rc = Pak.Check (header);

      fclose (fp);

      return rc;
}

#ifdef TEST

char *Arctypes[] = {
  "UNKNOWN"    ,
  "ARC"        ,
  "ARC6"       ,
  "PAK"        ,
  "ZOO"        ,
  "HPACK"      ,
  "RAR"        ,
  "UC2"        ,
  "SQZ"        ,
  "ARJ"        ,
  "LARC"       ,
  "LHARC"      ,
  "LHA"        ,
  "ZIP 1.x"    ,
  "ZIP 2.x"    ,
  "SFXARC"     ,
  "SFXARC6"    ,
  "SFXPAK"     ,
  "SFXARJ"     ,
  "SFXLHARC"   ,
  "SFXLHA"     ,
  "SFXZIP 1.x" ,
  "SFXZIP 2.x" ,
  "EXE"        ,
};

int main (int argc, char *argv[])
{
      int arc_type;

      if (argc != 2)
      {
            printf ("USAGE: WHICHARC arcfile\n");
            return 100;
      }

      arc_type = WhichArc (argv[1]);

      if (arc_type == -1)
            printf ("File %s not found!\n", argv[1]);
      else
      {
            printf ("%s:\tArchive type is %s\t(return value = %d)\n",
                    argv[1],
                    Arctypes[arc_type],
                    arc_type
                   );
      }

      return arc_type;
}

#endif

#ifdef __cplusplus
   }
#endif
