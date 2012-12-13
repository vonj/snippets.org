/*
** This file is placed into the public domain on February 22, 1996,  by
** its author: Carey Bloodworth
**
** Modifications:
**
** 07-Dec-1999 by
** Bob Stout & Jon Guthrie
**  General cleanup, use NUL (in SNIPTYPE.H) instead of NULL where appropriate.
**  Allow spaces in tag names.
**  Allow strings in quotes.
**  Allow trailing comments.
**  Allow embedded comment separator(s) in quoted strings.
**  Speed up line processing.
**  ReadCfg() returns an error if section or variable not found.
**  Changed integer type to short.
**  Use cant() calls in lieu of fopen() calls,
**    include ERRORS.H for prototype.
*/

#ifndef INI_H_
#define INI_H_

/*lint -e537 */
#include <limits.h>
#include "sniptype.h"

#define INI_LINESIZE 128

enum CfgTypes {Cfg_String,
               Cfg_Byte,
               Cfg_Ushort,
               Cfg_Short,
               Cfg_Ulong,
               Cfg_Long,
               Cfg_Double,
               Cfg_Boolean, /* Boolean is actually an integer Y/N T/F */
               Cfg_I_Array
              };

struct CfgStruct {
      char *Name;
      void *DataPtr;
      enum CfgTypes VarType;
};

int ReadCfg(const char *FileName,
            char *SectionName,
            struct CfgStruct *MyVars);

int SearchCfg(const char *FileName,
              char *SectionName,
              char *VarName,
              void *DataPtr,
              enum CfgTypes VarType);

int UpdateCfg(const char *FileName,
              char *SectionName,
              char *VarWanted,
              char *NewData);

#endif
