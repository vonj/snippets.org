/***************************************************************************
* @(#)repstr
* @(#)- Replace a pattern in a large amount of data using a copy function
*
***************************************************************************
*@(#)1993 Erik Bachmann
*
* Released to public domain 27-Oct-95
***************************************************************************/

/*
#define     TEST
*/
/*
#define     DEBUG
*/

#include    <stdio.h>
#include    <string.h>                          /* memset, strlen, memmove */
#include    "bacstd.h"                          /* filesize, strnsub       */

#if   defined DEBUG
#     define      BUFFERSIZE  80
#     define      BLOKSIZE          10
#else
#     define      BUFFERSIZE  2000
#     define      BLOKSIZE           250
#endif

#if   defined     TEST
#     define      MAXFIELDS   20
#endif

/*
 /-------------------------------------\
|        REPSTR                         |------------------------------------|
|\-------------------------------------/
|
| Replace a pattern in a large amount of data using a copy function
|
|
|----------------------------------------------------------------------------|
| CALL:
|    repstr( fpInput, fpOutput, pszCharTable ) ;

|
| HEADER:
|    stdio.h
|    string.h
|    bacstd.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    %
|
| PROTOTYPE:
|    int _CfnTYPE repstr(FILE *fpIn, FILE *fpOut, char *PatternTable[][2]) ;
|
| RETURN VALUE:
|    int           No of replacements
|
| MODULE:
|    repstr.c
|----------------------------------------------------------------------------|
|
|
|----------------------------------------------------------------------------|
|1993-06-19/Erik Bachmann
\---------------------------------------------------------------------------|*/

int _CfnTYPE repstr(FILE *fpIn, FILE *fpOut, char *PatternTable[][2])
{
      char  szBuffer[BUFFERSIZE+1] ;
      char  szTmpBuffer[BUFFERSIZE+1] ;

      long  lFileLength ;
      long  lFilePos = 0 ;

      int   i = 0 ;
      int   iStrLength ;

      /*----------------------------------------*/

      memset(szBuffer, '\0', BUFFERSIZE) ;

      iStrLength = fread(szBuffer, BLOKSIZE, 1, fpIn); /* Read first block */

      lFilePos = BLOKSIZE ;

      iStrLength = fread(&szBuffer[BLOKSIZE], BLOKSIZE, 1, fpIn) ;

      lFilePos += BLOKSIZE ;

      while( 0 != iStrLength )
      {     /* WHILE able to read */
            i = 0 ;

            lFilePos += BLOKSIZE ;

            while ( NULL != PatternTable[i][0][0] )
            {     /* For all patterns */
                  while (0 != strnsub(szBuffer, PatternTable[i][0],
                                      PatternTable[i][1], BUFFERSIZE))
                  {
                        ;     /* If found replace */
                  }
                  i++ ;
            }     /* Replace pattern with replacement */

            fwrite(&szBuffer, strlen(szBuffer) - BLOKSIZE, 1, fpOut) ;

            /* Write szBuffer execept last block */

            fflush(fpOut) ;

            memmove(&szBuffer, &szBuffer[strlen(szBuffer) - BLOKSIZE],
                    BLOKSIZE+1) ;

            memset(&szBuffer[BLOKSIZE], '\0', BLOKSIZE) ;

            iStrLength = fread(&szBuffer[BLOKSIZE], BLOKSIZE, 1, fpIn) ;

            szBuffer[BLOKSIZE + BLOKSIZE] = '\0' ;

      }


      i = 0 ;

      while (PatternTable[i][0][0] != NULL)
      {     /* For all patterns */
            while (0 != strnsub(szBuffer, PatternTable[i][0],
                                PatternTable[i][1], BUFFERSIZE))
            {
                  ;
            }
            i++ ;
      }

      /* Replace pattern with replacement */

      fwrite(&szBuffer, strlen(szBuffer), 1, fpOut) ;

      /* Write buffer */

      lFilePos += strlen(&szBuffer[BLOKSIZE]) ;

      return(0) ;
}


#if defined TEST

int main()
{
      char        *pszTegnTabel[MAXFIELDS][2] = {
                        {"XXX", "YYYY"},
                        {"ZZZ", "A"},
                        {"",""}
                        } ;

      FILE        *fp1,
                        *fp2 ;

      /*--------------------------------*/

      fp1 = fopen("test1", "rt") ;
      fp2 = fopen("test2", "wt") ;

      if ( (fp1 != NULL) && (fp2 != NULL) )

            repstr(fp1, fp2, pszTegnTabel) ;

      fclose(fp1) ;
      fclose(fp2) ;

      return(0) ;
}

#endif
