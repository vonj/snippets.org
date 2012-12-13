/*
**  REDIRECT.H - Snippets header I/O redirection functions
*/

#ifndef REDIRECT__H
#define REDIRECT__H

#include <stdio.h>
#include "sniptype.h"
#include "extkword.h"

typedef  struct {
      char  path[FILENAME_MAX];
      int   which;
      int   what;
      int   oldhandle;
      int   flag;
} REDIR_STRUCT;

extern REDIR_STRUCT redir_stdin, redir_stdout, redir_stderr;

void  start_redirect    ( REDIR_STRUCT *s );
void  stderr2stdout     ( REDIR_STRUCT *s );
void  stop_redirect     ( REDIR_STRUCT *s );


#endif /* REDIRECT__H */
