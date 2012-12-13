/***************************************************************************
* @(#)strtrim.c
* @(#)strtrimr - Removes all trailing blanks from a string.
* @(#)strtriml - Removes all leading blanks from a string.
* @(#)strtrim  - Removes all leading and trailing blanks in a string.
*
***************************************************************************
*@(#)1993 Erik Bachmann
*
* Released to public domain 27-Oct-95
***************************************************************************/

#include <string.h>
#include <ctype.h>
#include "bacstd.h"

/*
 /-------------------------------------\
|  STRTRIMR                             |------------------------------------|
|\-------------------------------------/
|
| Removes all trailing blanks from a string.
| Blanks are defined with ISSPACE  (blank, tab, newline, return, formfeed,
| vertical tab = 0x09 - 0x0D + 0x20)
|
|----------------------------------------------------------------------------|
| CALL:
|    strtrimr(&str);
|
| HEADER:
|    ctype.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|
| PROTOTYPE:
|    int _CfnTYPE strtrimr(char *pszStr);
|
| RETURN VALUE:
|    j-i         : No of removed blanks
|
| MODULE:
|    strtrim.c
|----------------------------------------------------------------------------|
|1994-01-08/Bac
|   All characters is checked (">" -> ">=").
|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/
int _CfnTYPE strtrimr(char *pszStr)
{
      int   i, j;                               /* Local counters */

      /*-------------------------------------------------*/

      j = i = strlen(pszStr) - 1; /* Calculate the length of the string */

      while (isspace(pszStr[i]) && (i >= 0))

            /* WHILE string ends with a blank */
            /*1994-01-08/Bac Even if all chars are blanks (= 0) */

            pszStr[ i-- ] = '\0';               /*- Replace blank with '\0' */

      return(j - i);                            /* Return no of replacements */
}

/*
 /-------------------------------------\
|  STRTRIML                             |------------------------------------|
|\-------------------------------------/
|
| Removes all leading blanks from a string.
| Blanks are defined with ISSPACE  (blank, tab, newline, return, formfeed,
| vertical tab = 0x09 - 0x0D + 0x20)
|
|
|----------------------------------------------------------------------------|
| CALL:
|    strtriml(&str);
|
| HEADER:
|    ctype.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|
| PROTOTYPE:
|    int _CfnTYPE strtriml(char *pszStr);
|
| RETURN VALUE:
|    i           : No of removed blanks
|
| MODULE:
|    strtrim.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE strtriml(char *pszStr)
{
      int   i = 0, j;                                 /* Local counters */

      /*-------------------------------------------------*/

      j = strlen(pszStr) - 1; /* Calculate the length of the string */

      while (isspace(pszStr[i]) && (i <= j))

            /* WHILE string starts with a blank */

            i++;                          /*- Count no of leading blanks */

      if (0 < i)                          /* IF leading blanks are found */
            strcpy(pszStr, &pszStr[i]);   /*- Shift string to the left */

      return(i);                          /* Return no of replacements */
}

/*
 /-------------------------------------\
|  STRTRIM                              |------------------------------------|
|\-------------------------------------/
| Removes all leading and trailing blanks in a string.
| Blanks are defined with ISSPACE  (blank, tab, newline, return, formfeed,
| vertical tab = 0x09 - 0x0D + 0x20)
|
|
|----------------------------------------------------------------------------|
| CALL:
|    strtrim(&str);
|
| HEADER:
|    ctype.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|
| PROTOTYPE:
|    int _CfnTYPE strtrim(char *pszStr);
|
| RETURN VALUE:
|    iBlank      : No of removed blanks
|
| MODULE:
|    strtrim.c
|----------------------------------------------------------------------------|
|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE strtrim(char *pszStr)
{
      int   iBlank;

      /*-------------------------------------------------*/

      iBlank  = strtrimr(pszStr);               /* Remove trailing blanks */
      iBlank += strtriml(pszStr);               /* Remove leading blanks */

      return(iBlank);
}


#ifdef      TEST

main()
{
      char  *strl = "     Leading blanks",
                  *strr = "Traling blanks     ",
                  *str  = "   Lead-&trailing  ";
      /*--------------------------------*/

      printf("\nBefore convertion:\n\t\"%s\"\n\t\"%s\"\n\t\"%s\"\n",
                  strl, strr, str);

      strtrimr(strr);
      strtriml(strl);
      strtrim(str);

      printf("\nAfter convertion:\n\t\"%s\"\n\t\"%s\"\n\t\"%s\"\n",
                  strl, strr, str);

      return(0);
}

#endif
