/*
      This program is similar to a program of the same name found on UNIX.
      It prints the files named in the command tail with headings
      except as modified below.

      usage: pr [-i -ln -on -pname -tn -wn] file1[ file2 ... filen]
      where:      -i     = accept files from stdin
                  -ln    = set lines per page to n
                  -on    = set page offset to n
                  -pname = output to file <name>
                  -tn    = set tabs to n cols
                  -wn    = set page width to n

      note: the expr 'PAGE(mesg)' found in col 1 will cause a formfeed
                  and the 'mesg' to be included in the title line on this and
                  each subsequent page until EOF or another PAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "getopts.h"                            /* Also in SNIPPETS     */

#define TAB_DEFAULT 4
#define PAGE_LENGTH 60
#define PAGE_OFFSET 0
#define PAGE_WIDTH      80
#define MAX_ARGS    70
#define MAX_FILES 64
#define PATH_LENGTH 63
#define PAGE(head)


int       page_length = PAGE_LENGTH;
int       page_offset = PAGE_OFFSET;
int       page_width  = PAGE_WIDTH;
int       tab_width   = TAB_DEFAULT;
Boolean_T read_stdin  = False_;

char      print_name[FILENAME_MAX] = "PRN";
char      filenames[MAX_FILES] [FILENAME_MAX];

struct Option_Tag options[] = {
      {'I',  False_, Boolean_Tag, &read_stdin  , NULL, NULL, NULL},
      {'L',  False_, Word_Tag,    &page_length , NULL, NULL, NULL},
      {'O',  False_, Word_Tag,    &page_offset , NULL, NULL, NULL},
      {'T',  False_, Word_Tag,    &tab_width   , NULL, NULL, NULL},
      {'W',  False_, Word_Tag,    &page_width  , NULL, NULL, NULL},
      {'P',  False_, String_Tag,   print_name  , NULL, NULL, NULL},
      {'\0', False_, Error_Tag,    NULL        , NULL, NULL, NULL}
};

char title[80];
char Date[20];
char Time[20];
int  ln, pn;

void prt (char fnp[], FILE *lp);
void new_page (char *fnp, FILE *lp);
void indent(FILE *lp);

PAGE (MAIN)
main(int argc, char *argv[])  /* copy file to printer */
{
      FILE *lp;
      int fi;
      int pn;
      char *cp;

      if (argc < 2) /* No args so tell 'em how it works */
      {
            fprintf(stderr,
            "usage:\n\npr %s %s\n\n",
            "[-i] [-lnn] [-onn] [-p<name>] [-tn] [-wnn]",
            "[file1[ file2 ... filen]]");
            fprintf(stderr,
            "where: i = read 'stdin' for filenames to print\n");
            fprintf(stderr,
            "       l = lines-per-page and nn <= 120\n");
            fprintf(stderr,
            "       o = page offset    and nn <= 120\n");
            fprintf(stderr,
            "       p = print redirection and\n");
            fprintf(stderr,
            "           <name> = pathname or devicename\n");
            fprintf(stderr,
            "       t = spaces-per-tab and n  <= 8\n");
            fprintf(stderr,
            "       w = page width     and nn <= 160\n\n");
            fprintf(stderr,
            "Notes: PAGE(<title text of any length>) in col 1 of text file\n");
            fprintf(stderr,
            "       and <title text...> the title you want.\n\n");
            fprintf(stderr,
            "       C pgms should include the following macro:\n\n");
            fprintf(stderr,
            "            #define PAGE(title)\n\n");
            fprintf(stderr,
            "       < and > not required and should not be used\n\n");
            exit(0);
      }
      
      if (Error_ == getopts(argc, argv))
            return 1;
      
      if ((page_length <= 0) || (page_length > 120))
            page_length = PAGE_LENGTH;

      if ((tab_width <= 0) || (tab_width > 8))
            tab_width = TAB_DEFAULT;

      if ((page_offset < 0) || (page_offset > 120))
            page_offset = PAGE_OFFSET;

      if ((page_width <= 0) || (page_width > 160))
            page_width = PAGE_WIDTH;

      for (fi = 0, pn = 1; pn < xargc; ++fi, ++pn)
      {
            if (fi < MAX_FILES)
                  strcpy(filenames[fi], xargv[pn]);
            else
            {
                  fprintf(stderr, "pr: "
                        "Exceeded maximum file capacity\n");
                  break;
            }
      }

      if ((lp = fopen(print_name, "w")) == 0)
      {
            fprintf(stderr, "pr: Unable to open %s as output\n", print_name);
            exit(1);
      }

      if (read_stdin)
      {
            for(;;)
            {
                  if (fi == MAX_FILES)
                  {
                        fputs("pr: Exceeded maximum file capacity\n",
                              stderr);
                        break;
                  }
                  cp = fgets(filenames [fi], FILENAME_MAX, stdin);
                  if (!cp)
                        break;
                  else  fi++;
            }
      }
      /* now print each file */

      for (pn = 0; pn < fi; pn++)
            prt(filenames [pn], lp);  /* print the file */
      return 0;
}
PAGE (NEW PAGE)

void new_page (char *fnp, FILE *lp)
{
      if (ln < 3)
            return;
      ++pn;
      if (pn > 1)
            fputc('\f', lp);
      fprintf(lp, "%s    %s %s    PAGE %d:  %s\n\n",
                   fnp, Date, Time, pn, title);
      ln = 2;
}

PAGE (PRINT FILE)
void prt (char fnp[], FILE *lp)
{
      FILE *inp_file;
      int col;
      char line [256], *st, *et, *sp;
      time_t now;
      struct tm *tnow;

      inp_file = fopen(fnp, "r");
      if (!inp_file)
      {
            fprintf(stderr, "pr: unable to open %s\n", fnp);
            return;
      }
      else
            fprintf(stderr, "pr: printing %s\n", fnp);

      pn = 0;
      ln = 999;
      now  = time(&now);
      tnow = localtime(&now);
      strftime(Date, 19, "%a %d %b %Y", tnow);
      strftime(Time, 19, "%I:%M %PM", tnow);
      *title = '\0';

      while (fgets(line, 256, inp_file))
      {
            if (strncmp(line, "PAGE", 4) == 0)
            {
                  if (NULL != (st = strchr(line, '(')))
                  {
                        et = strchr(line, ')');
                        strncpy(title, st + 1, (et) ? et - st - 1 : 160);
                  }
                  ln = 999;
            }

            if (ln > page_length)
                  new_page(fnp, lp);

            if (page_offset)
                  indent(lp);

            for (col = (page_offset) ? page_offset : 1, sp = &line[0];
                   *sp; sp++)
            {
                  switch (*sp)
                  {
                  case '\t':  /* tab character */
                        do
                        {
                              fputc(' ', lp);
                              col++;
                              if (col > page_width)
                              {
                                    fputc('\n', lp);
                                    col = (page_offset) ? page_offset : 1;
                                    ln++;
                                    if (ln > page_length)
                                          new_page(fnp, lp);
                                    if (page_offset)
                                          indent(lp);
                                    break;
                              }
                        } while ((col - 1) % tab_width);
                        break;

                  case '\f':  /* form feed character */
                        new_page(fnp, lp);
                        break;

                  default:
                        fputc(*sp, lp);
                        ++col;
                        if (col > page_width)
                        {
                              fputc('\n', lp);
                              col = (page_offset) ? page_offset - 1 : 0;
                              ln++;
                              if (ln > page_length)
                                    new_page(fnp, lp);
                              if (page_offset)
                                    indent(lp);
                        }
                  }
            } /* of line print (for) */
            ++ln;
      } /* of while not eof */
      fclose(inp_file);
      fputc(014, lp);
} /* of print */

void indent(FILE *lp)
{
      int i;

      for(i = 1; i < page_offset; i++)
            fputc(' ', lp);
}
