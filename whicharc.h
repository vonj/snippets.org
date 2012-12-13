/*
**  WHICHARC.H
**
**   Written by Jari Laaksonen (2:221/360.20), 2 Sep 1994
**   Based on the code by Heinz Ozwirk and David Gersic.
**
**   Free for all participants of the C_ECHO & other conferences where
**   this code is posted. Released to SNIPPETS by the authors.
**
**   See the file WHICHARC.DOC about archive types and file offsets
**   for self extracting archives.
*/

#ifdef __cplusplus
   extern "C" {
#endif

int WhichArc (char *pName);
int SeekFile (FILE *fp, char *header, long pos);

#ifdef __cplusplus
   }
#endif
