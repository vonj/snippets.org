/*
**  XSTRCMP.C - Simple string pattern matching functions using DOS
**              wildcards ('?' & '*').
**
**  Derived from code by Arjan Kentner (submitted by Steve Summit),
**  modified by Bob Stout.
*/

#include <stdio.h>                  /* For NULL                         */
#include <ctype.h>                  /* For toupper()                    */
#include <assert.h>                 /* For assert()                     */
#include "sniptype.h"               /* For Boolean_T                    */
#include "dirent.h"                 /* Validate prototypes, also
                                       includes extkword.h for PASCAL   */

/*
**  Arguments: 1 - Pattern to match
**             2 - String to match
**
**  Returns: True_ if match
**           False_ if no match
**           Error_ if passed a null pointer (see below)
**
**  Notes: 1. Two versions are supplied, one case-sensitive and one not.
**         2. Each version consists of a recursive static function which
**            does all the work, and a wrapper which checks for null
**            pointers before calling the static function.
**         3. If assert() is enabled (i.e. if NDEBUG is undefined or false),
**            the wrapper functions will abort with an assertion error in
**            case a null pointer is passed.
**         4. If assert() is disabled (i.e. if NDEBUG is defined), the
**            wrapper function will return Error_ to the calling program in
**            case a null pointer is passed.
*/

/*
**  Case-sensitive version
*/

static Boolean_T PASCAL patmat (const char *pat, const char *str)
{
      switch (*pat)
      {
      case '\0':
            return !*str;

      case '*' :
            return patmat(pat+1, str) || *str && patmat(pat, str+1);

      case '?' :
            return *str && patmat(pat+1, str+1);

      default  :
            return (*pat == *str) && patmat(pat+1, str+1);
      }
}

Boolean_T xstrcmp (const char *pat, const char *str)
{
      assert(str && pat);
      if (NULL == str || NULL == pat)
            return Error_;
      else  return(patmat(pat, str));
}

/*
**  Case-insensitive version
*/

static Boolean_T PASCAL patimat (const char *pat, const char *str)
{
      switch (*pat)
      {
      case '\0':
            return !*str;

      case '*' :
            return patimat(pat+1, str) || *str && patimat(pat, str+1);

      case '?' :
            return *str && patimat(pat+1, str+1);

      default  :
            return (toupper(*pat) == toupper(*str)) && patimat(pat+1, str+1);
      }
}

Boolean_T xstricmp (const char *pat, const char *str)
{
      assert(str && pat);
      if (NULL == str || NULL == pat)
            return Error_;
      else  return(patimat(pat, str));
}

#ifdef TEST

#include <stdio.h>

main(int argc, char *argv[])
{
      if (3 != argc)
      {
            puts("Usage: XSTRCMP mask string");
            return -1;
      }
      printf("xstrcmp(\"%s\", \"%s\") returned %d\n", argv[1], argv[2],
            xstrcmp(argv[1], argv[2]));

      printf("xstricmp(\"%s\", \"%s\") returned %d\n", argv[1], argv[2],
            xstricmp(argv[1], argv[2]));
      
      printf("xstricmp(NULL, \"%s\") returned %d\n", argv[2],
            xstricmp(NULL, argv[2]));
      
      printf("xstricmp(\"%s\", NULL) returned %d\n", argv[1],
            xstricmp(argv[1], NULL));
      
      printf("xstricmp(NULL, NULL) returned %d\n", xstricmp(NULL, NULL));
      
      return 0;
}

#endif /* TEST */
