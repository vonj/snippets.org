/* getcmt.c - get comments from a C or C++ source file    */
/*
                               Byte_Magic Software
                             9850 Meadowglen Ln. #35
                               Houston, Tx.  77042
                                 (713) 975-9033

Author:     Greg Messer
Program:    getcmt.c
Purpose:    Isolate comments from a C or C++ source file. Filter.
Syntax:     getcmt [/L?] [filename]
            (may use redirection for file and/or device I/O)
Release:    Released to the Public Domain by Byte_Magic Software.
Date:       07-11-88 GM...
                First release.
Revised:    01-13-90 GM...
                Streamlined logic.
            01-15-90 Bob Stout (RBS)...
                Fixed unsigned char error as return from getc().
            01-22-90 GM...
                Fixed bug handling "xx/" (xx = **) at end of comment.
                Added C++ comment extraction.
                Added return of count to OS (ERRORLEVEL in MS-DOS).
            01-24-90 RBS
                Added filename spec option
                Added /? switch
System:     Compiled with Zortech C V2.06 under MS-DOS 3.20
            for IBM PCs and compatibles.
Rules:      ANSI C comments begin with /x and end with x/. (x = *).
            Comments do not nest and do not appear in string or character
            constants.
            C++ comments begin with double slashes and end at EOL.
            A Microsoft extension to C allows C++ style comments to serve as
            single-line comments in C source.
Comments:   Useful for creating documentation and improving commenting style.
            Input may be from a specified filename or stdin.
            Output is to stdout, so use DOS redirection for output.
            Messages go to stderr so they are not redirectable from the screen.
            Returns ERRORLEVEL = number of comments in source file(s).
Examples:
            Example... Output to screen:
            getcmt < cfile.c
               (displays comments from cfile.c on screen)
            type cfile.c | getcmt
               (same as above but slightly slower)
            getcmt < cfile.c | more
               (same as above, but pauses after each full screen)
            getcmt cfile.c /l | more
               (same as above, but display line numbers)

            Example... Output to printer:
            getcmt < cfile.c > prn
               (same as above but prints output on printer)
            type cfile.c | getcmt > prn
               (same as above but slightly slower)

            Example... Output to file:
            getcmt < cfile.c > cfile.cmt
               (writes cfile.c comments to cfile.cmt, overwriting existing file)
            getcmt < cfile.c >> cfile.doc
               (writes cfile.c comments to end of cfile.doc (appends))
                       
            getcmt /?
               (displays help screen, returns ERRORLEVEL = 0)
            getcmt /x
               (invalid option - displays help screen, returns ERRORLEVEL = -1)

            For complete instructions on using redirection symbols, consult
            the PC-DOS or MS-DOS manual or a general DOS reference book.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define LOOKS_GREAT 1
#define LESS_FILLING 0

int extract_c_cmts(void);
void inside_c_cmt(int);

FILE *infile = stdin;                   /* read input from here             */
int  show_nos = 0;                      /* 0 = don't display line numbers   */
int  line_no = 1;                       /* line_no = line number            */

/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   */

main(int argc, char *argv[])                            /* main logic:      */
{
    register int i;
    const char *hype =
        "\nGETCMT v1.1 - GET CoMmenTs\nby Byte_Magic Software\n";
    const char *help =
        "\nUsage: GETCMT [/l?] [filename | <sourcefile.ext] "
        "[>destination file or device]\n"
        "Options:  l - Print line numbers\n"
        "          ? - Help\n"
        "\nFilename optional - Reads source code from stdin "
        "(Ctrl-C to quit before EOF)\n";
    const char *oops =
        "\a*** GETCMT - Can't open input file ";
                                            /* display messages to operator */
#if LOOKS_GREAT
    fputs(hype, stderr);
#elif LESS_FILLING
    i = 0;
    while(hype[i] != '\0')
        putc(hype[i++], stderr);
#endif

    if (1 < argc)
    {
        for (i = 1; i < argc; ++i)
        {
            if ('/' == *argv[i])
            {
                if ('l' == tolower(argv[i][1]))
                    show_nos = 1;
                else
                {
                    int ercode;

                    ercode = ('?' == argv[i][1]) ? 0 : -1;
#if LOOKS_GREAT
                    fputs(help, stderr);
#elif LESS_FILLING
                    i = 0;
                    while(help[i] != '\0')
                        putc(help[i++], stderr);
#endif
                    if (ercode)             /* output BEL if invalid switch */
                        putc('\a', stderr);
                    return(ercode);
                }
            }
            else
            {
                infile = fopen(argv[i], "r");
                if (!infile)
                {
#if LOOKS_GREAT
                    fputs(oops, stderr);
                    fputs(argv[i], stderr);
#elif LESS_FILLING
                    char *p = argv[i];

                    i = 0;
                    while (oops[i])
                        putc(oops[i], stderr);
                    i = 0;
                    while (*p)
                        putc(*p++, stderr);
#endif
                }
            }
        }
    }

    i = extract_c_cmts();                   /* extract comments in infile   */
    putc('\n', stdout);

    return(i);                              /* return number of comments to */
                                            /* OS (ERRORLEVEL in DOS)       */
}

/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   */

int extract_c_cmts()                        /* comment extraction logic:    */
{
    register int chi, cht;              /* chi = char in, cht = char test   */
    int count;                          /* count = comment count            */

    count = 0;
    chi = getc(infile);
    while(chi != EOF)                       /* as long as there is input... */
    {
        if(chi == '/')                      /* process comments             */
        {
            cht = getc(infile);
            if(cht == EOF)
                return(count);
            if(cht == '*' || cht == '/')    /* if start of a comment...     */
            {
                count++;                    /* count it and                 */
                inside_c_cmt(cht);          /* output all of the comment    */
            }
            else
                ungetc(cht, infile);
        }
        if ('\n' == chi)
            line_no += 1;
        chi = getc(infile);                  /* continue scanning input     */
    }
    return(count);
}

/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   */

char *lntoaz(void)                      /* line number to zero-padded ASCII */
{
    int i, num = line_no;
    static char numbuf[] = "0000: ";

    if (9999 < num)
        strncpy(numbuf, "0000", 4);
    else for (i = 3; i >= 0; --i)
    {
        numbuf[i] = (char)('0' + num % 10);
        num /= 10;
    }
    return numbuf;
}

/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   */

void inside_c_cmt(int ch)                   /* comment output logic:        */
{                                           /* input ch = either '*' for C  */
                                            /* or '/' for C++               */

    register int chi, cht;              /* chi = char in, cht = char test   */
#if LESS_FILLING
    char *p;
#endif

    if(ch == '/')                           /* make ch = '\n' if C++        */
        ch = '\n';                          /* note: ch is already 1st char */
                                            /* of end comment if this is C  */
    putc('\n', stdout);
    if (show_nos)
    {
#if LOOKS_GREAT
    fputs(lntoaz(), stdout);
#elif LESS_FILLING
    p = lntoaz();
        while (*p)
        putc(*p++, stdout);
#endif
    }
    chi = getc(infile);
    while(chi != EOF)                       /* as long as there is input... */
    {                                       /* process comments             */
        if(chi == ch)
        {
            if(ch == '\n')                  /* if C++ comment is ended...   */
                return;                     /* stop outputting              */
            cht = getc(infile);
            if(cht == '/')                  /* if C comment is ended...     */
                return;                     /* stop outputting              */
            else
            {
                ungetc(cht, infile);
                putc(chi, stdout);
            }
        }
        else
            putc(chi, stdout);              /* else comment text, output it */
        if ('\n' == chi)
            line_no += 1;
                                            
        chi = getc(infile);                  /* continue scanning input     */
    }
    return;
}

/*  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   */
/* end of getcmt.c  */
