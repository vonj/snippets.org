/***************************************************************************
* @(#)strnsub
* @(#)-     searches string for a given pattern and replaces it with
* @(#)      a new text string.
*
***************************************************************************
*@(#)1993 Erik Bachmann
*
* Released to public domain 27-Oct-95
***************************************************************************/

#include "bacstd.h"
#include <stdio.h>
#include <string.h>

/*
 /-------------------------------------\
|        STRNSUB                        |------------------------------------|
|\-------------------------------------/
|    Function searches string for a given pattern and replaces it with
| a new text string.
|
| Note that the pattern string is replaced with the
| replacement string and they do not have to be the same length. It is
| assumed, however, that the original string is long enough to hold the
| result. Maximum length + 1 is given in iMaxLength
|
|
|----------------------------------------------------------------------------|
| CALL:
|    strnsub(pszStreng, org, rep, maxlength) ;
|
| HEADER:
|    bacstd.h
|    stdio.h
|    string.h
|    malloc.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    char *pszString    the string to search
|    char *pat          the pattern to locate
|    char *rep          the replacement string
|    int  iMaxLength    the maximum length of string after replacement
|
| PROTOTYPE:
|    char CfnTYPE *strnsub(char *pszString, char *pat, char *rep,
|                          int iMaxLength) ;
|
| RETURN VALUE:
|    char *         pointer to the replaced string
|
| MODULE:
|    strsub.c
|----------------------------------------------------------------------------|
|
|
|
|----------------------------------------------------------------------------|
|1993-03-25/Erik Bachmann
\---------------------------------------------------------------------------|*/

char _CfnTYPE *strnsub(char *pszString,
                       char *pszPattern,
                       char *pszReplacement,
                       int iMaxLength)
{
      char  *pszSubstring,
                  *pszTmpSubstring ;

      int   iPatternLength,
                  iReplacementLength ;

      /*-------------------------------------------------*/

      pszTmpSubstring = pszSubstring = pszString ;
      iPatternLength = strlen(pszPattern) ;
      iReplacementLength = strlen(pszReplacement) ;

      if ( 0 == strcmp( pszPattern, pszReplacement ) )
            return( 0 ) ;                 /* Pattern == replacement: loop */

      if ( NULL == ( pszSubstring = strstr(pszString, pszPattern ) ) )
      {                                   /* No match found               */
            return( NULL );
      }

      if ( ( strlen( pszString ) + ( iReplacementLength - iPatternLength ) )
             >= iMaxLength )

            /* Enough space for replacement? */

            return( NULL ) ;

      pszTmpSubstring = (char *) calloc(iMaxLength, sizeof(char)) ;

      /* Buy some workspace      */

      if ( NULL == pszTmpSubstring )            /* No memory left          */
            return( NULL ) ;

      strcpy( pszTmpSubstring, pszSubstring + iPatternLength ) ;

      /* If there was space left */

      while ( iReplacementLength-- )
      {     /* Copy replacement        */
            *pszSubstring++ = *pszReplacement++ ;
      }

      strcpy( pszSubstring, pszTmpSubstring ) ;

      /* Add old stuff back      */

      free( pszTmpSubstring ) ;

      return( pszSubstring - iPatternLength ) ;

      /* Return pointer to change*/
}

#ifdef      TEST

int main()
{
      char  szStreng[20] ;
      char  *org  = "xx" ;
      char  *rep  = "YYYY" ;

      /*-------------------------------------------------*/

      fprintf(stderr, "strnsub()\n\n") ;

      strcpy(szStreng, "testxxtest") ;

      fprintf(stderr, "Replacing %s with %s\n\nBefore : %s",
              org, rep, szStreng) ;

      if ( NULL == strnsub(szStreng, org, rep, 20) )
            fprintf(stderr, "\nERROR\n\a") ;
      else
            fprintf(stderr, "\nOK\n") ;

      fprintf(stderr, "After : %s\n\n", szStreng) ;

}

#endif
