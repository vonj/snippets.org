/*
   --------------------------------------------------------------------
   Module:     REPLACE.C
   Author:     Gilles Kohl
   Started:    09.06.1992   12:16:47
   Modified:   09.06.1992   12:41:41
               22-Sep-95    Bob Stout
   Subject:    Replace one string by another in a given buffer.
               This code is public domain. Use freely.
   --------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snip_str.h"

/*
** strrepl: Replace OldStr by NewStr in string Str contained in buffer
**          of size BufSiz.
**
** Str should have enough allocated space for the replacement - if not,
** NULL is returned. Str and OldStr/NewStr should not overlap.
**
** The empty string ("") is found at the beginning of every string.
**
** Returns: pointer to first location behind where NewStr was inserted.
**          Str if OldStr was not found.
**          NULL if replacement would overflow Str's buffer
**
** This is useful for multiple replacements, see example in main() below
** (be careful not to replace the empty string this way !)
**
**  NOTE: The name of this funtion violates ANSI/ISO 9899:1990 sec. 7.1.3,
**        but this violation seems preferable to either violating sec. 7.13.8
**        or coming up with some hideous mixed-case or underscore infested
**        naming. Also, many SNIPPETS str---() functions duplicate existing
**        functions which are supported by various vendors, so the naming
**        violation may be required for portability.
*/

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

char *strrepl(char *Str, size_t BufSiz, char *OldStr, char *NewStr)
{
      int OldLen, NewLen;
      char *p, *q;

      if(NULL == (p = strstr(Str, OldStr)))
            return Str;
      OldLen = strlen(OldStr);
      NewLen = strlen(NewStr);
      if ((strlen(Str) + NewLen - OldLen + 1) > BufSiz)
            return NULL;
      memmove(q = p+NewLen, p+OldLen, strlen(p+OldLen)+1);
      memcpy(p, NewStr, NewLen);
      return q;
}

#if defined(__cplusplus) && __cplusplus
 }
#endif

#ifdef TEST

/*
** Test main().
** Given two arguments, replaces the first arg. in the lines read from
** stdin by the second one.
** Example invocation:
** replace printf puts <replace.c
** will replace all printf's by puts in replace's source.
**
*/

int main(int argc, char *argv[])
{
      char buf[200];
      char *Start, *Str;
      size_t BufSiz, BufLeft;

      if(argc < 3)
      {
            puts("Usage: STRREPL old_string new_string [buffer_size]");
            return EXIT_FAILURE;
      }
      if (argc > 3)
      {
            BufSiz = atoi(argv[3]);
            if (200 < BufSiz)
                  BufSiz = 200;
      }
      else  BufSiz = 20;            /* Pretend we have a short buffer   */

      /* Repeat until all occurrences replaced */

      while(NULL != (Start = fgets(buf, 200, stdin)))
      {
            if ('\n' == LAST_CHAR(Start))
                  LAST_CHAR(Start) = NUL;       /* Strip trailing \n    */
            for (Str = Start, BufLeft = BufSiz; Start != NULL; )
            {
                  Start = strrepl(Start, BufLeft, argv[1], argv[2]);
                  if (Start == Str)
                        break;
                  BufLeft -= Start - Str;
                  Str = Start;
            }
                  
            if (NULL == Start)
                  puts("\a*** Modified string will not fit in buffer ***");
            else  printf("%s\n", buf);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */

