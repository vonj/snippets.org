/*
**  Variant Soundex Algorithm (Algorithm #4), Optimized for use with Surnames
**
**  Written by Joe Celko. Originally published in "The C Gazette" vol. 4 nr. 2
**  Source code may be freely used is source is acknowledged
**  Object code may be freely used
**
**  Uses STRUPR.C, also from SNIPPETS
*/

/*
**  Preserves first character, uppercase name, drop non-alpha characters,
**  convert letters to Soundex digits, and returns first N letters.
**  Many of the steps here could be combined into the same loop, but
**  they are kept separate for clarity and to give the user a chance
**  to experiment with changes.
*/

#include <string.h>
#include "phonetic.h"
#include "snip_str.h"

#define WBUFSIZE 512

void soundex4(const char *instr, char *outstr, int N)
{
      char *p, *p1;
      int i;
      char workbuf[WBUFSIZE + 1];
      char priorletter;

      /* Make a working copy  */

      strncpy(workbuf, instr, WBUFSIZE);
      workbuf[WBUFSIZE] = NUL;
      strupr(workbuf);

      /* Convert all vowels to 'A'  */

      for (p = workbuf; *p; ++p)
      {
            if (strchr("AEIOUY", *p))
                  *p = 'A';
      }

      /* Prefix transformations: done only once on the front of a name  */

      if (Success_ == strncmp(workbuf, "MAC", 3))     /* MAC to MCC     */
            workbuf[1] = 'C';
      else if (Success_ == strncmp(workbuf, "KN", 2)) /* KN to NN       */
            workbuf[0] = 'N';
      else if ('K' == workbuf[0])                     /* K to C         */
            workbuf[0] = 'C';
      else if (Success_ == strncmp(workbuf, "PF", 2)) /* PF to FF       */
            workbuf[0] = 'F';
      else if (Success_ == strncmp(workbuf, "SCH", 3))/* SCH to SSS     */
            workbuf[1] = workbuf[2] = 'S';

      /*
      ** Infix transformations: done after the first letter,
      ** left to right
      */

      while ((p = strstr(workbuf, "DG")) > workbuf)   /* DG to GG       */
            p[0] = 'G';
      while ((p = strstr(workbuf, "CAAN")) > workbuf) /* CAAN to TAAN   */
            p[0] = 'T';
      while ((p = strchr(workbuf, 'D')) > workbuf)    /* D to T         */
            p[0] = 'T';
      while ((p = strstr(workbuf, "NST")) > workbuf)  /* NST to NSS     */
            p[2] = 'S';
      while ((p = strstr(workbuf, "AV")) > workbuf)   /* AV to AF       */
            p[1] = 'F';
      while ((p = strchr(workbuf, 'Q')) > workbuf)    /* Q to G         */
            p[0] = 'G';
      while ((p = strchr(workbuf, 'Z')) > workbuf)    /* Z to S         */
            p[0] = 'S';
      while ((p = strchr(workbuf, 'M')) > workbuf)    /* M to N         */
            p[0] = 'N';
      while ((p = strstr(workbuf, "KN")) > workbuf)   /* KN to NN       */
            p[0] = 'N';
      while ((p = strchr(workbuf, 'K')) > workbuf)    /* K to C         */
            p[0] = 'C';
      while ((p = strstr(workbuf, "AH")) > workbuf)   /* AH to AA       */
            p[1] = 'A';
      while ((p = strstr(workbuf, "HA")) > workbuf)   /* HA to AA       */
            p[0] = 'A';
      while ((p = strstr(workbuf, "AW")) > workbuf)   /* AW to AA       */
            p[1] = 'A';
      while ((p = strstr(workbuf, "PH")) > workbuf)   /* PH to FF       */
            p[0] = p[1] = 'F';
      while ((p = strstr(workbuf, "SCH")) > workbuf)  /* SCH to SSS     */
            p[0] = p[1] = 'S';

      /*
      ** Suffix transformations: done on the end of the word,
      ** right to left
      */

      /* (1) remove terminal 'A's and 'S's      */

      for (i = strlen(workbuf) - 1;
            (i > 0) && ('A' == workbuf[i] || 'S' == workbuf[i]);
            --i)
      {
            workbuf[i] = NUL;
      }

      /* (2) terminal NT to TT      */

      for (i = strlen(workbuf) - 1;
            (i > 1) && ('N' == workbuf[i - 1] || 'T' == workbuf[i]);
            --i)
      {
            workbuf[i - 1] = 'T';
      }

      /* Now strip out all the vowels except the first     */

      p = p1 = workbuf;
      while (NUL != (*p1++ = *p++))
      {
            while ('A' == *p)
                  ++p;
      }

      /* Remove all duplicate letters     */

      p = p1 = workbuf;
      priorletter = NUL;
      do {
            while (*p == priorletter)
                  ++p;
            priorletter = *p;
      } while (NUL != (*p1++ = *p++));

      /* Finish up      */

      strncpy(outstr, workbuf, N);
      outstr[N] = NUL;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
      char outbuf[80];
      int N;

      if (argc != 3)
      {
            puts("Usage: SOUNDEX4 string length");
            return EXIT_FAILURE;
      }

      soundex4(argv[1], outbuf, N = atoi(argv[2]));

      printf("soundex4(\"%s\", %d) => \"%s\"\n", argv[1], N, outbuf);

      return EXIT_SUCCESS;
}

#endif /* TEST */
