/******************************************************************************
* @(#) CMPSTR  - Comparing strings with variable sort order
* @(#) TCMPSTR - Comparing strings with variable sort order and
*                masking/truncation
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*1994-03-15/Bac
*   TCMPSTR added
*
*******************************************************************************
*1994/Erik Bachmann (E-mail: ebp@dde.dk)
*
* Released to public domain 27-Oct-95
******************************************************************************/

#include <string.h>                    /* strlen */

/**
#define TEST
**/

/*
 /-------------------------------------\
|          CMPSTR                       |------------------------------------|
|\-------------------------------------/
|
| Comparing two strings with variable sort order
|
|
| Examples of sort sequences found in SORTKEY.h
|
|
|
|----------------------------------------------------------------------------|
| CALL:
|    iOrder = cmpstr("abc‘›†", "abc†›‘",
|                    (unsigned char *) CaseMatch) ;
|
| HEADER:
|    sortkey.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    char *pszStr1      Pointer to first string
|    char *pszStr2      Pointer to second string
|    char *pszOrder     Pointer to table with sort sequences
|
| PROTOTYPE:
|    int far cmpstr(unsigned char *pszStr1, unsigned char *pszStr2,
|                   unsigned char *pszOrder) ;
|
| RETURN VALUE:
|    -1          pszStr1 is first
|    0           the two strings are identical (i sort order)
|    1           pszStr2 is first
|
| MODULE:
|    cmpstr.c
|----------------------------------------------------------------------------|
|1993-06-07/Bac   Length comparision uptimised (length calculation excluded)

|
|----------------------------------------------------------------------------|
|1993-04-10/Erik Bachmann
\---------------------------------------------------------------------------|*/

int far cmpstr(unsigned char *pszStr1,
               unsigned char *pszStr2,
               unsigned char *pszOrder)
{
      int i = 0;

      /*---------------------------------------------------------*/

      while (('\0' != pszStr1[i]) && ('\0' != pszStr2[i]))
      {
            if (pszOrder[pszStr1[i]] < pszOrder[pszStr2[i]])
                  return -1;        /* Char from first string is first  */

            if (pszOrder[pszStr1[i]] > pszOrder[pszStr2[i]])
                  return 1;         /* Char from second string is first */

            i++;                    /* Next char                        */
      }

      if (('\0' == pszStr1[i] ) && ('\0' != pszStr2[i]))
            return -1;              /* First string is shortest         */

      if (('\0' != pszStr1[i] ) && ('\0' == pszStr2[i]))
            return 1;               /* Second string is shortest        */

      return 0;                     /* They are identical               */
}

/*
 /-------------------------------------\
|          TCMPSTR                      |------------------------------------|
|\-------------------------------------/
|
| Comparing two strings with variable sort order and truncation/masking
|
|
| Examples of sort sequences found in SORTKEY.h
|
|
|
|----------------------------------------------------------------------------|
| CALL:
|    iOrder = cmpstr("ab$‘›†?", "abc†›‘123",
|                    (unsigned char *) CaseMatch, "$", "?") ;
|
| HEADER:
|    sortkey.h
|
| GLOBALE VARIABLES:
|    %
|
| ARGUMENTS:
|    char *pszStr1      Pointer to first string
|    char *pszStr2      Pointer to second string
|    char *pszOrder     Pointer to table with sort sequences
|
| PROTOTYPE:
|    int far cmpstr(unsigned char *pszStr1,  unsigned char *pszStr2,
|                   unsigned char *pszOrder, unsigned char *pszMask,
|                   unsigned char *pszTrunc) ;
|
| RETURN VALUE:
|    -1          pszStr1 is first
|    0           the two strings are identical (i sort order)
|    1           pszStr2 is first
|
| MODULE:
|    cmpstr.c
|----------------------------------------------------------------------------|
|
|----------------------------------------------------------------------------|
|1994-03-14/Erik Bachmann
\---------------------------------------------------------------------------|*/

int far tcmpstr(unsigned char *pszStr1,
                unsigned char *pszStr2,
                unsigned char *pszOrder,
                unsigned char *pszMask,
                unsigned char *pszTrunc)
{
      int  i = 0;
      char cTruncateFlag = 0;
      int  iCompareLength;

      /*---------------------------------------------------------*/

      iCompareLength = strlen(pszStr1) - 1 ;
                                    /* Find last charater i string 1    */

      if (strrchr(pszTrunc, pszStr1[iCompareLength])
          && (0 <= iCompareLength))
                                    /* While string is terminated by one */
      {                             /* of the specified characters       */

            cTruncateFlag++;
            iCompareLength--;
      }

      while ((((0 != cTruncateFlag) && (i < iCompareLength)
               || (0 == cTruncateFlag) && ('\0' != pszStr1[i])))
             && ('\0' != pszStr2[i]))
      {
            if (0 != strchr(pszMask, pszStr1[i]))
            {
                  i++;
                  continue;         /* If string masked: continue       */
            }

            if (pszOrder[pszStr1[i]] < pszOrder[pszStr2[i]])
                  return -1;        /* Char from first string is first  */

            if (pszOrder[pszStr1[i]] > pszOrder[pszStr2[i]])
                  return 1;         /* Char from second string is first */

            i++;                    /* Next char                        */
      }

      if ((('\0' == pszStr1[i]) && ('\0' != pszStr2[i]))
          && (0 == cTruncateFlag))
      {
            return -1;              /* First string is shortest         */
      }

      if ((('\0' != pszStr1[i]) && ('\0' == pszStr2[i]))
          && 0 == cTruncateFlag)
      {
            return 1;               /* Second string is shortest        */
      }

      return 0;                     /* They are identical               */
}


#ifdef TEST

#include "sortkey.h"

void state(int iOrder)
{
      if (0 == iOrder)
            printf("\tequal");
      else
      {
            if (0 <= iOrder)
                  printf("\tString 1 is higher than string 2");
            else  printf("\tString 1 is lower than string 2");
      }
}

void main(void)
{
      int iOrder = 0;
      char  *str1a   = "ab$‘›†?",
            *str1b   = "ab$‘›†",
            *str1c   = "abc‘›†?",
            *str1d   = "ab$‘›†",
            *str1e   = "abc†‘›",
            *str1f   = "abc‘›†_",

            *str2a   = "abc‘›†123",
            *str2b   = "abc‘›†";

      printf("\n\n");

      /* 2a */

      iOrder = tcmpstr(str1a, str2a, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1a, str2a);
      state(iOrder);

      iOrder = tcmpstr(str1b, str2a, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1b, str2a);
      state(iOrder);

      iOrder = tcmpstr(str1c, str2a, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1c, str2a);
      state(iOrder);

      iOrder = tcmpstr(str1d, str2a, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1d, str2a);
      state(iOrder);

      iOrder = tcmpstr(str1e, str2a, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1e, str2a);
      state(iOrder);

      iOrder = tcmpstr(str1f, str2a, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1f, str2a);
      state(iOrder);

      /* 2b */

      iOrder = tcmpstr(str1a, str2b, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1a, str2b);
      state(iOrder);

      iOrder = tcmpstr(str1b, str2b, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1b, str2b);
      state(iOrder);

      iOrder = tcmpstr(str1c, str2b, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1c, str2b);
      state(iOrder);

      iOrder = tcmpstr(str1d, str2b, (unsigned char *) CaseMatch, "$", "?");
      printf("\n%10s / %10s", str1d, str2b);
      state(iOrder);
}
#endif
