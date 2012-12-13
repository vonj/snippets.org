/***************************************************************************
* @(#)strrepc
* @(#)      Replaces all occurrences of a character in a string with another
* @(#)      character
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
|       STRREPC                         |------------------------------------|
|\-------------------------------------/
|
| Replaces all occurrences of a character in a string with another character
|
|----------------------------------------------------------------------------|
| CALL:
|    strrepc( str, cFrom, cTo ) ;
|
| HEADER:
|    string.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    pszStr      : String to be converted
|    cFrom       : Char to be replaced
|    cTo         : Replacement
|
| PROTOTYPE:
|    int _CfnTYPE strrepc( char *pszStr, char cFrom, char cTo ) ;
|
| RETURN VALUE:
|    iReturn     : No of replacements
|
| MODULE:
|    strrepc.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1992-11-09/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE strrepc(char *pszStr, char cFrom, char cTo)
{
      char  *ptr ;                              /* Pointer to string */
      int   iReturn = 0 ;                       /* No of replacements */

      /*----------------------------------------------------------------*/

      while( 0 != ( ptr = strchr( pszStr, cFrom ) ) )

      {     /* WHILE cFrom occurs in pszStr */

            pszStr[ (int) ptr - (int) pszStr ] = cTo ;

            /*- Replace next cFrom with cTo */

            iReturn++ ;                                           /*- count */
      }

      return( iReturn ) ;
}

#ifdef      TEST

int main()
{
      char streng[20];

      /*-----------------------------------------*/

      strcpy(streng, "abcabcabc");

      fprintf(stderr, "strrepc()\n\n");

      fprintf(stderr, "Replacing c with C\n\nBefore : %s\n\n", streng);

      strrepc(streng, 'c', 'C');

      fprintf(stderr, "After  : %s\n\n", streng);

      return( 0 ) ;
}

#endif
