/*
**  valid_fname.c AKA vfname.c (revised)
*/

#include <stdio.h>
#include <string.h>
#include "filnames.h"
#include "vfname.h"


/*
**  valid_fname.c
**
**  Verifies whether a filename is valid or invalid without
**  altering the passed filename itself.
**
**  Arguments: 1 - fname = a filename with no path or drive
**             2 - wild_check: 0 means wildcard use okay
**                             any other value means test for
**                             wildcards which are not acceptable
**
**  Returns:   Error_   - fname is invalid
**             Success_ - fname is valid
**
**  Side effects: none
**
**  Notes: Space, ASCII character 32, is a special case. Dos will
**         write a filename or volume label that includes a space.
**         Getting access to that file, afterwards, is not always
**         easy :) For my purposes, space is an invalid filename
**         token. You? You're on your own :)
**
**         High-bit-set or embedded control characters are not handled
**         in this version.
**
**  Revisions: 1) Dropped str2upper after comment by David Johnson
**                on 07-17-93
**             2) Added [] to token list after comment by Ed
**                Kowalski on 07-17-93
**             3) Added lpt1-lpt3, com1-com4 and clock$ to
**                invalid name list after comment by Ed
**                Kowalski on 07-17-93
**             4) Eliminated double exit points after my own
**                comment to Bob Stout :)
**             5) Major revision to clean up code and add parameterized
**                filename and format validation
**
**  Public domain by Sid Rogers on 07/22/1993,
**                   Bob Stout 11-Sep-95
*/

Boolean_T valid_fname(const char *fname, Boolean_T wild_check)
{
      /*
      ** Invalid filename tokens - path separators first, wildcards last
      */

      static char *invalid_tokens=
            (PATH_SEP_CHARS INVALID_CHARS WILDCARD_CHARS);

      /*
      ** Invalid file names -- even with extension .xxx
      */

      static char *invalid_3lnam[] = DEVICE_LIST_3;
      static char *invalid_4lnam[] = DEVICE_LIST_4;
      static char *invalid_6lnam[] = DEVICE_LIST_6;

      int num_vals;
      int fl = strlen(fname);
      int i, j, k;
      char *ptr;

      if (wild_check)               /* Using wildcards invalid if TRUE  */
            num_vals = strlen(invalid_tokens);
      else  num_vals = strlen(invalid_tokens) - strlen(WILDCARD_CHARS);

      for (i = 0; NULL != invalid_3lnam[i]; ++i)
      {
            if (Success_ == strncmp(invalid_3lnam[i], fname, 3))
                  return Error_;
      }

      for (i = 0; NULL != invalid_4lnam[i]; ++i)
      {
            if (Success_ == strncmp(invalid_4lnam[i], fname, 4))
                  return Error_;
      }

      for (i = 0; NULL != invalid_6lnam[i]; ++i)
      {
            if (Success_ == strncmp(invalid_6lnam[i], fname, 6))
                  return Error_;
      }

      /* Process filename for invalid tokens    */

      for (i = -1, j = 0; i < 0 && j <= num_vals; j++)
      {
            for (k = 0; k < fl; k++)
            {
                  if (invalid_tokens[j] == fname[k])
                        i = j;
            }
            if (i >= 0)
                  return Error_;
      }

      /*
      ** OK so far, check format
      */

      if (fl > FNAME_MAX)
            return Error_;

      for (i = 0; i < strlen(PATH_SEP_CHARS); ++i)
      {
            if (NULL != strchr(fname, invalid_tokens[i]))
                  return Error_;
      }
      
      if (NULL == (ptr = strchr(fname, '.')))
      {
            if (fl > FNAME_MAX)
                  return Error_;
      }
      else
      {
            if ((ptr - (char *)fname) > FNAME_SIZE_MAX)
                  return Error_;

#if FNAME_SIZE_MIN

            if ((ptr - (char *)fname) < FNAME_SIZE_MIN)
                  return Error_;

#endif

            ++ptr;                              /* Skip the period      */

            if (!MULTIPLE_EXTS && NULL != strchr(ptr, '.'))
                  return Error_;

            if (strlen(ptr) > FEXT_SIZE_MAX)
                  return Error_;

#if FEXT_SIZE_MIN

            if (strlen(ptr) < FEXT_SIZE_MIN)
                  return Error_;
#endif

      }

      return Success_;
}

#ifdef TEST

main()
{
      static char *name_test[]= {"aaa","aux","con","prn","nul","lpt1",
                                    "lpt2","lpt3","com1","com2","com3",
                                    "com4","bbbb","clock$","cccccc",
                                    "dev/nul","clock$.ext",NULL};
      static char *token_test[]={"00fname.","01 fname","02fname,",
                                    "03fname[","04fname;","05fname:",
                                    "06fname|","07fname/","08fname<",
                                    "09fname>","10fname+","11fname=",
                                    "12fname\\","13fname\"","14fname]",
                                    "15fname*","16fname?","filename.17",
                                    "filename.018","19longfilename",
                                    "20.longext","21.mlt.exts","a.22",
                                    ".23","fname24.",NULL};
      register int i;

      puts("Names test");
      for (i = 0; (name_test[i]); ++i)
      {
            printf("%6s is %s\n", name_test[i],
                  valid_fname(name_test[i], 0) ? "INvalid" : "Valid");
      }

      puts("\nTokens test");
      for (i = 0; (token_test[i]); ++i)
      {
            printf("%s is %s\n", token_test[i],
                  valid_fname(token_test[i], 1) ? "INvalid" : "Valid");
      }

      return 0;
}

#endif /* TEST */
