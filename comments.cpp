/*
 * Test file for REMCMMNT.C and COMMCONV.C
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

  /**** comment ** comment ** comment ** comment ****/
  //*** comment ** comment ** comment ** comment ****/

  char ch = '\"'; /* double quote but not a start of a string */

  char x1[]       // here we...
    = "";
  char x2[] = ""; /* ...have some... */
  char x3[] = ""  /* ...empty... */ ;
  char x4[] = ""; // ...strings.

  printf ("this is a string");          /* C comment */
  printf ("this is \" another string"); // C++ comment
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

// C comment /* in C++ comment */

/* C++ comment // in C comment */

/*
   C++ comment // in C comment
*/

  printf ("this /* is not // a comment * * ! ");
  printf ("this /* is not a comment * * ! ");
  printf ("this // is not a comment * * ! ");
  printf ("this */ is not a comment * * ! ");

// C++ comment in \             
   several        \  
   lines

// C++ comment
// C++ comment /

// not a multiline C++ \comment
  int d;

// not a multiline C++ \ comment
  int e;

/* C comment \          
   C comment */

  return 0;
}
/* end file */
