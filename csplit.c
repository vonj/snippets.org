/*
 * Donated to public domain
 *
 * Designation:  CSplit
 *
 * Description:  This program is used to process source files for
 *               the purpose of transmission through a FidoNet (tm)
 *               echo in such a manner as to circumvent over-size
 *               message problems.
 *
 *  This program implements the following specifications:
 *
 *  1) a. Combine multiple source files.
 *     b. Split source into 90 line sections.
 *     c. Add header and trailer marker lines delimiting each
 *        section and file.
 *  2) a. Delete any trailing whitespace.
 *     b. Replace tabs with spaces honoring tabstop boundaries.
 *     c. Default to 4 columns per tabstop.
 *     d. Allow user to specify alternate tabstop column number.
 *  3) a. Wrap lines longer than 75 characters long using the C
 *        "\ at the end of a line" notation (using space break).
 *     b. Distinguish wrapped lines from user-continued lines by
 *        inserting a line with a single "\" character between the
 *        two lines that contain the wrapped text.
 *  4) a. Calculate a CRC for each section and include it in the
 *        section trailer marker lines.
 *  5) a. Provide a help display for program usage when the
 *        program is executed without parameters.
 *  6) a. Provide as detailed of explanation as possible when
 *        an unexpected condition occurs.
 *     b. Attempt to continue execution for all but the most severe
 *        errors.
 *
 *
 * Syntax:
 *
 * Split:     CSPLIT  [/tn] [/wn] [/ln] [/sc]  outfile  src.ext [ ... ]
 *
|* Extract:   CSPLIT  /x  infile  [ ... ]
 *
 * Where:      /t n - For TAB character expansion, the number of columns
 *                    for each TAB stop.  (defaults to every 4 columns)
 *             /w n - For width control, the column number to be used for
 *                    breaking long lines.  (the default is column 75)
 *             /l n - For length control, the number of lines to limit
|*                    each section or 0 for no split.  (default is 90)
 *             /s c - Use 'c' as an alternate line separator character
 *                    instead of the default separator character, '>'.
|*                    Ignored for extraction - matches separator found
|*                    in extract file.  However, the extract file must
|*                    only use one separator character.
 *           infile - Input file name.  An extension indicates that the
 *                    file contains the sections in proper consecutive
|*                    order ready for extraction.  Otherwise, infile.001,
|*                    infile.002, etc., will be used.
|*          outfile - Name of the output file(s).  The extension will
|*                    be ignored if specified and each output file will
|*                    be named such that the section number will be the
|*                    extension (e.g., outfile.001, outfile.002, etc..)
 *          src.ext - The first source file..etc  Wildcard filespecs are
 *                    supported only under non-ANSI compiling conditions.
 *
|* Notes:  Paths are supported for all filenames, however, the paths
|*         are not preserved internally during the split operation.
|*         The extraction process will therefore only create files in
|*         the current directory.
 *
 * Revision History:  ( thanks to all the contributors )
 *
 * 08/31/93  Fred Cole  Original draft
 * 09/05/93  Fred Cole  Added CRC calculation and extraction ability.
 *                      Fixed a line wrap problem.
 * 09/14/93  Fred Cole  Added conditional compilation directives to
 *                      allow non-ANSI filespec support.  Squashed an
 *                      extract() function bug.
 * 11/21/93  Thad Smith Test for incomplete input file on extraction.
 *                      Remove spaces added in message transmission.
 *                      Default to 90 lines/section.  Fix tab expansion.
 * 12/03/93  Fred Cole  Fixed a cleanup() function bug.
 * 12/09/93  Keith Campbell / TS
 *                      Fixed bug with options preceded by '-' and fixed
 *                      tempfile opening bug.
 * 01/02/94  David Nugent / FC
 *                      Additions for findfirst/findnext support for
 *                      MSC6 (& 7) and OS/2 in initlist() routine and
 *                      portable.h header file.
 * 01/02/94  Auke Reitsma / FC
 *                      Increased number of chars read in to prevent line
 *                      numbers from becoming out-of-sync with input.
 * 01/12/94  Chad Wagner / FC
 *                      Correction to initlist() function to eliminate
 *                      redundant line increment.
 *--- v2.0 --------------------------------------------------------------
 * 07/23/94  Keith Campbell / FC
 *                      Modified to not abort extraction when a CRC
 *                      mismatch occurs - just issue warning message.
 * 07/30/94  Auke Reitsma / FC
 *                      Added multiple file extraction ability.
 * 09/17/94  Keith Campbell / FC
 *                      Added version separator lines.
 * 10/28/94  Bob Stout / FC
 *                      Added separator character, width and length
 *                      command line options.
 * 12/18/94  Fred Cole  Revised code to facilitate maintenance.
 * 12/27/94  Fred Cole  Limited the minimum width for breaking long
 *                      lines to column 55 since this is the length
 *                      of the longest separator line.
 * 01/15/95  David Gersic / FC
 *                      Modified the line wrap logic to handle long
 *                      sequences of characters lacking whitespace.
 *--- v2.1 --------------------------------------------------------------
 * 10/30/95  Phi Nguyen / FC
 *                      Added file extraction messages.
 * 10/31/95  Fred Cole  Added ability to extract unconcatenated files.
 *                      Added path support except for extracted files
 *                      ( i.e., paths are not preserved internally ).
 * 11/06/95  Fred Cole  Corrected tabstop calculation on wrapped lines.
 * 11/07/95  Fred Cole  Increased max section length to SHRT_MAX lines.
 * 11/08/95  Bob Stout / FC
 *                      Disable the split logic when a 0 section length
 *                      is specified with the /L command line option.
 *--- v2.2 --------------------------------------------------------------
 * 11/22/95  Fred Cole  Modified logic to ignore leading whitespace added
 *                      to separator lines ( mail reader quoting? ).
 * 11/22/95  Doug Nazar (DN2) / FC
 *                      Modified sscanf() format specifiers to correctly
 *                      convert CRC values for 32 bit platforms.
 * 11/22/95  Fred Cole  Changed TRUE/FALSE enum to macros to accommodate
 *                      systems where these identifiers already exist.
 * 11/29/95  Bob Stout / FC
 *                      Added unsigned casts to allow signed 'length' to
 *                      be compared to SHRT_MAX on 16-bit platforms.
 * 11/29/95  Doug Nazar (DN2) / FC
 *                      Added setbuf() statement to unbuffer stdout.
 * 06/02/96  Fred Cole  Renamed TRUE/FALSE macros to avoid possible
 *                      identifier conflicts.
 * 06/02/96  Darin McBride / FC
 *                      Modified logic to allow source files located on
 *                      on another drive to be processed.  This change
 *                      assumes that ':' is not a valid character for a
 *                      file name.
 * 06/02/96  Fred Cole  Corrected error in extract logic when file does
 *                      not exist.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "csplit.h"

char tempfile[MAXFSPEC + 1]; /* necessary evils - global variables */
FILE *finp = NULL;
FILE *fout = NULL;
FILE *ftmp = NULL;
SLST *head = NULL;
SLST *cur  = NULL;

int main (int argc, char *argv[])
{
  char     sepchar = SEP_CDEF;
  char     outfile[MAXFSPEC + 1];
  char    *sptr    = 0;
  int      argndx  = 1;
  int      chr     = 0;
  int      extract = B_FALSE;
  int      length  = LENDEF;
  int      retc    = 0;
  int      tabstop = TABDEF;
  int      width   = WIDDEF;


  setbuf (stdout, 0);                  /* DN2: buffered output fix */
  printf ("\nCSplit %s  (pd) 1993-1996 by Fred Cole\n", VERSION);
  printf ("This executable program is public domain.\n\n");

  if (1 == argc)
  {
    disp_help ();
    return SYNTAX;
  }

  while (('/' == argv[argndx][0]) || ('-' == argv[argndx][0]))
  {
    chr = toupper (argv[argndx][1]);

    switch (chr)
    {
      case '?':
      case 'H':                   /* /H,/? help option */
        disp_help ();
        return SYNTAX;

      case 'X':                   /* /X extract option */
        extract = B_TRUE;
        break;

      case 'T':                   /* /T tab option */
        tabstop = atoi (&argv[argndx][2]);

        if ((tabstop < TABMIN) || (tabstop > TABMAX))
          printf ("Invalid tab parameter \"%s\" (%d-%d).\n", argv[argndx], TABMIN, TABMAX);

        if (tabstop < TABMIN)
          tabstop = TABMIN;
        else if (tabstop > TABMAX)
          tabstop = TABMAX;

        break;

      case 'W':                   /* /W width option */
        width = atoi (&argv[argndx][2]);

        if ((width < WIDMIN) || (width > WIDMAX))
          printf ("Invalid width parameter \"%s\" (%d-%d).\n", argv[argndx], WIDMIN, WIDMAX);

        if (width < WIDMIN)
          width = WIDMIN;
        else if (width > WIDMAX)
          width = WIDMAX;

        break;

      case 'L':                   /* /L length option */
        if (('0' == argv[argndx][2]) && ('\0' == argv[argndx][3]))
          length = 0;
        else
        {
          length = atoi (&argv[argndx][2]);

          if ((length < LENMIN) || ((unsigned)length > LENMAX))
            printf ("Invalid length parameter \"%s\" (0,%d-%d).\n", argv[argndx], LENMIN, LENMAX);

          if (length < LENMIN)
            length = LENDEF;
          else if ((unsigned)length > LENMAX)
            length = LENMAX;
        }

        break;

      case 'S':                   /* /S separator character option */
        sscanf (&argv[argndx][2], "%c", &sepchar);

        if (0 == isgraph (sepchar))
        {
          printf ("Invalid input parameter \"%s\".\n", argv[argndx]);
          sepchar = SEP_CDEF;
        }
        break;

      default:
        printf ("Ignoring unknown input parameter \"%s\".\n", argv[argndx]);
    }
    ++argndx;
  }

  if (B_TRUE == extract)
  {
    if (argndx == argc)
    {
      printf ("No file argument specified for extraction.\n");
      return SYNTAX;
    }

    /* AR: handle multiple files; break on error */
    for ( ; argndx < argc; ++argndx)
      if (NOERR != (retc = extr_file (argv[argndx], sepchar)))
        break;

    cleanup ();
    return retc;
  }

  if ((argc - argndx) < 2)
  {
    printf ("Missing input and/or output file name arguments.\n");
    disp_help ();
    return SYNTAX;
  }

  if (NULL != (sptr = strrchr (argv[argndx], '\\')))  /* ignore path */
  {
    if (NULL != (sptr = strchr (sptr, '.')))
      *sptr = '\0';                              /* truncate any ext */
  }
  else if (NULL != (sptr = strchr (argv[argndx], '.')))
  {
    *sptr = '\0';                                /* truncate any ext */
  }

  if (strlen (argv[argndx]) > MAXFSPEC)
  {
    printf ("Output file name argument too long.\n");
    return SYNTAX;
  }

  strncpy (outfile, argv[argndx], MAXFSPEC);
  outfile[MAXFSPEC] = '\0';                      /* ensure termination */

  if (NOERR != (retc = init_list (argc, argv, ++argndx)))
  {
    cleanup ();
    return retc;
  }

  retc = split_src (head, outfile, length, width, tabstop, sepchar);
  cleanup ();
  return retc;
}

/*
 * add_list - Add a file name to linked list of files to be processed.
 */
SLST *add_list (char *fname)
{
  SLST *new = NULL;

  if (NULL == (new = (SLST *)malloc (sizeof(SLST))))
  {
    puts ("Error allocating memory.\n");
  }
  else
  {
    strcpy (new->srcfile, fname);
    new->next = NULL;

    if (NULL == cur)
      head = new;
    else
      cur->next = new;
  }
  cur = new;
  return cur;
}

/*
 * cleanup - Just a convenient way to provide centralized housekeeping.
 */
void cleanup (void)
{
  free_list ();

  if (NULL != finp)  fclose (finp);
  if (NULL != fout)  fclose (fout);
  if (NULL != ftmp)  fclose (ftmp);

  /* Now, does it really exist? */
  if (NULL != (ftmp = fopen (tempfile, "r")))
  {
    fclose (ftmp);
    remove (tempfile);
  }
}

/*
 * csp_fgets - A custom fgets() function that expands
 *             tabs, deletes trailing whitespace and
 *             performs line wrapping.
 */
char *csp_fgets (char *s, int len, FILE * fp, int tabstop)
{
  static char  sbuf[LLENMAX * 2]; /* big enough for TAB expansion */
  static char *beg  = sbuf;
  static int   tofs = 0;
  static int   wrap = B_FALSE;

  char *e = 0;
  char *w = 0;
  char *p = s;
  char *q = 0;

  int   ch     = 0;
  int   cnt    = 0;
  int   i      = 0;
  int   spaces = 0;

  if (B_TRUE == wrap)             /* if line wrap */
  {
    tofs += (int)(beg - sbuf);    /* adj. TAB column offset */
    strcpy (s, "\\\n");
    memmove (sbuf, beg, strlen (beg) + 1); /* DG: Modification for  */
    beg  = sbuf;                           /* DG: long lines w/o WS */
    wrap = B_FALSE;
    return s;
  }

  while ((cnt < len-1) && ('\n' != ch))
  {                                    /* get next char from buffer */
    if (0 == (ch = *beg++))            /* if buffer empty */
    {
      memset (sbuf, 0, sizeof (sbuf));
      beg = fgets (sbuf, LLENMAX, fp); /* grab another string */

      if (NULL == beg)                 /* if end of file... */
      {
        beg = sbuf;
        *beg = 0;

        if (0 == cnt)
          return NULL;                 /*  and buffer empty */
      }
      else
      {
        w = e = &sbuf[i = strlen (sbuf)]; /* find 1st trailing ws char */

        while ((w > sbuf) && (isspace (*(w - 1))))
          --w;

        if (('\n' == *(e - 1)) || /* if terminated w/newline char */
            (i < (len-1)))        /* or unterminated short line */
        {
          *w++ = '\n';            /* terminate with newline char */
        }

        *w = 0;
        ch = *beg++;
      }
    }

    if (ch == '\t')               /* if TAB character */
    {                             /* space to next tab stop */
      /*
       * TS: The following code has been changed to pad to the next
       * tab stop, rather than insert a fixed number of spaces.
       */
      spaces = tabstop - (((int)(tofs + beg - sbuf) - 1) % tabstop);
      memmove (beg + spaces - 1, beg, strlen (beg) + 1);

      for (q = beg - 1; spaces > 0; --spaces)
        *q++ = ' ';

      ch = ' ';                   /* change TAB to space */
    }

    *p++ = (char)ch;              /* update output string */
    ++cnt;

    if ((cnt == len - 1) && ('\n' != ch))   /* if need to wrap line */
    {
      beg -= 2;                   /* make room for "\\\n" characters */
      e = beg;
      p -= 2;
      q = p;

      /* unget characters to 1st previous space */
      while ((e > sbuf) && (' ' != *(e - 1)))
      {
        --q;
        --e;
      }

      /*if (((beg - e) < len) && (e != sbuf))*/
      if (e != sbuf)              /* if wrap on space char */
      {                           /* ( else wrap asis ) */
        p = q;
        beg = e;
      }

      *p++ = '\\';                /* terminate current line */
      *p++ = '\n';
      wrap = B_TRUE;              /* flag wrap line pending */
    }
  } /*end_while*/

  if ('\n' == ch)
    tofs = 0;

  *p = 0;
  return s;
}

/*
 * disp_help - Display help screen to user.
 */
void disp_help (void)
{
  puts ("This utility processes C/C++ source files for the purpose of transmission");
  puts ("through a FidoNet (tm) echo so as to circumvent oversize message problems.\n");
  puts ("Split:    CSplit  [ [/tn] [/wn] [/ln] [/sc] ]  outfile  srcfile [ ... ]");
  puts ("Extract:  CSplit  /x  infile  [ ... ]\n");
  puts ("Where:      /t n - For TAB character expansion, the number of columns");
printf ("                   for each TAB stop.  (min/default/max: %d/%d/%d)\n", TABMIN, TABDEF, TABMAX);
  puts ("            /w n - For width control, the column number to be used for");
printf ("                   breaking long lines.  (min/default/max: %d/%d/%d)\n", WIDMIN, WIDDEF, WIDMAX);
  puts ("            /l n - For length control, the number of lines to limit");
printf ("                   each section.  (0 disable, min/default/max: %d/%d/%d)\n", LENMIN, LENDEF, LENMAX);
  puts ("            /s c - Use 'c' as the line separator character instead");
printf ("                   of the default separator character, '%c'.\n", SEP_CDEF);
  puts ("         outfile - Output file name.  The extension will be the");
  puts ("                   sequential section number.");
#if defined(__STDC__)
  puts ("         srcfile - Source file(s)  (no wildcard support)");
#else
  puts ("         srcfile - Source file(s)  (wildcards supported)");
#endif
  puts ("          infile - Input file name.  An extension indicates file contains the");
  puts ("                   sections in proper consecutive order ready for extraction.");
  puts ("                   Otherwise, infile.001, infile.002, etc., will be used.");
}

/*
 * extr_file - This function processes a properly concatenated file
 *             of consequtive sections to extract the original source.
 */
int extr_file (char *infile, char sepchar)
{
  static char  line [WIDMAX * 2];
  static char  line2[WIDMAX * 2];

  char         outfile[MAXFSPEC + 1];
  char         sep_ln[32];
  char        *sptr = 0;
  char         tchar      = 0;

  int          curpart    = 0;
  int          found      = B_FALSE;
  int          indx       = 0;
  int          in_section = B_FALSE;
  int          len        = 0;
  int          key        = 0;
  int          lines      = 0;
  int          maxpart    = 0;
  int          pos_wrap   = B_FALSE;
  int          sepmax     = 0;
  int          seppart    = 0;
  int          sep_id_len = 0;
  int          temp       = 0;

  unsigned short crc = 0;
  unsigned short sepcrc = 0;


  for (temp = 0; temp < SEP_CLEN; ++temp)
    sep_ln[temp] = (char)sepchar;

  sep_ln[temp++] = ' ';
  strcpy (&sep_ln[temp], SEP_ID);
  sep_id_len = strlen (sep_ln);

  if (NULL == (finp = fopen (infile, "r")))
  {
    char fname[MAXFSPEC + 1];

    if ((NULL == (sptr = strrchr (infile, '\\'))) && /* ignore path */
        (NULL == (sptr = strrchr (infile, ':'))))
    {
      sptr = infile;                        /* V2.2#3 correction */
    }

    if (NULL != strchr (sptr, '.'))         /* if extension exists */
    {
      printf ("Error opening input file for extraction.\n");
      return FILEIO;
    }

    if ((strlen (infile) + 4) > MAXFSPEC)   /* if file spec too large */
    {
      printf ("Input file name argument too long.\n");
      return SYNTAX;
    }

    indx = 1;
    sprintf (fname, "%s.%03d", infile, indx);

    if (NULL != (finp = fopen (fname, "r")))
    {
      sprintf (tempfile, "%s.CSP", infile);

      if (NULL == (ftmp = fopen (tempfile, "w")))
      {
        printf ("Unable to open \"%s\" temp file.\n", tempfile);
        return FILEIO;
      }

      printf ("Processing input files: %s ...\n", fname);

      do
      {
        while (NULL != fgets (line, sizeof (line), finp))
          fputs (line, ftmp);

        fclose (finp);
        ++indx;
        sprintf (fname, "%s.%03d", infile, indx);
        finp = fopen (fname, "r");

      } while (NULL != finp);

      fclose (ftmp);

      if (NULL == (finp = fopen (tempfile, "r")))
      {
        printf ("Error opening temp file \"%s\" for extraction.\n", outfile);
        return FILEIO;
      }
    }
    else
    {
      printf ("Error opening input file \"%s\" for extraction.\n", infile);
      return FILEIO;
    }
  }
  else
  {
    printf ("Processing input file: %s\n", infile);
  }

  crc = curpart = maxpart = lines = 0;
  in_section = pos_wrap = B_FALSE;
  fout = NULL;
  initcrctab ();
  *line2 = 0;

  while (NULL != finp)
  {
    /* AR: increased line input size */
    sptr = fgets (line, sizeof (line), finp);

    if (NULL == sptr)
    {
      if (feof (finp))            /* end of file */
      {
        fclose (finp);
        finp = NULL;
      }
      else
      {
        if (lines)
          printf ("(line %d) ", lines);

        printf ("Unable to read from input file:  %s\n", infile);
        return FILEIO;
      }
    }
    else                          /* process line */
    {
      /* TS: eliminate any added trailing spaces */
      for (indx = strlen (sptr) - 1; indx && (' ' == line[indx - 1]); --indx)
        continue;

      line[indx]   = '\n';
      line[indx+1] = '\0';
      ++lines;

      /*
       * Find 1st separator line to determine if correct separator
       * character is being used and use the one that was found.
       */
      if (!found && (0 != strstr (line, SEP_ID)))
      {
        for (temp = 0; isspace (line[temp]); ++temp)
          ;  /* null statement */

        tchar = line[temp];                 /* skip leading ws */

        if (sepchar != tchar)
          for (temp = 0; temp < SEP_CLEN; ++temp)
            sep_ln[temp] = tchar;

        found = B_TRUE;
      }

      if (0 != (sptr = strstr (line , sep_ln))) /* if separator line */
      {
        sptr += sep_id_len;

        if (sptr == strstr (sptr, SEP_BF))  /* if begin file */
        {
          if (NULL != fout)
          {
            printf ("(line %d) ", lines);
            puts ("Encountered 2nd \"Begin file\" separator");
            puts ("line before expected \"End file\" separator line:");
            puts (line);
            return PROCESS;
          }

          sptr += strlen (SEP_BF);

          if (1 != sscanf (sptr, "%s", outfile))
          {
            printf ("(line %d) ", lines);
            puts ("Unable to parse filename from separator line:");
            puts (line);
            return PROCESS;
          }

          if (NULL != (fout = fopen (outfile, "r")))
          {
            key = 0;
            printf ("\nOutput file already exists:  %s\n", outfile);

            do
            {
              printf ("Overwrite? (y/n) ");
              key = getchar ();
              puts ("");
              temp = key;

              while (temp != '\n')
                temp = getchar();  /* eat any/all extra keystrokes */

              if (('n' == key) || ('N' == key))
                return ABORT;

            } while (('y' != key) && ('Y' != key));

            if (NULL == freopen (outfile, "w", fout))
            {
              printf ("Unable to open file for output:  %s\n", outfile);
              return FILEIO;
            }
          }
          else
          {
            if (NULL == (fout = fopen (outfile, "w")))
            {
              printf ("Unable to open file for output:  %s\n", outfile);
              return FILEIO;
            }
          }

          printf ("Extracting file %s\n", outfile);   /* Phi Nguyen */
          crc = updcrc (crc, (unsigned char *)line, strlen (line));
        }
        else if (sptr == strstr (sptr, SEP_EF))  /* if end file */
        {
          if (NULL == fout)
          {
            printf ("(line %d) ", lines);
            puts ("Encountered an \"End file\" separator line");
            puts ("before a \"Begin file\" separator line:");
            puts (line);
            return PROCESS;
          }

          if (fclose (fout))
          {
            printf ("Unable to close output file:  %s\n", outfile);
            return FILEIO;
          }

          fout = NULL;
          crc = updcrc (crc, (unsigned char *)line, strlen (line));
        }
        else if (sptr == strstr (sptr, SEP_BP))  /* if begin part */
        {
          if (B_TRUE == in_section)
          {
            printf ("(line %d) ", lines);
            puts ("Encountered 2nd \"Begin part\" separator");
            puts ("line before expected \"End part\" separator line:");
            puts (line);
            return PROCESS;
          }
          sptr += strlen (SEP_BP);

          if (2 != sscanf (sptr, "%d/%d", &seppart, &sepmax))
          {
            printf ("(line %d) ", lines);
            puts ("Unable to parse \"n/x\" from separator line:");
            puts (line);
            return PROCESS;
          }

          if (0 == maxpart)
            maxpart = sepmax;

          if (curpart+1 != seppart)
          {
            printf ("(line %d) ", lines);
            printf ("Encountered part %d while expecting part %d:\n", seppart, curpart+1);
            puts (line);
            return PROCESS;
          }
          in_section = B_TRUE;
          ++curpart;
        }
        else if (sptr == strstr (sptr, SEP_EP))  /* if end part */
        {
          if (B_FALSE == in_section)
          {
            printf ("(line %d) ", lines);
            puts ("Encountered 2nd \"End part\" separator line");
            puts ("before expected \"Begin part\" separator line:");
            puts (line);
            return PROCESS;
          }
          sptr += strlen (SEP_EP);
          sptr = strstr (sptr, ": ");

          if (1 != sscanf (sptr+2, "%hx", &sepcrc))   /* DN2: 32 bit fix */
          {
            printf ("(line %d) ", lines);
            puts ("Corrupt CRC in separator line:");
            puts (line);
            return PROCESS;
          }

          if (crc != sepcrc)
          {
            printf ("(line %d) ", lines);
            printf ("Calculated CRC mismatch (0x%04x):\n", crc);
            puts (line);           /* KC: report CRC mismatch only */
          }
          crc = 0;
          in_section = B_FALSE;

          if (curpart == maxpart)                /* if finished */
            break;
        }
        else
        {
          if (sptr != strstr (sptr, SEP_VR))     /* if not version */
          {
            printf ("(line %d) ", lines);
            puts ("Unrecognized separator line:");
            puts (line);
            return PROCESS;
          }
        }
      }
      else                        /* else process data line */
      {
        if (B_TRUE == in_section) /* save only file data */
        {
          len = strlen (line);
          crc = updcrc (crc, (unsigned char *)line, len);

          if (B_TRUE == pos_wrap) /* if possible line wrap in progress */
          {
            if (0 == strncmp (line, "\\\n", 2))  /* if wrapped line */
            {
              strcpy (line, line2);
              line[strlen (line) - 2] = 0;  /* remove wrap EOL */
            }
            else
            {
              strcat (line2, line);
              strcpy (line, line2);
            }
            pos_wrap = B_FALSE;
          }
          else  if ('\\' == line[len - 2]) /* if possible wrapped line */
          {
            strcpy (line2, line);
            pos_wrap = B_TRUE;
          }

          if ((B_FALSE == pos_wrap) &&
              ((NULL == fout) || (EOF == fputs (line, fout))))
          {
            puts ("Error writing output\n");
            return FILEIO;
          }
        }
      }
    }
  } /*end_while*/

  /* TS: Test for incompete processing. */
  if (B_TRUE == in_section)
  {
    printf ("Error: end of input while processing section %d of %d\n", seppart, sepmax);
    return PROCESS;
  }

  if (seppart != sepmax)
  {
    printf ("Error: end of input after processing section %d of %d\n", seppart, sepmax);
    return PROCESS;
  }
  return NOERR;
}

/*
 * free_list - This function simply frees each linked list item.
 */
void free_list (void)
{
  while (NULL != head)
  {
    cur = head->next;
    free (head);
    head = cur;
  }
}

/*
 * init_list - This function creates a linked list of input source
 *             files.  Wildcard specifications are accommodated when
 *             ANSI mode is not in effect.
 */
int init_list (int argc, char **argv, int argo)
{
  int i;
#if !defined(__STDC__)
  char         filename[MAXFSPEC + 1];
  char         path[PATHMAX + 1];
  char        *sptr;
  int          done;
  DOSFileData  fd;
#endif

  for (i = argo; i < argc; ++i)             /* process CL arguments */
  {
#if !defined(__STDC__)
    if (strlen (argv[i]) > (MAXFSPEC - FNAMELEN))
    {
      printf ("Input file argument too long:  %s\n", argv[i]);
      return SYNTAX;
    }

    done = FIND_FIRST(argv[i], 0x20, &fd);  /* david nugent */

    if (done)
    {
      printf ("Error with filespec: %s\n", argv[i]);
      return PROCESS;
    }

    strcpy (path, argv[i]);                 /* preserve path */

    if (NULL != (sptr = strrchr (path, '\\')))
      *(sptr + 1) = '\0';
    else
      path[0] = '\0';

    while (!done)
    {
      if ('\0' != path[0])
      {
        strcpy (filename, path);

        if (NULL != (sptr = strrchr (ff_name(&fd), '\\')))
          strcat (filename, sptr + 1);      /* just in case */
        else
          strcat (filename, ff_name(&fd));
      }
      else
        strcpy (filename, ff_name(&fd));

      if (NULL == add_list (filename))      /* david nugent */
        return MEMORY;

      done = FIND_NEXT(&fd);
    }
    FIND_END(&fd);                          /* david nugent */
#else
    if (strlen (argv[i]) > MAXFSPEC)
    {
      printf ("Input file argument too long:  %s\n", argv[i]);
      return SYNTAX;
    }

    if (NULL == add_list (argv[i]))
      return MEMORY;

#endif
  }
  return NOERR;
}

/*
 * split_src - This function takes a linked list of input source
 *             files, concatenates the source and then splits it
 *             into sections of controlled size.
 */
int split_src (SLST *filelist, char *outfname, int length, int width, int tabstop, char sepchar)
{
  char           line[WIDMAX + 1];
  char          *ext     = 0;
  char           filename[MAXFSPEC + 1];
  char           outfile[MAXFSPEC + 1];
  char           sep_ln[32];
  char          *sptr    = 0;
  int            curpart = 0;
  int            key     = 0;
  int            lines   = 0;
  int            maxpart = 1;
  int            retc    = 0;
  int            temp    = 0;
  unsigned short crc     = 0;


  strcpy (outfile, outfname);

  if (NULL == (ext = strchr (outfile, '.')))     /* ignore any ext */
    ext = &outfile[strlen (outfile)];

  *ext = 0;                                      /* make temp file name */
  strcpy (tempfile, outfile);
  strcat (tempfile, ".CSP");

  if (NULL == (ftmp = fopen (tempfile, "w")))
  {
    printf ("Error creating temp file:  %s\n", tempfile);
    return FILEIO;
  }

  for (temp = 0; temp < 10; ++temp)
    sep_ln[temp] = (char)sepchar;

  sep_ln[temp] = '\0';

  for (cur = filelist, lines = 0; NULL != cur; cur = cur->next)
  {
    if (NULL == finp)
    {
      if (NULL == (finp = fopen (cur->srcfile, "r")))
      {
        printf ("Error opening source file:  %s\n", cur->srcfile);
        return FILEIO;
      }

      if ((NULL != (sptr = strrchr (cur->srcfile, '\\'))) ||
          (NULL != (sptr = strrchr (cur->srcfile, ':')))) /* Darin McBride */
        strcpy (filename, sptr+1);
      else
        strcpy (filename, cur->srcfile);

      retc = fprintf (ftmp, "%s %s%s%s %s\n", sep_ln, SEP_ID, SEP_BF, filename, sep_ln);

      if (0 == retc)
      {
        puts ("Error writing output\n");
        return FILEIO;
      }
      ++lines;
    }

    while (NULL != finp)
    {
      /*
       * The function csp_fgets() is equivalent to fgets() in that it
       * too reads n-1 characters or up to a newline character.  This
       * function additionally expands TAB characters, deletes trailing
       * whitespace and wraps lines exceeding the specified length.
       */
      if (NULL == csp_fgets (line, width, finp, tabstop))
      {
        if (feof (finp))
        {
          fclose (finp);
          finp = NULL;
          retc = fprintf (ftmp, "%s %s%s%s %s\n", sep_ln, SEP_ID, SEP_EF, filename, sep_ln);

          if (0 == retc)
          {
            puts ("Error writing output\n");
            return FILEIO;
          }
          ++lines;      /* adjust line count */
        }
        else
        {
          puts ("Error reading input\n");
          return FILEIO;
        }
      }
      else
      {
        if (EOF == fputs (line, ftmp))
        {
          puts ("Error writing output\n");
          return FILEIO;
        }
        ++lines;
      }
    } /*end_while*/
  } /*end_for*/

  if (0 != length)
  {
    /* There are 3 lines of overhead per section. */
    maxpart = lines / (length - 3);

    if ((lines % (length - 3)) > 0)
      ++maxpart;                       /* for partial section */
  }

  curpart = 1;
  sprintf (ext, ".%03d", curpart);     /* make 1st output file name */

  /* warn user if the output filename is already in use */
  if (NULL != (fout = fopen (outfile, "r")))
  {
    key = 0;
    printf ("Output file already exists:  %s\n", outfile);

    do
    {
      printf ("Overwrite? (y/n): ");
      key = (toupper) (getchar ());    /* prevent using toupper macro */
      puts ("");
      temp = key;

      while (temp != '\n')
      {
        temp = getchar ();             /* eat all extra keystrokes */
      }

      if ('N' == key)
        return ABORT;

    } while ('Y' != key);

    fclose (fout);
    fout = NULL;
  }

  if (NULL == freopen (tempfile, "r", ftmp))
  {
    printf ("Error reopening temp file:  %s\n", tempfile);
    return FILEIO;
  }

  initcrctab ();

  while (NULL != ftmp)
  {
    lines = 0;

    if (NULL == fout)
    {
      sprintf (ext, ".%03d", curpart); /* make output file name */

      if (NULL == (fout = fopen (outfile, "w")))
      {
        printf ("Error opening output file:  %s\n", outfile);
        return FILEIO;
      }

      retc = fprintf (fout, "%s %s%s%s %s\n", sep_ln, SEP_ID, SEP_VR, VERSION, sep_ln);

      if (0 == retc)
      {
        puts ("Error writing output\n");
        return FILEIO;
      }

      ++lines;
      retc = fprintf (fout, "%s %s%s%d/%d %s\n", sep_ln, SEP_ID, SEP_BP, curpart, maxpart, sep_ln);

      if (0 == retc)
      {
        puts ("Error writing output\n");
        return FILEIO;
      }

      ++lines;
    }

    crc = 0;

    while (((0 == length ) || (lines < (length - 1))) &&
           (NULL != ftmp))
    {
      if (NULL == fgets (line, WIDMAX, ftmp))
      {
        if (feof (ftmp))
        {
          fclose (ftmp);
          ftmp = NULL;
        }
        else
        {
          puts ("Error reading input\n");
          return FILEIO;
        }
      }
      else
      {
        crc = updcrc (crc, (unsigned char *)line, strlen (line));

        if (EOF == fputs (line, fout))
        {
          puts ("Error writing output\n");
          return FILEIO;
        }

        ++lines; /* increment line count */
      }
    } /*end_while*/

    if (0 == fprintf (fout, "%s %s%s%d/%d crc: %04x %s\n", sep_ln, SEP_ID, SEP_EP, curpart, maxpart, crc, sep_ln))
    {
      puts ("Error writing output\n");
      return FILEIO;
    }

    fclose (fout);
    fout = NULL;
    ++curpart;
  } /*end_while*/

  return NOERR;
}


/*
 * CRC-16f.c, from Snippets.  Calculate, intelligently, the CRC
 * of a dataset incrementally given a buffer full at a time.
 * Initialize crc to 0 for XMODEM, -1 for CCITT.
 */

/*
 * P, the CRC polynomial, is used by XMODEM (almost CCITT).
 * If you change P, you must change crctab[]'s initial value
 * to what is printed by initcrctab().
 */
#define  P 0x1021
#define  W 16       /* number of bits in CRC: don't change it */
#define  B 8        /* number of bits per char: don't change it */

unsigned short crctab[1<<B];

void initcrctab (void)
{
  register b, v, i;

  for (b = 0; b <= (1<<B) - 1; ++b)
  {
    for (v = b<<(W-B), i = B; --i >= 0; )
    {
      v = v & 0x8000 ? (v<<1)^P : v<<1;
    }

    crctab[b] = v;
  }
}

unsigned short updcrc (unsigned short icrc, unsigned char *icp, unsigned int icnt)
{
  register unsigned short crc = icrc;
  register unsigned char  *cp = icp;
  register unsigned int   cnt = icnt;

  while (cnt--)
  {
    crc = (crc<<B) ^ crctab[(crc>>(W-B)) ^ *cp++];
  }

  return crc;
}

