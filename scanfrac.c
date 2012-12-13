/* function scanfrac - scan an input string for a numeric value.
**
** Written in ANSI C and contributed to the public domain by
** Thad Smith III, Boulder, CO.     August 5, 1991
*/

/*******************************************************************
** scanfrac() scans an input string for a numeric value, which can
** be specified as:
**  1. an integer,               5
**  2. a floating point value,   5.1
**  3. a fraction, or            3/4
**  4. a mixed fraction.         5 3/4  or  5-3/4
**
** Conditions:
**  1. Preceding whitespace is allowed.
**  2. The input number may be signed.
**  3. The fractional part of a mixed fraction (but not pure fraction)
**     must be less than 1.
**  4. The numerator and denominator of a fraction or mixed fraction
**     must be less than 2^31.
**
** Parameters:
**  1. Input buffer containing value.
**  2. Pointer to double to receive return value.
**
** Return status:
**  0 = OK, value returned in f,
**  1 = bad input format,
**  2 = can't allocate memory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "snipmath.h"

int scanfrac (const char buf[], double *f)
{
      char *tbuf = malloc (strlen(buf) +2);   /* input + terminator     */
      static char term[] = "\a";              /* terminator flag        */
      char t1,t2,t3;                          /* separator chars        */
      char sign;                              /* possible sign          */
      int nc;                                 /* # conversions          */
      long int b,c;                           /* 2nd & 3rd inputs       */

      if (!tbuf)                          /* couldn't allocate memory   */
            return 2;

      /* Copy the input to a temporary buffer and append a terminator
      ** character.  This terminator is used to determine whether the
      ** scanning of the input field by sscanf() was terminated by end
      ** of input or by an invalid character.  If terminated properly,
      ** the terminator character picked up in t1, t2, or t3.
      */

      strcat (strcpy(tbuf, buf), term);       /* input + term flag      */
      nc = sscanf (tbuf, " %lf%c %ld %c %ld %c",
                           f,&t1,&b,&t2,&c,&t3);
      free (tbuf);

      switch (nc)                   /* number of sscanf() conversions   */
      {
      case 2:         /* single floating value: a */
            if (t1 == *term) return 0;
            break;
      case 4:         /* pure fraction: a/b */
            if (t1 == '/' && t2 == *term && fmod (*f,1.0) == 0.0 && b > 0)
            {
                  *f /= b;
                  return 0;
            }
            break;
      case 6:         /* mixed fraction: a b/c  or  a-b/c */
            if ((t1 == ' ' || t1 == '-') && t2 == '/' && t3 == *term &&
                  fmod (*f,1.0) == 0.0 && b >= 0 && c > b)
            {
                  /* get first non-blank character so that
                  ** -0 b/c will be neg
                  */

#ifdef __ZTC__  /* fix for missing const in sscanf() declaration */
                  sscanf ((char*)buf, " %c", &sign);
#else
                  sscanf (buf, " %c", &sign);
#endif
                  if (sign == '-')
                        *f -= (double)b/c;
                  else  *f += (double)b/c;
                  return 0;
            }
      }
      return 1;
}

#ifdef TEST

/* This is a simple test driver.  It should be omitted before
** placing scanfrac() into a library.
*/

main ()
{
      char buf[80];
      double f;
      int stat;

      printf ("Enter 999. or generate EOF to stop\n");
      do
      {
            printf ("Enter value: ");
            if (! gets (buf))
            {
                  printf ("EOF detected. Aborting.\n");
                  return 1;
            }
            stat = scanfrac (buf, &f);
            printf ("\nStat = %d, value = %f\n", stat, f);
      } while ( f != 999.);
      return 0;
}

#endif /* TEST */
