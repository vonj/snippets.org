/*
**  TAIL.C
**  ----------------------------------------------------------------------
**  Display the last n lines of a file (20 lines by default).
**
**  Revision history
**  ================
**  Modified  19930604 by Ruurd Pels:
**  - Increased default line numbers from 5 to 20
**  - Made ANSI C conformant (I hope)
**  - Added '-' support for commandline
**  - Outputs header to stderr instead of stdout to leave it out when
**    redirecting files
**  - Fixed \r\r\n bug for MSDOS machines
**
**  Modified  19861005 by Joe Huffman:
**  - Utilize prototyping, fixed a bug, added (a few) comments and help.
**
**  Written   19860204 by Joe Huffman.
**
**  Not copyrighted.
*/

#include <stdio.h>
#include <stdlib.h>

char            head1[] = {"\n------- \""};
char            head2[] = {" -------\n"};
FILE *          fp;
int             filenum;
int             cc;
unsigned int    linenum = 20;
unsigned int    indx;
long int *      tail;

/*
**  Get the number of lines to display at the "tail" of each file from
**  the command line.
*/

void getlinenum(int n, char * str[])
{
      for (--n; n; --n)
      {
            ++str;
            if ((**str == '/') || (**str == '-'))
            {
                  linenum = atoi(*(str) + 1);
                  if (linenum <= 0)
                        linenum = 20;
            }
      }

      /* Because we save a pointer to the end of the PREVIOUS line */
      linenum++;
}

/*
**  Set the file pointer "fp" to "linenum - 1" lines before the end of
**  the file.
*/

void gettail(void)
{
      unsigned char outstr[15];
      unsigned long int currline = 0L;

      tail = (long int *)malloc(sizeof(*tail) * linenum);
      if (!tail)
      {
            fputs("Insufficient memory.", stderr);
            exit(1);
      }
      tail[0] = ftell(fp);
      indx = 0;

      for (cc = getc(fp); cc != EOF; cc = getc(fp))
      {
            if (cc == '\r')
            {
                  ++currline;
                  cc = getc(fp);
                  if (cc != '\n')
                        ungetc(cc, fp);
                  ++indx;
                  indx %= linenum;
                  tail[indx] = ftell(fp);
            }
            else
            {
                  if (cc == '\n')
                  {
                        ++currline;
                        cc = getc(fp);
                        if (cc != '\r')
                              ungetc(cc, fp);
                        ++indx;
                        indx %= linenum;
                        tail[indx] = ftell(fp);
                  }
            }
      }
      fputs("\" ", stderr);
      ltoa(currline, (char *)outstr, 10);
      fputs((char *)outstr, stderr);
      fputs(" lines", stderr);
      if (currline >= linenum - 1)
      {
            indx++;
            indx %= linenum;
      }
      else  indx = 0;

      if (fseek(fp, tail[indx], 0) == -1)
      {
            fputs("\nFile seek error.", stderr);
            exit(1);
      }
      free(tail);
}

/*
**  Tell the user what the program is and how to use it.
*/

void help(void)
{
      char *ptr;
      static char help_str[] =  "Usage:\n\nTAIL <filename> [filename] "
            "[/n]\n\n<filename>  - The name of a valid file, wildcards "
            "accepted.\nn           - Number of lines to print out, 20 "
            "by default.";

      for (ptr = &help_str[0]; *ptr; ptr++)
            fputc(*ptr, stdout);
}

int main(int argc, char **argv)
{
      if (argc <= 1)
      {
            help();
            exit(1);
      }

      getlinenum(argc, argv);

      for (filenum = 1; filenum < argc; ++filenum)
      {
            if (*argv[filenum] == '/')
                  continue;
            fp = fopen(argv[filenum], "rb");
            if (!fp)
            {
                  fputs(head1, stderr);
                  fputs(argv[filenum], stderr);
                  fputs("\" not found.", stderr);
                  fputs(head2, stderr);
            }
            else
            {
                  fputs(head1, stderr);
                  fputs(argv[filenum], stderr);
                  gettail();
                  fputs(head2, stderr);
                  for (cc = getc(fp); cc != EOF; cc = getc(fp))
                  {
#ifdef __MSDOS__
                        if (cc != '\r')
                        {
                              fputc(cc, stdout);
                        }
#else
                        fputc(cc, stdout);
#endif
                  }
                  fclose(fp);
            }
      }
      return EXIT_SUCCESS;
}
