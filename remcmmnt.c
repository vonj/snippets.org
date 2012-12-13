/*
 * REMCMMNT.C
 * Remove comments from C or C++ source
 *
 * ver 1.1, 14 Jun 1995
 *   - changed multiline C++ comment handling
 *
 * ver 1.2, 8 Nov 1995
 *   - bug: if file was ended with // comment with no CR/LF,
 *          program jammed in an endless loop.
 *
 * ver 1.3, 21 Nov 1995
 *   - bug: did not handle empty strings ""
 *   - bug: did not handle escape character \" inside strings
 *   - bug: did not catch an end of comment that ended with two or more '*'s
 *
 * Public domain by:
 *   Jari Laaksonen
 *   Arkkitehdinkatu 30 A 2
 *   FIN-33720 Tampere
 *   FINLAND
 *
 *   Fidonet : 2:221/360.20
 *   Internet: jla@to.icl.fi
 */

#include <stdio.h>

int main (int argc, char **argv)
{
  int  Char,
       Char2,
       cpp_comment   = 0,
       c_comment     = 0,
       in_string     = 0,
       cpp_multiline = 0;
  char CannotOpen[] = "Cannot open %s\n\n";
  FILE *InFile, *OutFile = stdout;

  if (argc < 2)
  {
    fprintf (stderr, "USAGE: REMCMMNT InFile [OutFile]\n");
    return 1;
  }
  if ((InFile = fopen (argv[1], "r")) == NULL)
  {
    fprintf (stderr, CannotOpen, argv[1]);
    return 2;
  }

  if (argc == 3)
  {
    if ((OutFile = fopen (argv[2], "w")) == NULL)
    {
      fprintf (stderr, CannotOpen, argv[2]);
      OutFile = stdout;  /* if can't open, output goes to stdout instead */
    }
  }

  while ((Char = fgetc (InFile)) != EOF)
  {
    /* do we have excape characters \", \\ etc. inside string ? */
    if (in_string && Char == '\\')
    {
      Char2 = fgetc (InFile);

      fputc (Char,  OutFile);
      fputc (Char2, OutFile);
      continue;
    }

    if (Char == '\"')
    {
      Char2 = fgetc (InFile);

      /* do we have a character constant '\"' or an empty string "" ? */
      if (Char2 != '\'' && Char2 != '\"')
        in_string = ! in_string;    /* if not, we are in a string */

      if (c_comment == 0 && cpp_comment == 0)
      {
        fputc (Char,  OutFile);
        fputc (Char2, OutFile);
        continue;
      }
    }

    if (! in_string)
    {
      if (Char == '/')
      {
        Char2 = fgetc (InFile);     /* check next char */
        if (Char2 == '/')           /* is it start of C++ comment? */
          cpp_comment = 1;
        else if (Char2 == '*')      /* is it start of C comment? */
          c_comment = 1;

        if (c_comment == 0 && cpp_comment == 0)
        {
          fputc (Char,  OutFile);
          fputc (Char2, OutFile);
          continue;
        }
      }
      else if (Char == '*' && c_comment)
      {
        Char2 = fgetc (InFile);
        if (Char2 == '/')                   /* is it end of C comment? */
        {
          c_comment = 0;
          Char = fgetc (InFile);
        }
      }

      if (c_comment || cpp_comment) /* are we inside C or C++ comment? */
      {
        Char = '\n'; /* print newline after comment line is processed */
        while ((Char2 = fgetc (InFile)) != '\n') /* rest of the line */
        {
          if (Char2 == EOF)
            break;

          if (cpp_multiline)
          {
            if (Char2 != ' ' && Char2 != '\t') /* if not white space => */
              cpp_multiline = 0;             /* ...not C++ multiline comment */
          }

          if (Char2 == '\\' && cpp_comment)
            cpp_multiline = 1;

          if (Char2 == '*' && c_comment)
          {
            Char2 = fgetc (InFile); /* check next char */
            if (Char2 == '/')       /* is it end of C comment? */
            {
              c_comment = 0;
              Char = fgetc (InFile);
              break;
            }
            else
              ungetc (Char2, InFile);        /* put it back to stream */
            if (Char2 == '\n')
              break;
          }
        }
        if (cpp_comment && cpp_multiline == 0)
          cpp_comment = 0;
        if (c_comment || cpp_comment)
          fputc (Char,  OutFile);

        /* clear flag for the next round. if it is still clear after
           next C++ comment line is processed, multiline C++ comment
           is ended.
        */
        cpp_multiline = 0;
      }

    }
    if (c_comment == 0 && cpp_comment == 0)
      fputc (Char,  OutFile);

  } /* while end */

  if (argc == 3)
    fclose (OutFile);
  fclose (InFile);

  fflush (stdout);
  fprintf (stderr, "\nOK!\n");

  return 0;
}
