/*
 * Test file for Comment Utilities.
 *   This file should be compilable before AND after comment
 *   conversion or removal.
 *
 *   Jari Laaksonen
 *   Arkkitehdinkatu 30 A 2
 *   FIN-33720 Tampere
 *   FINLAND
 *   Fidonet:  2:221/360.20
 *   Internet: jla@to.icl.fi
 */

#include <stdio.h>

int main()
{
/*
  comment */ int a; /* comment continues in same line
*/

/* C comment */ int b; // C++ comment

/*
   C comment
*/ int c; // C++ comment

  /**** comment ****/
  //*** comment ****/

  char ch = '\"'; /* double quote but not a start of a string */

  char x1[]       // here we...
    = "";
  char x2[] = ""; /* ...have some... */
  char x3[] = ""  /* ...empty... */ ;
  char x4[] = ""; // ...strings.

  printf ("this is a string");          /* C comment */
  printf ("this is \" another string"); // C++ comment
  printf ("this is \' another string"); // C++ comment
  printf ("yet another \\ string");     // C++ comment

/* C comment in one line */

// C++ comment in one line

/* C comment
   in several
   lines
   printf ("// not a comment");
*/

/* C comment
   in several lines */

// C comment in C++ comment: /* comment */

/* C++ comment in C comment: // comment */

/*
   C++ comment in C comment: // comment
*/

  printf ("this /* is not // a comment * * ! ");
  printf ("this /* is not a comment * * ! ");
  printf ("this // is not a comment * * ! ");

  printf ("this */ is not a comment * * ! ");

// C++ comment
// C++ comment /

// C++ comment in \
   several        \
   lines

/\
/ C++ comment

  a = 0; /\
* C comment */

  /* C       *\
   *         *
  \* comment */

/* C comment \
   C comment */

// char s[] = "string \
               string";

// not a multiline C++ \comment
  b = 0;

// not a multiline C++ \ comment
  c = 0;

  return 0;
}
// end file
