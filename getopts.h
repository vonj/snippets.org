/************************************************************************/
/*                                                                      */
/*  MFLFILES Header file                                                */
/*                                                                      */
/*  Function prototypes for file system access functions.               */
/*                                                                      */
/*  Original Copyright 1990-96 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL).                               */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#ifndef GETOPTS_H
#define GETOPTS_H

#include <stdio.h>
#include "sniptype.h"
#include "extkword.h"

#if __cplusplus
 extern "C" {
#endif

/************************************************************************/
/*                                                                      */
/*  Process command line options and wildcard arguments                 */
/*                                                                      */
/************************************************************************/

/*
**  Types of arguments we understand
*/

typedef enum {
      Error_Tag,
      Boolean_Tag,
      Bitfield_Tag,     /* All bitfields are 32 bits  */
      Int_Tag,
      Short_Tag,
      Long_Tag,
      Byte_Tag,
      Word_Tag,
      DWord_Tag,
      Float_Tag,
      DFloat_Tag,
      String_Tag
      } TAG_TYPE;

/*
**  Normal option definition
*/

struct Option_Tag {
      int         letter;                 /* Option switch              */
      Boolean_T   case_sense;             /* TRUE = case sensitive      */
      TAG_TYPE    type;                   /* Type of option             */
      Boolean_T   additive;               /* Cumulative option          */
      void       *buf;                    /* Storage location           */
      char       *min;                    /* Minimum value (string)     */
      char       *max;                    /* Maximum value (string)     */
      char       *Default;                /* Default value (string)     */
};

/*
**  Verbose option definition
*/

struct Voption_tag {
      char       *optstr;                 /* Option string              */
      Boolean_T   case_sense;             /* TRUE = case sensitive      */
      TAG_TYPE    type;                   /* Type of option             */
      Boolean_T   additive;               /* Cumulative option          */
      void       *buf;                    /* Storage location           */
      char       *min;                    /* Minimum value (string)     */
      char       *max;                    /* Maximum value (string)     */
      char       *Default;                /* Default value (string)     */
};

#define MAX_XARGS 512

extern struct Option_Tag options[];

extern int        xargc;
extern char      *xargv[MAX_XARGS];
extern Boolean_T  getopts_range_err;
extern Boolean_T  xargs_on;

extern struct Option_Tag options[];             /* MUST be defined      */
extern struct Voption_tag voptions[];           /* Defined, else NULL   */

int    getopts(int, char *[]);
double getopts_eval(char *str);

#if __cplusplus
 }
#endif

#endif /* GETOPTS_H */
