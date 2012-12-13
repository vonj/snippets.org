/*
**  GETOPTS.H
**
**  public domain by Bob Stout
*/

#ifndef GETOPTS__H
#define GETOPTS__H

#include <stdlib.h>                       /* For size_t           */
#include "sniptype.h"

typedef enum {
      Error_Tag = -1,
      Boolean_Tag,
      Word_Tag,
      DWord_Tag,
      Double_Tag,
      String_Tag
      } TAG_TYPE;

struct Option_Tag {
      int         letter;                 /* Option switch        */
      TAG_TYPE    type;                   /* Type of option       */
      void       *buf;                    /* Storage location     */
      size_t      siz;                    /* Size of buffer (used
                                             only for strings     */
};

extern struct Option_Tag options[];
extern int    xargc;
extern char  *xargv[];

int getopts_lite(int, char *[]);

#endif /* GETOPTS__H */
