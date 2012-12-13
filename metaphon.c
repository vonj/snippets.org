/*
**  METAPHON.C - Phonetic string matching
**
**  The Metaphone algorithm was developed by Lawrence Phillips. Like the
**  Soundex algorithm, it compares words that sound alike but are spelled
**  differently. Metaphone was designed to overcome difficulties encountered
**  with Soundex.
**
**  This implementation was written by Gary A. Parker and originally published
**  in the June/July, 1991 (vol. 5 nr. 4) issue of C Gazette. As published,
**  this code was explicitly placed in the public domain by the author.
*/

#include <ctype.h>
#include "phonetic.h"

/*
**  Character coding array
*/

static char vsvfn[26] = {
      1,16,4,16,9,2,4,16,9,2,0,2,2,2,1,4,0,2,4,4,1,0,0,0,8,0};
/*    A  B C  D E F G  H I J K L M N O P Q R S T U V W X Y Z      */

/*
**  Macros to access the character coding array
*/

#define vowel(x)  (vsvfn[(x) - 'A'] & 1)  /* AEIOU    */
#define same(x)   (vsvfn[(x) - 'A'] & 2)  /* FJLMNR   */
#define varson(x) (vsvfn[(x) - 'A'] & 4)  /* CGPST    */
#define frontv(x) (vsvfn[(x) - 'A'] & 8)  /* EIY      */
#define noghf(x)  (vsvfn[(x) - 'A'] & 16) /* BDH      */

/*
**  metaphone()
**
**  Arguments: 1 - The word to be converted to a metaphone code.
**             2 - A MAXMETAPH+1 char field for the result.
**             3 - Function flag:
**                 If 0: Compute the Metaphone code for the first argument,
**                       then compare it to the Metaphone code passed in
**                       the second argument.
**                 If 1: Compute the Metaphone code for the first argument,
**                       then store the result in the area pointed to by the
**                       second argument.
**
**  Returns: If function code is 0, returns Success_ for a match, else Error_.
**           If function code is 1, returns Success_.
*/

Boolean_T metaphone(const char *Word, char *Metaph, metaphlag Flag)
{
      char *n, *n_start, *n_end;    /* Pointers to string               */
      char *metaph, *metaph_end;    /* Pointers to metaph               */
      char ntrans[512];             /* Word with uppercase letters      */
      char newm[MAXMETAPH + 4];     /* New metaph for comparison        */
      int KSflag;                   /* State flag for X translation     */

      /*
      ** Copy word to internal buffer, dropping non-alphabetic characters
      ** and converting to upper case.
      */

      for (n = ntrans + 1, n_end = ntrans + sizeof(ntrans) - 2;
            *Word && n < n_end; ++Word)
      {
            if (isalpha(*Word))
                  *n++ = toupper(*Word);
      }

      if (n == ntrans + 1)
            return Error_;           /* Return if zero characters        */
      else  n_end = n;              /* Set end of string pointer        */

      /*
      ** Pad with NULs, front and rear
      */

      *n++ = NUL;
      *n   = NUL;
      n    = ntrans;
      *n++ = NUL;

      /*
      ** If doing comparison, redirect pointers
      */

      if (COMPARE == Flag)
      {
            metaph = Metaph;
            Metaph = newm;
      }

      /*
      ** Check for PN, KN, GN, WR, WH, and X at start
      */

      switch (*n)
      {
      case 'P':
      case 'K':
      case 'G':
            if ('N' == *(n + 1))
                  *n++ = NUL;
            break;

      case 'A':
            if ('E' == *(n + 1))
                  *n++ = NUL;
            break;

      case 'W':
            if ('R' == *(n + 1))
                  *n++ = NUL;
            else if ('H' == *(n + 1))
            {
                  *(n + 1) = *n;
                  *n++ = NUL;
            }
            break;

      case 'X':
            *n = 'S';
            break;
      }

      /*
      ** Now loop through the string, stopping at the end of the string
      ** or when the computed Metaphone code is MAXMETAPH characters long.
      */

      KSflag = False_;              /* State flag for KStranslation     */
      for (metaph_end = Metaph + MAXMETAPH, n_start = n;
            n <= n_end && Metaph < metaph_end; ++n)
      {
            if (KSflag)
            {
                  KSflag = False_;
                  *Metaph++ = *n;
            }
            else
            {
                  /* Drop duplicates except for CC    */

                  if (*(n - 1) == *n && *n != 'C')
                        continue;

                  /* Check for F J L M N R  or first letter vowel */

                  if (same(*n) || (n == n_start && vowel(*n)))
                        *Metaph++ = *n;
                  else switch (*n)
                  {
                  case 'B':
                        if (n < n_end || *(n - 1) != 'M')
                              *Metaph++ = *n;
                        break;

                  case 'C':
                        if (*(n - 1) != 'S' || !frontv(*(n + 1)))
                        {
                              if ('I' == *(n + 1) && 'A' == *(n + 2))
                                    *Metaph++ = 'X';
                              else if (frontv(*(n + 1)))
                                    *Metaph++ = 'S';
                              else if ('H' == *(n + 1))
                                    *Metaph++ = ((n == n_start &&
                                          !vowel(*(n + 2))) ||
                                          'S' == *(n - 1)) ? 'K' : 'X';
                              else  *Metaph++ = 'K';
                        }
                        break;

                  case 'D':
                        *Metaph++ = ('G' == *(n + 1) && frontv(*(n + 2))) ?
                              'J' : 'T';
                        break;

                  case 'G':
                        if ((*(n + 1) != 'H' || vowel(*(n + 2))) &&
                              (*(n + 1) != 'N' || ((n + 1) < n_end &&
                              (*(n + 2) != 'E' || *(n + 3) != 'D'))) &&
                              (*(n - 1) != 'D' || !frontv(*(n + 1))))
                        {
                              *Metaph++ = (frontv(*(n + 1)) &&
                                    *(n + 2) != 'G') ? 'J' : 'K';
                        }
                        else if ('H' == *(n + 1) && !noghf(*(n - 3)) &&
                              *(n - 4) != 'H')
                        {
                              *Metaph++ = 'F';
                        }
                        break;

                  case 'H':
                        if (!varson(*(n - 1)) && (!vowel(*(n - 1)) ||
                              vowel(*(n + 1))))
                        {
                              *Metaph++ = 'H';
                        }
                        break;

                  case 'K':
                        if (*(n - 1) != 'C')
                              *Metaph++ = 'K';
                        break;

                  case 'P':
                        *Metaph++ = ('H' == *(n + 1)) ? 'F' : 'P';
                        break;

                  case 'Q':
                        *Metaph++ = 'K';
                        break;

                  case 'S':
                        *Metaph++ = ('H' == *(n + 1) || ('I' == *(n + 1) &&
                              ('O' == *(n + 2) || 'A' == *(n + 2)))) ?
                              'X' : 'S';
                        break;

                  case 'T':
                        if ('I' == *(n + 1) && ('O' == *(n + 2) ||
                              'A' == *(n + 2)))
                        {
                              *Metaph++ = 'X';
                        }
                        else if ('H' == *(n + 1))
                              *Metaph++ = 'O';
                        else if (*(n + 1) != 'C' || *(n + 2) != 'H')
                              *Metaph++ = 'T';
                        break;

                  case 'V':
                        *Metaph++ = 'F';
                        break;

                  case 'W':
                  case 'Y':
                        if (vowel(*(n + 1)))
                              *Metaph++ = *n;
                        break;

                  case 'X':
                        if (n == n_start)
                              *Metaph++ = 'S';
                        else
                        {
                              *Metaph++ = 'K';
                              KSflag = True_;
                        }
                        break;

                  case 'Z':
                        *Metaph++ = 'S';
                        break;
                  }
            }

            /*
            ** Compare new Metaphone code with old
            */

            if (COMPARE == Flag &&
                  *(Metaph - 1) != metaph[(Metaph - newm) - 1])
            {
                  return Error_;
            }
      }

      /*
      ** If comparing, check if Metaphone codes were equal in length
      */

      if (COMPARE == Flag && metaph[Metaph - newm])
            return Error_;

      *Metaph = NUL;
      return Success_;
}

#ifdef TEST

/*
**  Test demo to search a drive for a filename which sounds similar to
**  file names passed on the command line
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"     /* metaphone() is portable so use Posix   */
#include "filnames.h"   /* valid_fname() prototype                */
#include "snip_str.h"   /* strchcat() prototype                   */

/*
**  Prototypes
*/

void print_find_t(char * dir, DOSFileData *find);
void search(char *dir, char *name);

Boolean_T found = False_;
char meta[MAXMETAPH + 4];

main(int argc, char *argv[])
{
      char *ptr;

      if (2 > argc)
      {
            puts("Usage: METAPHON filename [...filename]");
            return EXIT_FAILURE;
      }
      while (--argc)
      {
            char *fname = *++argv;

            if (Success_ != valid_fname(fname, -1))
            {
                  printf("\nIllegal filename: %s\n", fname);
                  continue;
            }
            printf("\nSearching for: %s\n", fname);

            /* Remove the extension, if any     */

            if (NULL != (ptr = strchr(fname, '.')))
                  *ptr = NUL;

            /* Store the Metaphone code         */

            metaphone(fname, meta, GENERATE);
            printf("Metaphone for %s is %s\n", fname, meta);

            /* Search for matches               */

            search("", "");
            if (!found)
                  puts("A match was not found");
      }
      return EXIT_SUCCESS;
}

void search(char *dir, char *newdir)
{
      char         curdir[FILENAME_MAX];
      DIR         *dirp;
      DOSFileData *dstruct;
      char        *ptr;
      Boolean_T    retval;

      while (NULL != (ptr = strchr(dir, '\\')))
            *ptr = '/';

      strcpy(curdir, dir);

      if ('/' != LAST_CHAR(curdir))
            strchcat(curdir, '/', FILENAME_MAX);

      strcat(curdir, newdir);

      if (NULL != (dirp = opendir(curdir)))
      {
            while (NULL != (dstruct = readdir(dirp)))
            {
                  if ('.' == *(ff_name(dstruct)))
                        continue;
                  else
                  {
                        /* Don't look at file extension     */

                        if (NULL != (ptr = strchr(ff_name(dstruct), '.')))
                              *ptr = NUL;
                        else  ptr  = NULL;

                        retval = metaphone(ff_name(dstruct), meta, COMPARE);

                        /* Restore extension, if any        */

                        if (ptr != NULL)
                              *ptr = '.';

                        if (Success_ == retval)
                              print_find_t(curdir, dstruct);
                  }
                  if (ff_attr(dstruct) & _A_SUBDIR)
                        search(curdir, ff_name(dstruct));
            }
            closedir(dirp);
      }
}

void print_find_t(char *dir, DOSFileData *find)
{
      printf("%s/%-12s %8ld %2.2d-%02.2d-%02.2d %c%c%c%c%c\n", dir,
            ff_name(find), ff_size(find), ff_mo(find),
            ff_day(find), (ff_yr(find) + 80) % 100,
            (ff_attr(find) & _A_SUBDIR) ? 'D' : '.',
            (ff_attr(find) & _A_RDONLY) ? 'R' : '.',
            (ff_attr(find) & _A_HIDDEN) ? 'H' : '.',
            (ff_attr(find) & _A_SYSTEM) ? 'S' : '.',
            (ff_attr(find) & _A_ARCH)   ? 'A' : '.');
      found = True_;
}

#endif /* TEST */
