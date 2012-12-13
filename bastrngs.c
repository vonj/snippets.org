/*
**  BASIC-like string operations
**
**  public domain by Bob Stout
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <assert.h>
#include "sniptype.h"
#include "bastrngs.h"

static int stralloc_ptr;
static char *strings[8];
static int str_tag[8];

/*
**  stralloc() is the key function in this package, maintaining a pool of
**  reusable strings.
*/

char *stralloc(size_t length)
{
      register int i;

      if (UINT_MAX == length)       /* Assume size_t == unsigned int    */
            return NULL;

      i = stralloc_ptr++;
      ++length;                     /* Allow for terminating NUL        */

      if ((!strings[i]) || (length > strlen(strings[i])))
      {
            strings[i] = (char *)realloc(strings[i], length);
            assert(NULL != strings[i]);
            str_tag[i] = -1;
      }
      else  str_tag[i] = 0;
      stralloc_ptr &= 7;
      return (strings[i]);
      /* Maintains 8 strings in a circular buffer */
}

/*
**  free the string pool.
*/

void str_free(char *string)
{
      register int i;

      for (i = 0; i < 8; ++i)
      {
            if (strings[i] == string)
            {
                  if (str_tag[i])
                        free(strings[i]);
                  return;
            }
      }
}

/*
**  return the leftmost N characters from a string, equivalent to LEFT$
*/

char *left(char *string, size_t N)
{
      char *buf;
      size_t strlength = strlen(string);

      if (N > strlength)
            N = strlength;
      buf = stralloc(N);
      memcpy(buf, string, N);
      buf[N] = NUL;
      return buf;
}

/*
**  return the rightmost N characters from a string, equivalent to RIGHT$
*/

char *right(char *string, size_t N)
{
      char *buf;
      size_t strlength = strlen(string);

      if (N > strlength)
            N = strlength;
      buf = stralloc(N);
      strcpy(buf, &string[strlength-N]);
      return buf;
}

/*
**  return a substring, N characters long beginning at position M,
**  equivalent to MID$
*/

char *mid(char *string, size_t M, size_t N)
{
      char *buf;
      size_t strlength = strlen(string);

      if (M > strlength)
            return NULL;
      if (N > (strlength - M))
            N = strlength - M;
      buf = stralloc(N);
      memcpy(buf, &string[M-1], N);
      buf[N] = NUL;
      return buf;
}

/*
**  string concatenation function, equivalent to A$=B$+C$+...
*/

char *string_add(char *string, ...)
{
      va_list arg_ptr;
      char *temp1, *temp2, *buf;

      va_start(arg_ptr, string);
      temp1 = string;
      do
      {
            if(NULL == (temp2 = va_arg(arg_ptr, char *)))
                  break;
            buf = stralloc(strlen(temp1) + strlen(temp2));
            temp1 = strcat(strcpy(buf, temp1), temp2);
      } while (NULL != temp2);
      return temp1;
}

/*
**  create a string of repeated characters, equivalent to STRING$()
*/

char *string(int ch, size_t N)
{
      char *buf;

      if (N)
      {
            buf = stralloc(N);
            memset(buf, ch, N);
      }
      buf[N] = NUL;
      return buf;
                  
}

/*
**  BASIC INSTR$() work alike - returns position (1-based) of findstr in
**  string, starting search at position start_pos (also 1-based).
**
**  Function suggested by Tika Carr
*/

unsigned int instr(const unsigned int start_pos,
                   const char        *string,
                   const char        *findstr)
{
      char *p;
      unsigned int pos;

      if (start_pos > strlen(string))
            return 0;
      else  pos = start_pos - 1;    /* BASIC offsets are one-based, not
                                       zero-based as in C               */

      if (NULL != (p = strstr(string + pos, findstr)))
            return (int)(p - (char *)string) + 1; /* Position 0 = position 1 */
      else  return 0;
}

#ifdef TEST

/*
**  Demo main()
*/

main()
{
      char *x = "European", *y = "Hardware", *z = "Skaters", *q;
      char *a = "This is a test", *b = "is" ,
           *c = "\nSearching for \"%s\" in \"%s\" starting at position %d\n";
      unsigned int i, pos;

      z = string_add(left(x, 2), right(y, 2), mid(z, 2, 2), "!", NULL);
      q = string('!', 4);
      printf("%s%s\n", z, q);

      for (i = 2; i < strlen(a); i+= 2)
      {
            printf(c, b, a, i);
            if (0 != (pos = instr(i, a, b)))
                  printf("Found at position %d\n", pos);
            else  puts("Not found");
      }

      return 0;
}

#endif /* TEST */
