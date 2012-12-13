/*
**  NLCNVRT.C - A utility to convert newlines in text files
**
**  public domain by Bob Stout
**  Mac support by Norman Dodge and Bob Stout
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "sniptype.h"
#include "getopts.h"

char switch_ch;

struct Option_Tag options[MAX_XARGS] = {
      {
            'd',
            True_,
            Boolean_Tag,
            False_,
            &switch_ch,
            "",
            "",
            ""
      },
      {
            'D',
            True_,
            Boolean_Tag,
            False_,
            &switch_ch,
            "",
            "",
            ""
      },
      {
            'u',
            True_,
            Boolean_Tag,
            False_,
            &switch_ch,
            "",
            "",
            ""
      },
      {
            'U',
            True_,
            Boolean_Tag,
            False_,
            &switch_ch,
            "",
            "",
            ""
      },
      {
            'm',
            True_,
            Boolean_Tag,
            False_,
            &switch_ch,
            "",
            "",
            ""
      },
      {
            'M',
            True_,
            Boolean_Tag,
            False_,
            &switch_ch,
            "",
            "",
            ""
      },
};

FILE *infile;
FILE *outfile;

static void usage(void);
static void dosTOunix(void);
static void dosTOmac(void);
static void unixTOdos(void);
static void unixTOmac(void);
static void macTOdos(void);
static void macTOunix(void);
static void Cout(int ch, FILE *outfile);
static void Sout(char *str, FILE *outfile);

void usage(void)
{
      puts("Usage: NLCNVRT -[d | u | m | D | U | M] file1 [...fileN]");
      puts("Switches: -d  Convert to DOS  from Unix - converts LF   => CRLF");
      puts("        : -u  Convert to Unix from DOS  - converts CRLF => LF");
      puts("        : -m  Convert to Mac  from DOS  - converts CRLF => CR");
      puts("        : -D  Convert to DOS  from Mac  - converts CR   => CRLF");
      puts("        : -U  Convert to Unix from Mac  - converts CR   => LF");
      puts("        : -M  Convert to Mac  from Unix - converts LF   => CR");
      exit(EXIT_FAILURE);
}

main(int argc, char *argv[])
{
      int i;
      char outname[L_tmpnam] = "", *p;

      if (argc < 3)
            usage();

      xargs_on = True_;
      getopts(argc, argv);

      for (i = 1; i < xargc; ++i)
      {
            tmpnam(outname);
            if (NULL != (p = strrchr(outname, '\\')))
                  p = strrchr(outname, '\\');
            if (NULL != p)
            {
                  if (*(++p))
                        strcpy(outname, p);
                  else  ErrExit("Bad temporary file name %s", outname);
            }
            puts(xargv[i]);
            infile = cant(xargv[i], "rb");
            outfile = cant(outname, "wb");
            
            switch (argv[1][1])
            {
                  case 'd':
                        unixTOdos();
                        break;
                        
                  case 'D':
                        macTOdos();
                        break;
                        
                  case 'u':
                        dosTOunix();
                        break;
                        
                  case 'U':
                        macTOunix();
                        break;
                        
                  case 'm':
                        dosTOmac();
                        break;
                        
                  case 'M':
                        unixTOmac();
                        break;
                        
                  default:
                        usage();
            }
            fclose(infile);
            fclose(outfile);
            remove(xargv[i]);
            rename(outname, xargv[i]);
      }
      return EXIT_SUCCESS;
}

void unixTOdos(void)
{
      int ch, lastch = 0;

      while (EOF != (ch = fgetc(infile)))
      {
            if ('\n' == ch && '\r' != lastch)
                  fputc('\r', outfile);
            fputc(lastch = ch, outfile);
      }
      if ('\n' != lastch)
            fputs("\r\n", outfile);
}

void dosTOunix(void)
{
      int ch, lastch = 0;

      while (EOF != (ch = fgetc(infile)) && '\x1a' != ch)
      {
            if ('\r' != ch)
                  fputc(lastch = ch, outfile);
      }
      if ('\n' != lastch)
            fputc('\n', outfile);
}

void macTOdos(void)
{
      int ch, lastch = 0;

      while (EOF != (ch = fgetc(infile)))
      {
            fputc(lastch = ch, outfile);
            if ('\r' == ch)
                  fputc('\n', outfile);
      }
      if ('\r' != lastch)
            fputs("\r\n", outfile);
}

static void dosTOmac(void)
{
      int ch, lastch = 0;

      while (EOF != (ch = fgetc(infile)) && '\x1a' != ch)
      {
            if ('\r' != ch)
            {
                  if ('\n' == ch)
                        ch = '\r';
                  fputc(lastch = ch, outfile);
            }
      }
      if ('\r' != lastch)
            fputc('\r', outfile);
}

static void unixTOmac(void)
{
      int ch, lastch = 0;

      while (EOF != (ch = fgetc(infile)))
      {
            if ('\n' == ch)
                  ch = '\r';
            fputc(lastch = ch, outfile);
      }
      if ('\r' != lastch)
            fputc('\r', outfile);
}

static void macTOunix(void)
{
      int ch, lastch = 0;

      while (EOF != (ch = fgetc(infile)))
      {
            if ('\r' == ch)
                  ch = '\n';
            fputc(lastch = ch, outfile);
      }
      if ('\n' != lastch)
            fputc('\n', outfile);
}

void Cout(int ch, FILE *outfile)
{
      if (EOF == fputc(ch, outfile))
            exit(EXIT_FAILURE);
}

void Sout(char *str, FILE *outfile)
{
      if (EOF == fputs(str, outfile))
            exit(EXIT_FAILURE);
}
