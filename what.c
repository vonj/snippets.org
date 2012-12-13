/****************************************************************************
*     WHAT.c
*  @(#)
*     @(#) Scans binary or text files for a token and prints the line following
*     @(#) the token on stdout (normally the screen).
*     @(#) The token is by default @(#) like in the UNIX version of WHAT.
*  @(#)
*
*
*
*** CORRECTIONS *************************************************************
*1995-07-30/Bac:
*-    FindStringInFile() included explicit in project and recompiled.
*-    DEBUGTRACE inserted and iDebugFlag added
*1995-09-20/Bac:  v. 0.03
*-    fsif() ajusted of searching in last block of a file.
*1995-10-26/Bac: v. 0.04
*-    Rewritten for public use
*
*** BUGS & FLAWS **********************************************************
*
*
*****************************************************************************
*@(#)1995 Erik Bachmann                                                      *
*
* Released to public domain 27-Oct-95
*****************************************************************************/
#include    "bacstd.h"                                      /* */
#include    <string.h>                                      /* strlen(),  */
#include    <stdio.h>                                       /* FILE */
#include    "dirport.h"


PROGRAMINF("WHAT", "0.04", "1995-06-18", "(p)1995 Erik Bachmann");

/*** MACROS *****************************************************************/
#define     MAXPATH     80
#define     MAXLINE     256

/* TRACE for debugging is NOT implemented in this version */
#define     TRACEFILE               NULL
#define     ON_DEBUG                NULL;
#define     TRACE                   _0
#define     DEBUGTRACE              _0

char  *pszErrMsg  = NULL;



/*** PROTOTYPES ************************************************************/
int main(int argc, char *argv[]);


/*** GLOBAL VARABLES *******************************************************/

static char szInputFile[MAXPATH];
static char szToken[MAXLINE] = "\x40(#)"; /* Token to identify comments */


const char  *HELPFORMAT =
{
      "Scans binary or text files for a token and prints the line following\n"
      "the token on stdout (normally the screen).\n"
      "The token is by default \x40(#) like in the UNIX version of WHAT.\n\n"
      "CALL: %Fs <filename> {/Ttoken}\n\n"
      "\tfilename\tfilename or wildcard\n"
      "\t/T\tToken if standard \x40(#) is not used.\n\n"
};

/*** FUNCTIONS *************************************************************/
/* Dummy function */

int _0(char *fmt, ...)
{     }     /*** _0 ***/

/*
 /=======================================\
|      SEARCH_FILE
|-------------------------------------|
|\=======================================/
|
|     Scanning through a binary file for a token. Prints the following line on
|  stdout.
|
|
|-----------------------------------------------------------------------------|
| CALL:
|    search_file(ffblk.ff_name);
|
| HEADER:
|    %
|
| GLOBALE VARIABLES:
|    szToken                  The Token to search for
|
| ARGUMENTS:
|    *szInputFile       Name of a file to scan for template strings
|
|
| PROTOTYPE:
|    int  search_file(char *szInputFile)
|
| RETURN VALUE:
|    int
|
| MODULE:
|    what.c
|
|-KNOWN ERRORS AND FLAWS------------------------------------------------------|
|
|
|
|
|-CORRECTIONS-----------------------------------------------------------------|
|
|
|
|
|-----------------------------------------------------------------------------|
|1995-06-20/Erik Bachmann
\===========================================================================|*/

int  search_file(char *szInputFile)
{
      int         iStatusFlag       = 0;                    /* Return value */
      int         iTokenLength      = 0;
      char        szStr[MAXLINE+1];
      long        lStartOffset      = 0L;

      FILE        *fpInFile;

      /*-------------------------------------*/

      iTokenLength = strlen(szToken);

      if (0 >= iTokenLength)
      {
            pszErrMsg   = "Token is too short (ie. < 1)";
            fprintf(stderr, pszErrMsg);
            DEBUGTRACE(TRACEFILE, pszErrMsg);
            return(1);
      }

      if (0 == (fpInFile = fopen(szInputFile, "rb")))
      {
            fprintf(stderr, "Cannot open input file: %s", szInputFile);
            return(1);
      }

      lStartOffset = (long) find_str_in_file(fpInFile, lStartOffset, szToken);

      if (0 <= lStartOffset)
      {
            fprintf(stdout, "\n - %s\n", szInputFile);
            /* First found -> write filename */
      }

      while (0 <= lStartOffset)
      {
            ON_DEBUG { TRACE(TRACEFILE, "\n\t%s found in %s offset %lu",
                        szToken, szInputFile, lStartOffset); }

            /* Write to file if debug flag in active */

            lStartOffset += (long)strlen(szToken);

            /* Go beyond token */

            fseek(fpInFile, lStartOffset, SEEK_SET);

            /* Go to start offset */

            fgets(szStr, MAXLINE, fpInFile);

            /* Get the string after token */

            strtrimr(&szStr);                   /* Remove trailing line feed */

            fprintf(stdout, "%s\n", szStr);

            /* Write string */

            lStartOffset += (long)strlen(szStr);

            /* Next search starts from end of string */

            lStartOffset = (long)find_str_in_file(fpInFile,
                  (long)lStartOffset, szToken);

            /* Find next token */
      }
      fclose(fpInFile);

      return(iStatusFlag);                      /* Return default value */
}


/**** Main *****************************************************************/

/*
 /=======================================\
|   MAIN
|-------------------------------------|
|\=======================================/
|
| Looping through files matching the first argument
|
|
|
|
| Virtual main()
|
| See template.c
|
|-KNOWN ERRORS AND FLAWS------------------------------------------------------|
|
|
|
|
|-CORRECTIONS-----------------------------------------------------------------|
|
|
|
|
|-----------------------------------------------------------------------------|
|1995-06-19/Erik Bachmann
\===========================================================================|*/

int main(int argc, char *argv[])
{
      DOSFileData       ffblk;                        /* File control block */
      int                     done;                   /* Done flag */

      /*--------------------------------*/

      fprintf(stderr, "\n%Fs v. %Fs : %s\n", PROGNAME, PROGVER,
              stModulInfo.pszCopyright);
      fprintf(stdout, "Argument: %s\n", argv[1]);

      if ('?' == argv[1][1])
      {
            fprintf(stderr, HELPFORMAT, argv[0]);
            exit();
      }
      strcpy(szInputFile, argv[1]);

      done = FIND_FIRST(szInputFile, 0, &ffblk);

      /* Find first file */

      while (!done)
      {
            search_file(ff_name(&ffblk));  /* Search file for tokens */
            done = FIND_NEXT(&ffblk);      /* Find next matching file */
      }

      return(0);
}
