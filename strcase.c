/***************************************************************************
* @(#)strcase
* @(#)	Converts a string to upper/lowercase using non ASCII convertion
*
***************************************************************************
*@(#)1993 Erik Bachmann
*
* Released to public domain 27-Oct-95
***************************************************************************/
#include <string.h>							/* strlen */
#include "bacstd.h"

/*
 /-------------------------------------\
|          STRCASE                      |------------------------------------|
|\-------------------------------------/                                    
|                                                                            
| Converts a string to upper/lowercase using non ASCII convertion            
|                                                                            
| Examples of sort sequences found in SORTKEY.h                              
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    pszStr = strcase("abc‘›†", (unsigned char *) CaseUpper) ;                
|                                                                            
| HEADER:                                                                    
|    sortkey.h                                                               
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    char *pszStr       Pointer to string                                    
|    char *pszOrder     Pointer to table with sort sequences                 
|                                                                            
| PROTOTYPE:                                                                 
|    int _CfmTYPE strcase(unsigned char *pszStr, unsigned char *pszOrder) ;   
|                                                                            
| RETURN VALUE:                                                              
|    char *        Pointer to string                                         
|                                                                            
| MODULE:                                                                    
|    STRCASE.c                                                               
|----------------------------------------------------------------------------|
|                                                                            
|----------------------------------------------------------------------------|
|1993-04-10/Erik Bachmann                                                    
\---------------------------------------------------------------------------|*/

unsigned char _CfnTYPE *strcase(unsigned char *pszStr, unsigned char *pszOrder)
{
	long 		lCount	=	0L ;

	/*---------------------------------------------------------*/

	for ( lCount = 0L ; '\0' != pszStr[lCount] ; lCount++ )
	{
		pszStr[lCount] = pszOrder[ pszStr[lCount] ] ;
	}

	return( pszStr ) ;
}	/*** strcase() ***/

#ifdef	TEST

#include "sortkey.h"

int	main()
{
	int i;
	char				*str	= "abcd‘›†„”„”‚";

	/*---------------------------------------*/

	fprintf(stderr, "strcase()\n\n");

	printf("\nOriginal string : %s", str);

	strcase(str, (unsigned char *) CaseUpper);
	printf("\nUppercase string: %s", str);

	strcase(str, (unsigned char *) CaseLower);
	printf("\nLowercase string: %s", str);

	return( 0 ) ;
}	/*** main() ***/

#endif
