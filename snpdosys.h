/*
**  SNPDOSYS.H - Snippets header file for DOS system-level functions
*/

#ifndef SNPDOSYS__H
#define SNPDOSYS__H

#include <stdio.h>
#include "sniptype.h"
#include "extkword.h"

/*
**  Modified 02-Aug-2001 by rbs
*/

/*
**  File: REDIRECT.C
*/

typedef  struct {
      char  path[FILENAME_MAX];     /* File name                              */
      int   which;                  /* File handle to redirect                */
      int   where;                  /* Redirection file handle                */
      int   oldhandle;              /* Saves a duplicate of the original      */
      int   flag;                   /* Non-zero means active                  */
} REDIR_STRUCT;

extern REDIR_STRUCT redir_stdin, redir_stdout, redir_stderr;

void  start_redirect    ( REDIR_STRUCT *s );
void  stderr2stdout     ( REDIR_STRUCT *s );
void  stop_redirect     ( REDIR_STRUCT *s );


#endif /* SNPDOSYS__H */
