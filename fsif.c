#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "bacstd.h"

MODULEINF("1992-08-01", "1990-1992 Erik Bachmann (E-mail: ebp@dde.dk" ) ;

#define  BUFFERSIZE  512

/*
 /=======================================\
|     FIND_STR_IN_FILE                    |-----------------------------------|
|\=======================================/
|
| Searches a binary file for a string.
|
| Returns the start offset for the first occurrence of the string.
|
|
|
|
|-----------------------------------------------------------------------------|
| CALL:
|    find_str_in_file(Filename, startoffset, string);
|
| HEADERS:
|    stdio.h
|    string.h
|    malloc.h
|    io.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    fpFile      : Pointer to the (open) binary file
|    lOffset     : Startoffset for search
|    pszStr      : String to search for
|
| PROTOTYPE:
|    long _CfnTYPE find_str_in_file(FILE *fpFile, long lOffset,
|                                   char *pszStr);
|
| RETURN VALUE:
|    long lStatusFlag   : -1   : String not found
|                         -2   : Not enough memory to perform search
|                         else : Offset for string in file
|
| MODULE:
|    fsif.c
|-----------------------------------------------------------------------------|
|1994-03-18/Bac
|   -  Enhanced error detection. Returning error value
|1995-06-19/Bac
|   -  Patch for returning position. Changed from blockwise to bytewise
|      calculation.
|1995-09-20/Bac
|  -  Ajusted secondary search in last block
|
|1995-10-27/Bac
|  -  Released to public domain
|
|-----------------------------------------------------------------------------|
|1992-08-01/Erik Bachmann
\===========================================================================|*/

long _CfnTYPE find_str_in_file(FILE *fpFile, long lOffset, char *pszStr)
{
      long  lStatusFlag = -1l,            /* Status: -1 = not found     */
            lPosInFile  = 0l,             /* Position in file           */
            lCurrentPos = 0l;

      char  cFound      = FALSE,          /* Flag: FALSE = Not found (yet) */
            *pszBuffer  = NULL;           /* Buffer for fileinput       */

      int   iDataSize   = 0,              /* Size of data read from file*/
            iSector     = 0,              /* No of blocks read          */
            offset      = 0,              /* local counter              */
            iBufferSize = BUFFERSIZE,
            iNoBlocks   = 0;              /* No of blocks remaining     */

      /*----------------------------------------------------------------*/

      lPosInFile = filelength(fileno(fpFile));  /* Find filesize        */

      iNoBlocks = (int)(lPosInFile / (int) iBufferSize) ;
                                    /* Calculate remaining no of blocks */

      fseek(fpFile, lOffset, SEEK_SET) ;  /* Go to start offset */

      lCurrentPos = lOffset ;

      iNoBlocks = (int)((lPosInFile - lOffset) / (int) iBufferSize);
                                    /* Calculate remaining no of blocks */

      if (0 == (pszBuffer = (char *) calloc(2 * iBufferSize, sizeof(char))))
            return -2;              /* Allocate buffer                  */

      memset(pszBuffer, '\0', iBufferSize);

      iDataSize = fread(pszBuffer, sizeof(char), iBufferSize, fpFile);
                                    /* Read the first block             */

      while ((0 < iNoBlocks) && !cFound)
      {                             /* Repeat until EOF or found        */
            iSector++;              /* Counting no of blocks read       */
            iNoBlocks--;

            memset(&pszBuffer[iBufferSize], '\0', iBufferSize);
            iDataSize = fread(&pszBuffer[iBufferSize], sizeof(char),
                              iBufferSize, fpFile); /* Read next block  */

            for (offset = 0; offset < iBufferSize; offset++)
                                    /* Search first block               */
            {
                  if (0 == strncmp(&pszBuffer[ offset ],
                                   pszStr, strlen(pszStr)))
                  {                 /* Is the string placed here?       */
                        cFound = TRUE;    /* Yes -> set flag            */
                        break;
                  }
                  else  lCurrentPos++;    /* No -> Try again            */
            }

            memcpy(pszBuffer, &pszBuffer[ iBufferSize ], iBufferSize);
                                    /* Shift block left                 */
      }

      /* Search the last Sector read if tag not found yet               */

      if (!cFound)
      {
            iSector++;              /* Counting no of blocks read       */

            for (offset = 0; offset < iDataSize - strlen(pszStr); offset++)
                  if (0 == strncmp(&pszBuffer[ offset ], pszStr,
                                   strlen(pszStr)))
                  {                 /* Is the string placed in last block?*/
                        cFound = TRUE;    /* Found -> set flag          */
                        break;
                  }
                  else if (lCurrentPos < lPosInFile)
                                    /* Check for End of File            */
                        lCurrentPos++;    /* In file -> goto next       */
                  else  break;            /* End of File = quit         */
      }

      free(pszBuffer);              /* Free the allocated memory again  */

      if (TRUE == cFound)                 /* IF tag is found            */
            return ((long) lCurrentPos);  /* Return bytewise positon    */
      else  return lStatusFlag;           /* Return errorcode           */
}
