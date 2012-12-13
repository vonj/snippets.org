/****************************************************************************
*@(#) date_mac_conv() - Converting a date string with the format from
*@(#)    macro __DATE__ to ISO standard.
*
****************************************************************************
*@(#)1992 Erik Bachmann  (E-mail: ebp@dde.dk)
*
* Released to public domain 27-Oct-95
****************************************************************************/

#include <stdio.h>                     /* scanf, sprintf */
#include <string.h>                    /* strcpy, strcmp */
#include "bacstd.h"                    /* _CfnTYPE */

/*
 /-------------------------------------\
|     DATE_MAC_CONV                     |------------------------------------|
|\-------------------------------------/                                    
| Converting a date string with the format from __DATE__ to ISO standard     
|                                                                            
|                                                                            
|                                                                            
| Example:   "Sep 16 1992" -> "1992-09-16"                                   
|                                                                            
|----------------------------------------------------------------------------|
| CALL:                                                                      
|    strcpy(string, date_mac_conv(string) ) ;                                 
|                                                                            
| HEADER:                                                                    
|    stdio.h     : scanf, sprint                                             
|    string.h    : strcpy, strcmp                                            
|                                                                            
| GLOBALE VARIABLES:                                                         
|    %                                                                       
|                                                                            
| ARGUMENTS:                                                                 
|    pszDate     : String containing __DATE__ format.                        
|                                                                            
| PROTOTYPE:                                                                 
|    char _CfnTYPE *date_mac_conv(char *pszDate) ;                            
|                                                                            
| RETURN VALUE:                                                              
|    char        : ISO date format YYYY-MM-DD (length 8 char)                
|                                                                            
| MODULE:                                                                    
|    date__.c                                                                
|----------------------------------------------------------------------------|
|                                                                            
|                                                                            
|                                                                            
|                                                                            
|----------------------------------------------------------------------------|
|1992-09-16/Erik Bachmann                                                   
\---------------------------------------------------------------------------|*/

char _CfnTYPE *date_mac_conv(char *pszDate)
{
   char  *paMonth[12] = {"Jan", "Feb", "Mar", "Apr","May", "Jun", "Jul", "Aug",
                        "Sep", "Oct", "Nov", "Dec"} ;
                                       /* Names of months */

   char  szString[12] ;                /* String fore convertion */
   char  cMonth[4],                    /* Month */
         cDay[3],                      /* Day */
         cYear[5] ;                    /* Year */
   int   iCounter  ;                   /* Local counter */

   /*-----------------------------------------------------------*/
   
   strcpy(szString, pszDate) ;            /* Get string to convert */
   
   sscanf(szString, "%s %s %s", cMonth, cDay, cYear) ;
                                       /* Split string */
   
   for ( iCounter = 11  ; iCounter >= 0 ; iCounter--)
                                       /* loop for testing month */
   {
      if (strcmp(cMonth, paMonth[iCounter]) == 0)
                                       /* - IF month is found */
      {
         sprintf(cMonth, "%0.2i", iCounter + 1 ) ;
                                       /* - - Insert counter + 1 as month */
         break ;
      }
   }

   /* Check valid day */
   iCounter = atoi( cDay ) ;
   if ( ( 1 > iCounter ) || ( 31 < iCounter ) )
      strcpy( cDay, "01" ) ;

   sprintf(szString, "%04.4s-%02.2s-%02.2s", cYear,cMonth,cDay) ;
                                       /* Merge ISO date */

   return( (char _CfnTYPE *) szString ) ;
}  /*** date_mac_conv() ***/

#ifdef TEST
main()
{
   char  szString[20] ;

   /*--------------------------------*/

   printf( "\n%s\n", date_mac_conv( __DATE__ ) ) ;

   return( 0 ) ;
}
#endif
