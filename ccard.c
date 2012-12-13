/*
 *    ccard - credit card number validation
 *    1994 Peter Miller
 *    Public Domain
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * MANIFEST: functions to validate credit card numbers
 *
 * derived from code by
 *    Chris Stone <cstone@hms.com>
 *    The High Mountain Software Internet Gateway
 *
 * translated to C by
 *    Peter Miller, 28-Oct-94
 *    This source is hereby placed in the Public Domain.
 *    Please leave my name on it,
 *    and document changes in this header block.
 *
 * NO WARRANTY
 *
 *    BECAUSE THE PROGRAM IS IN THE PUBLIC DOMAIN, THERE IS NO
 *    WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
 *    LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE AUTHORS
 *    AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT
 *    WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *    BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *    AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO
 *    THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD
 *    THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL
 *    NECESSARY SERVICING, REPAIR OR CORRECTION.
 *
 *    IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
 *    WRITING WILL ANY AUTHOR, OR ANY OTHER PARTY WHO MAY MODIFY
 *    AND/OR REDISTRIBUTE THE PROGRAM, BE LIABLE TO YOU FOR DAMAGES,
 *    INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL
 *    DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM
 *    (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING
 *    RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES
 *    OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
 *    PROGRAMS), EVEN IF SUCH AUTHOR OR OTHER PARTY HAS BEEN ADVISED
 *    OF THE POSSIBILITY OF SUCH DAMAGES.
 */

#include <ctype.h>
#include <string.h>

#include "ccard.h"

#define SIZEOF(a) (sizeof(a) / sizeof((a)[0]))
#define ENDOF(a) ((a) + SIZEOF(a))

#define MINLEN 12
#define MAXLEN (MINLEN + 16)
#define L(n) (1 << ((n) - MINLEN))

static int all_numeric (char *, char *, int);


char *ccard_type_name(ccard_type_ty n)
{
      static char *name[] =
      {
            "unknown",
            "Mastercard",
            "Visa",
            "American Express",
            "Diners Club/Carte Blanche",
            "Discover",
            "enRoute",
            "Japanese Credit Bureau",
            "Australian Bankcard",
      };

      if (n < 0 || n >= SIZEOF(name))
            n = 0;
      return name[n];
}


char *ccard_error_name(ccard_error_ty n)
{
      static char *name[] =
      {
            "no error",
            "card type unknown",
            "card number contains non numeric characters",
            "card number is far too long",
            "card number is the wrong length",
            "checksum incorrect",
      };

      if (n < 0 || n >= SIZEOF(name))
            return "unknown";
      return name[n];
}


static int verify_checksum(char *credit_card)
{
      char        *cp;
      int         dbl;
      int         check_sum;

      /*
       * This checksum algorithm has a name,
       * but I can't think of it.
       */
      check_sum = 0;
      dbl = 0;
      /* assert(credit_card[0]); */
      cp = credit_card + strlen(credit_card) - 1;
      while (cp >= credit_card)
      {
            int         c;

            c = *cp-- - '0';
            if (dbl)
            {
                  c *= 2;
                  if (c >= 10)
                        c -= 9;
            }
            check_sum += c;
            dbl = !dbl;
      }

      return ((check_sum % 10) == 0);
}



static int all_numeric(char *s1, char *s2, int max)
{
      while (*s1)
      {
            if (isspace(*s1) || *s1 == '-')
            {
                  ++s1;
                  continue;
            }
            if (!isdigit(*s1))
                  return ccard_error_non_numeric;
            if (max <= 0)
                  return ccard_error_too_long;
            *s2++ = *s1++;
            --max;
      }
      *s2 = 0;
      return ccard_error_none;
}


ccard_error_ty ccard_valid(char *credit_card_in, ccard_type_ty *card_type)
{
      typedef struct table_ty table_ty;
      struct table_ty
      {
            char        *prefix;
            int         length_mask;
            ccard_type_ty     type;
            int         checksum;
      };

      static table_ty table[] =
      {
            { "1800", L(15), ccard_type_jcb, 1, },
            { "2014", L(15), ccard_type_enroute, 0, },
            { "2131", L(15), ccard_type_jcb, 1, },
            { "2149", L(15), ccard_type_enroute, 0, },
            { "300",  L(14), ccard_type_diners, 1, },
            { "301",  L(14), ccard_type_diners, 1, },
            { "302",  L(14), ccard_type_diners, 1, },
            { "303",  L(14), ccard_type_diners, 1, },
            { "304",  L(14), ccard_type_diners, 1, },
            { "305",  L(14), ccard_type_diners, 1, },
            { "34",   L(15), ccard_type_amex, 1, },
            { "36",   L(14), ccard_type_diners, 1, },
            { "37",   L(15), ccard_type_amex, 1, },
            { "38",   L(14), ccard_type_diners, 1, },
            { "3",    L(16), ccard_type_jcb, 1, },
            { "4",    L(13)|L(16), ccard_type_visa, 1, },
            { "51",   L(16), ccard_type_mastercard, 1, },
            { "52",   L(16), ccard_type_mastercard, 1, },
            { "53",   L(16), ccard_type_mastercard, 1, },
            { "54",   L(16), ccard_type_mastercard, 1, },
            { "55",   L(16), ccard_type_mastercard, 1, },
            { "56",   L(16), ccard_type_bankcard, 1, },
            { "6011", L(16), ccard_type_discover, 1, },
      };
      table_ty    *tp;
      char        credit_card[MAXLEN + 1];
      ccard_error_ty    err;
      int         len;

      /*
       * copy the number, eliding spaces
       * defer any errors until after we have tried to guess the card type
       */
      err = all_numeric(credit_card_in, credit_card, MAXLEN);

      /*
       * look for the card prefix in the table
       * to determine the card type
       */
      for (tp = table; tp < ENDOF(table); ++tp)
      {
            if (!memcmp(tp->prefix, credit_card, strlen(tp->prefix)))
                  break;
      }
      if (tp >= ENDOF(table))
      {
            *card_type = ccard_type_unknown;
            return ccard_error_type_unknown;
      }
      *card_type = tp->type;
      if (err != ccard_error_none)
            return err;

      /*
       * set the card type, then check the length
       */
      /* assert(tp->correct_length <= MAXLEN); */
      len = strlen(credit_card);
      if (len < MINLEN || (L(len) & tp->length_mask) == 0)
            return ccard_error_length_incorrect;

      /*
       * checksum
       */
      if (tp->checksum && !verify_checksum(credit_card))
            return ccard_error_checksum;
      
      /*
       * no errors found
       */
      return ccard_error_none;
}

#ifdef TEST
/*
 *    ccard - credit card number validation
 *    1994 Peter Miller
 *    Public Domain
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * MANIFEST: program entry point
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccardplv.h"

static char *progname;


static int suffix(char *s1, char *s2)
{
      int len1 = strlen(s1);
      int len2 = strlen(s2);
      return (len2 < len1 && !memcmp(s1 + len1 - len2, s2, len2));
}


static void print_version(char *s)
{
      char        *ep;

      for (;;)
      {
            ep = strrchr(s, '/');
            if (!ep)
                  break;
            if (ep > s && !ep[1])
            {
                  *ep = 0;
                  continue;
            }
            s = ep + 1;
            break;
      }
      progname = s;

      fprintf(stderr, "%s version %s\n", progname, PATCHLEVEL);
}


int main(int argc, char *argv[])
{
      int         j;

      print_version(argv[0]);
      for (j = 1; j < argc; ++j)
      {
            ccard_type_ty     type;
            ccard_error_ty    err;
            char        *s;

            err = ccard_valid(argv[j], &type);
            if (err)
            {
                  if (type != ccard_type_unknown)
                  {
                        fprintf
                        (
                              stderr,
                              "%s: %s: %s (%s)\n",
                              progname,
                              argv[j],
                              ccard_error_name(err),
                              ccard_type_name(type)
                        );
                  }
                  else
                  {
                        fprintf
                        (
                              stderr,
                              "%s: %s: %s\n",
                              progname,
                              argv[j],
                              ccard_error_name(err)
                        );
                  }
                  exit(1);
            }
            printf("\"%s\" is a", argv[j]);
            s = ccard_type_name(type);
            if (strchr("AEIOUaeiou", s[0]))
                  printf("n");
            printf(" %s", s);
            if (!suffix(s, "card"))
                  printf(" card");
            printf("\n");
      }
      exit(0);
      return 0;
}

#endif /* TEST */
