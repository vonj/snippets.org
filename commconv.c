/*
 * COMMCONV.C
 * Change C++ comments to C comments
 *
 * ver 2.2, 14 Jun 1995
 *   - changed multiline C++ comment handling.
 *
 * ver 2.3, 8 Nov 1995
 *   - bug: if file was ended with // comment with no CR/LF,
 *          program jammed in an endless loop.
 *
 * ver 2.4, 21 Nov 1995
 *   - bug: did not handle escape character \" inside strings.
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
       got_endl      = 0,
       cpp_comment   = 0,
       c_comment     = 0,
       in_string     = 0,
       cpp_multiline = 0;
  char CannotOpen[] = "Cannot open %s\n\n";
  FILE *InFile, *OutFile = stdout;

  if (argc < 2)
  {
    fprintf (stderr, "USAGE: COMMCONV InFile [OutFile]\n");
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
    fputc (Char, OutFile);

    if (in_string && Char == '\\')
    {
      Char2 = fgetc (InFile);
      fputc (Char2, OutFile);

      /* do we have an excape character \" inside string ? */
      if (Char2 == '\"')
        continue;
    }

    if (Char == '\"')
    {
      Char2 = fgetc (InFile);            /* check next char */

      /* do we have a character constant '\"' or an empty string "" ? */
      if (Char2 != '\'' && Char2 != '\"')
        in_string = ! in_string;         /* if not, we are in a string */
      fputc (Char2, OutFile);
    }

    if (in_string)                       /* we are in a string now */
      continue;

    if (Char == '/')
    {
      Char = fgetc (InFile);             /* check next char */
      if (Char == '/')                   /* is it start of C++ comment? */
      {
        Char = '*';                      /* change it to C comment */
        cpp_comment = 1;
      }
      else if (Char == '*')              /* is it start of C comment? */
        c_comment = 1;

      fputc (Char, OutFile);

    }
    else if (Char == '*' && c_comment)
    {
      Char = fgetc (InFile);
      if (Char == '/')                   /* is it end of C comment? */
        c_comment = 0;
      fputc (Char, OutFile);
    }

    if (c_comment || cpp_comment)        /* are we inside C or C++ comment? */
    {
      got_endl = 1;
      while ((Char = fgetc (InFile)) != '\n') /* check the rest of the line */
      {
        if (Char == EOF)
          break;

        if (cpp_multiline)
        {
          if (Char != ' ' && Char != '\t') /* if not white space => */
            cpp_multiline = 0;             /* ...not C++ multiline comment */
        }

        if (Char == '\\' && cpp_comment)
          cpp_multiline = 1;

        if (Char == '*')
        {
          Char2 = fgetc (InFile);        /* check next char */
          ungetc (Char2, InFile);        /* put it back to stream */

          if (Char2 == '/')              /* is it end of C comment */
          {
            c_comment = 0;
            /* is it end of C comment inside C++ comment */
            if (cpp_comment)
            {
              fputs ("* ", OutFile);
              Char = fgetc (InFile);
            }
          }
        }
        fputc (Char, OutFile);
        if (c_comment == 0 && cpp_comment == 0)
        {
          got_endl = 0;
          break;          /* break if end of C comment found */
        }
      } /* while */

      if (cpp_comment && cpp_multiline == 0)
      {
        fputs (" */", OutFile);          /* put ending C comment mark */
        cpp_comment = 0;
      }

      /* print endl if we processed whole line */
      if (got_endl)
        fputc ('\n', OutFile);

      /* clear flag for the next round. if it is still clear after
         next C++ comment line is processed, multiline C++ comment
         is ended.
      */
      cpp_multiline = 0;
    }
  } /* while end */

  if (argc == 3)
    fclose (OutFile);
  fclose (InFile);

  fflush (stdout);
  fprintf (stderr, "\nOK!\n");

  return 0;
}
