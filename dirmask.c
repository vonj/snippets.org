/*
**  DIRMASK.C - Complex pattern matching
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  The user is granted a free limited license to use this source file
**  to create royalty-free programs, subject to the terms of the
**  license restrictions specified in the LICENSE.MFL file.
*/

#include <stdio.h>
#include <string.h>
#include "sniptype.h"
#include "dirport.h"

/******************************************************************/
/*                                                                */
/*  dirmask()                                                     */
/*                                                                */
/*  Tests a directory entry for matching patterns. Tests both     */
/*  file name and attributes. Tests for both inclusion specs      */
/*  and exclusion specs.                                          */
/*                                                                */
/*  Parameters: 1 - Pointer to the directory entry's FIND         */
/*                  structure                                     */
/*              2 - True_ for case sensitivity, else False_.      */
/*              3 - Filename for inclusion matching, i.e. if      */
/*                  this spec matches the filename, we matched.   */
/*                  Use NULL to match anything.                   */
/*              4 - Filename for exclusion matching, i.e. if      */
/*                  this spec matches the filename, we failed.    */
/*                  Use NULL to exclude nothing.                  */
/*              5 - Attribute for inclusion mask. Use FA_ANY      */
/*                  to match anything).                           */
/*              6 - Attribute for exclusion mask. Use zero to     */
/*                  exclude nothing).                             */
/*                                                                */
/*  Returns: Success_ if name and attribute matched,              */
/*           else Error_.                                         */
/*                                                                */
/*  Side effects: Converts patterns to upper case if case is      */
/*                False_.                                         */
/*                                                                */
/******************************************************************/

int dirmask(DOSFileData    *dstruct,
            char           *fname_inc,
            char           *fname_exc,
            unsigned        attr_inc,
            unsigned        attr_exc)
{
      if (!dstruct)
            return Error_;
      if (fname_inc)
      {
            if (True_ != xstrcmp(ff_name(dstruct), fname_inc))
                  return Error_;
      }
      if (fname_exc)
      {
            if (True_ == xstrcmp(ff_name(dstruct), fname_exc))
                  return Error_;
      }
      if (!(ff_attr(dstruct) & attr_inc))
            return Error_;
      if (ff_attr(dstruct) & attr_exc)
            return Error_;
      return Success_;
}
