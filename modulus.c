/****************************************************************************
* @(#)MODULUS.c
* @(#)- Basic routines for creating and checking modulus values
*
*
*****************************************************************************
* CONTENTS:
*
*   modulus10() ;        Returns the modulus 10 check digit.
*   modulus11() ;        Returns the modulus 11 check digit
*   check_modulus10() ;  Checks if the check digit in string is correct
*   check_modulus11() ;  Checks if the check digit in string is correct
*
*****************************************************************************
* DESCRIPTION:
*
* Calculations of modulus 10 check digits:
*
*   Multiply units position an every       Base 6 1 2 4 8
*   alternate position i basic code             6   2   8
*   number by 2, starting from righthand
*   digit                                                 = 1256
*
*   Bring down digits that were not               1   4
*   multiplied by 2
*
*   Cross add these digits to digits in
*   first product                             1+2+5+6+1+4 = 19
*
*   Enter next higher number with a ending
*   zero. If the number already ends with
*   zero keep it.                                           20
*
*   Subtract the cross add sum with next
*   higher and you've got the check digit         20 - 19 = 1
*                                                           Í
*   Result : 61248 1
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Calculations of modulus 11 check digits:
*
*   Assigned weighted checking factor    base       9 4 3 4 5 7 8 4 2
*   to each digits position of basic     weight     4 3 2 7 6 5 4 3 2
*   code number starting with units
*   position of number and progressing
*   to most significant digit
*
*   Weight used is:
*   765432765432765432765432765432765432 etc.
*
*   Multiply esch digit of the basic               36
*   code number by its weighted checking             12
*   factor, and add the products                        6
*                                                        28
*                                                          30
*                                                            35
*                                                              32
*                                                                12
*                                                                   4
*
*
*                                                            Sum = 195
*
*   Divide Sum of products by 11                  195 % 11 = 17 plus
remainder of 8
*
*   Subtract remainder from 11                    11 - 8 = 3
*   difference is check digit                              Í
*
*   Result :  943457842 3
*
*****************************************************************************
*@(#)1993 Erik Bachmann (E-mail: ebp@dde.dk)
*
* Released to public domain 27-Oct-95
*****************************************************************************/
#include <math.h>                      /* pow() */
#include <string.h>                    /* strlen(), strncpy() */
#include "bacstd.h"


/*
 /-------------------------------------\
|      MODULUS10                        |------------------------------------|
|\-------------------------------------/                                    
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    iMod10 = modulus10("61248") ;                                            
|                                                                            
| HEADER:                                                                    
|    math.h        pow()                                                     
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    char *pszBase                                                           
|                  The string on which the modulus is performed              
|                                                                            
|                                                                            
| PROTOTYPE:                                                                 
|    int _CfnTYPE modulus10(char *pszBase) ;                                  
|                                                                            
| RETURN VALUE:                                                              
|    int           Modulus value                                             
|                                                                            
| MODULE:                                                                    
|    modulus.c                                                               
|----------------------------------------------------------------------------|
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
|1993-09-29/Erik Bachmann                                                    
\---------------------------------------------------------------------------|*/

int _CfnTYPE modulus10(char *pszBase)
{
   int   iSum  = 0,
         iTemp = 0,
         i,
         j ;

   /*--------------------------------*/

                                       /* Get units position an every
                                          alternate position i basic code,
                                          starting from righthand and
                                          cross add */
   for (i = strlen(pszBase) - 1, j = 0 ; i >= 0 ; i -= 2, j++ )
   {
      iSum += (pszBase[i] - '0') * (int)pow(10, j) ;
   }

   iSum *= 2 ;                         /* Multiply by 2 */

   iTemp = iSum ;
                                       /* Bring down digits that were not
                                          multiplied by 2 */
   for ( iSum = 0 ; j >= 0 ; j--)
   {
      iSum  += iTemp % 10 ;               /* Cross add these digits to digits in
                                          first product */
      iTemp /= 10 ;
   }
                                       /* Enter next higher number with a
                                          ending zero. If the number already
                                          ends with zero keep it. */

   for (i = strlen(pszBase) - 2 ; i >= 0 ; i -= 2 )
   {
      iSum += (pszBase[i] - '0') ;
   }

                                       /* Subtract the cross add sum with
                                          next higher and you've got the
                                          check digit*/
   if ( (iSum % 10) != 0)
   {
      iSum = ( ( (iSum / 10) + 1 ) * 10) - iSum ;
   }
   else
      iSum = 0 ;

   return( iSum ) ;
}  /*** modulus10() ***/


/*
 /-------------------------------------\
|      MODULUS11                        |------------------------------------|
|\-------------------------------------/                                    
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    iMod11 = modulus11("943457842") ;                                        
|                                                                            
| HEADER:                                                                    
|    math.h        pow()                                                     
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    char *pszBase                                                           
|                  The string on which the modulus is performed    
|                                                                            
| PROTOTYPE:                                                                 
|    int _CfnTYPE modulus11(char *pszBase) ;                                  
|                                                                            
| RETURN VALUE:                                                              
|    int           Modulus value                                             
|                                                                            
| MODULE:                                                                    
|    modulus.c                                                               
|----------------------------------------------------------------------------|
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
|1993-09-29/Erik Bachmann                                                    
\---------------------------------------------------------------------------|*/

int _CfnTYPE modulus11(char *pszBase)
{
   char *pszWeids = "765432" ;
   int   iSum = 0 ;
   int   i,
         j ;

   /*--------------------------*/

                                       /* Assigned weighted checking factor
                                          to each digits position of basic
                                          code number starting with units
                                          position of number and progressing
                                          to most significant digit */

   for ( i = strlen(pszBase) - 1, j = 5 ; i >= 0 ; i-- )
   {
                                       /* Multiply esch digit of the basic
                                          code number by its weighted checking
                                          factor, and add the products */
      iSum += (pszBase[i] - '0') * (pszWeids[j--] - '0') ;
      if (j < 0)
         j = 5 ;
   }

                                       /* Divide Sum of products by 11
                                          Subtract remainder from 11
                                          difference is check digit */
   iSum = 11 - (iSum % 11) ;

   return( iSum ) ;
}  /*** modulus11() ***/


/*--------------------------------------------------------------*/

/*
 /-------------------------------------\
|      CHECK_MODULUS10                  |------------------------------------|
|\-------------------------------------/                                    
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    iMod10 = check_modulus10("612481") ;                                     
|                                                                            
| HEADER:                                                                    
|    math.h        pow()                                                     
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    char *pszBase                                                           
|                  The string including the digit to be checked              
|                                                                            
|                                                                            
| PROTOTYPE:                                                                 
|    int _CfnTYPE check_modulus10(char *pszBase) ;                            
|                                                                            
| RETURN VALUE:                                                              
|    int           0     = OK                                                
|                  else    value - 10 = correct digit                        
| MODULE:                                                                    
|    modulus.c                                                               
|----------------------------------------------------------------------------|
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
|1993-09-29/Erik Bachmann                                                    
\---------------------------------------------------------------------------|*/

int _CfnTYPE check_modulus10(char *pszBase)
{
   char  szTmp[11] ;
   int   iTmp = 0 ;

   /*--------------------*/

   strncpy(szTmp, pszBase, strlen(pszBase) - 1) ;
   szTmp[strlen(pszBase) - 1] = '\0' ;
                                       /* Extract base */
   iTmp = pszBase[strlen(pszBase) - 1] - '0' ;
                                       /* Extract current check digit */
   if ( modulus10(szTmp) == iTmp )
      return( 0 ) ;                    /* Check digit is OK */
   else
      return( 10 + modulus10(szTmp) ) ;/* On error return the correct + 10 */
}  /*** check_modulus10() ***/

/*--------------------------------------------------------------*/

/*
 /-------------------------------------\
|      CHECK_MODULUS11                  |------------------------------------|
|\-------------------------------------/                                    
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    iMod11 = check_modulus11("9434578423") ;                                 
|                                                                            
| HEADER:                                                                    
|    math.h        pow()                                                     
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    char *pszBase                                                           
|                  The string including the digit to be checked              
|                                                                            
|                                                                            
| PROTOTYPE:                                                                 
|    int _CfnTYPE check_modulus10(char *pszBase) ;                            
|                                                                            
| RETURN VALUE:                                                              
|    int           0     = OK                                                
|                  else    value - 11 = correct digit                        
| MODULE:                                                                    
|    modulus.c                                                               
|----------------------------------------------------------------------------|
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
|1993-09-29/Erik Bachmann                                                    
\---------------------------------------------------------------------------|*/

int _CfnTYPE check_modulus11(char *pszBase)
{
   char  szTmp[11] ;
   int   iTmp = 0 ;

   /*--------------------*/

   strncpy(szTmp, pszBase, strlen(pszBase) - 1) ;
   szTmp[strlen(pszBase) - 1] = '\0' ;
                                       /* Extract base */
   iTmp = pszBase[strlen(pszBase) - 1] - '0' ;
                                       /* Extract current check digit */

   if ( modulus11(szTmp) == iTmp )
      return( 0 ) ;                    /* Check digit is OK */
   else
      return( 11 + modulus11(szTmp) ) ;
                                       /* On error return the correct + 11 */
}  /*** check_modulus11() ***/


/******************************************************/

#ifdef   TEST

int main(void)
{
   int iStatus ;

   /*------------------------------*/

   iStatus = modulus11("943457842") ;
   iStatus = check_modulus11("9434578423") ;
   iStatus = modulus10("61248") ;
   iStatus = check_modulus10("612481") ;

   return( iStatus ) ;
}
#endif
