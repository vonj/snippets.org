/***************************************************************************
* @(#)strtrimc.c
* @(#)strtrimcr
* @(#)      Removes all trailing occurences of specific characters in a string.
* @(#)strtrimcl
* @(#)      Removes all leading occurences of specific characters in a string.
* @(#)strtrimc
* @(#)      Removes all leading and traling occurences of specific characters
* @(#)      in a string.
*
***************************************************************************
*@(#)1993 Erik Bachmann
*
* Released to public domain 27-Oct-95
***************************************************************************/

#include <string.h>
#include "bacstd.h"

/*
 /-------------------------------------\
|  STRTRIMCR                            |------------------------------------|
|\-------------------------------------/
|
| Removes all trailing occurences of specific characters in a string.
|
|
|----------------------------------------------------------------------------|
| CALL:
|    strtrimcr(&str, ":;\\");
|
| HEADER:
|    ctype.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|    pszSet      : String with the characters to remove
|
|
| PROTOTYPE:
|    int _CfnTYPE strtrimcr(char *szStr, char *szSet);
|
| RETURN VALUE:
|    j-i         : No of removed characters
|
| MODULE:
|    strtrim.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE strtrimcr(char *szStr, char *szSet)
{
      int   i, j;                                     /* Locale counters */

      /*-------------------------------------------------*/

      j = i = strlen(szStr) - 1;                /* Find length of string */

      while (strrchr(szSet, szStr[ i ])
                  && (0 <= i))
      {
            /* While string is terminated by one of the specified characters */
            szStr[ i-- ] = '\0';          /*- Replace character with '\0' */
      }

      return(j - i);    /* Return the difference between old and new length */
}

/*
 /-------------------------------------\
|  STRTRIMCL                            |------------------------------------|
|\-------------------------------------/
|
| Removes all leading occurences of a specific character in a string.
|
|----------------------------------------------------------------------------|
| CALL:
|    strtrimcl(&str, ";:\\");
|
| HEADER:
|    ctype.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|    pszSet      : String with the characters to remove
|
| PROTOTYPE:
|    int _CfnTYPE strtrimcl(char *szStr, char *szSet);
|
| RETURN VALUE:
|    i           : No of removed characters
|
| MODULE:
|    strtrim.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE strtrimcl(char *szStr, char *szSet)
{
      int   i = 0, j;

      /*-------------------------------------------------*/

      j = strlen(szStr) - 1;                    /* Find length of string */

      while (strrchr(szSet, szStr[ i ])
                  && (i <= j))
      {
            /* While first character in string matches tag */

            i++;                    /*- Count no of removed chars */
      }

      if (0 < i)                    /* IF there were matches */
            strcpy(szStr, &szStr[ i ]);         /*- shift string to the left */

      return(i);                    /* Return no of matching chars */
}

/*
 /-------------------------------------\
|  STRTRIMC                             |------------------------------------|
|\-------------------------------------/
|
| Removes all leading and trailing occurences of a specific character in
|  a string.
|
|----------------------------------------------------------------------------|
| CALL:
|    strtrimc(&str, ";:\\");
|
| HEADER:
|    ctype.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|    pszSet      : String with the characters to remove
|
| PROTOTYPE:
|    int _CfnTYPE strtrimc(char *szStr, char *szSet);
|
| RETURN VALUE:
|    iStatusFlag : No of removed characters
|
| MODULE:
|    strtrimc.c
|----------------------------------------------------------------------------|
|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE strtrimc(char *szStr, char *szSet)
{
      int   iStatusFlag;

      /*-------------------------------------------------*/

      iStatusFlag =  strtrimcl(szStr, szSet);
      iStatusFlag += strtrimcr(szStr, szSet);

      return(iStatusFlag);
}

/*
 /-------------------------------------\
|  REP_LAST_CHAR                        |------------------------------------|
|\-------------------------------------/
|
| Replaces the last char on match with another specified char.
|
|
|----------------------------------------------------------------------------|
| CALL:
|    rep_last_char(str, '\n', '\0');
|
| HEADER:
|    string.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|    cChar1      : Character to replace
|    cChar2      : Character to replace with
|
|
| PROTOTYPE:
|    int _CfnTYPE rep_last_char(char *szStr, char cChar1, char cChar2);
|
| RETURN VALUE:
|    int         : Stringlength
|
| MODULE:
|    strtrim.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE rep_last_char(char *pszStr, char cChar1, char cChar2)
{
      int   i;

      /*--------------------------------*/

      i = strlen(pszStr) - 1;

      if (pszStr[ i ] == cChar1)
            pszStr[ i ] = cChar2;

      return(i);
}     /*** rep_last_char() ***/

#ifdef TEST

main()
{
      char  *strl = "xxyzxxxLeading x",
                  *strr = "x Traling xxyzxxx",
                  *str  = "xxzyxLead-&trailingxzxyx";
      /*--------------------------------*/

      printf("\nBefore convertion:\n\t\"%s\"\n\t\"%s\"\n\t\"%s\"\n",
                  strl, strr, str);

      strtrimcr(strr, "xyz");
      strtrimcl(strl, "xyz");
      strtrimc( str, "xyz");

      printf("\nAfter convertion:\n\t\"%s\"\n\t\"%s\"\n\t\"%s\"\n",
                  strl, strr, str);

      return(0);
}

#endif /* TEST */
