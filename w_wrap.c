/* w_wrap.c */

/*
**  This is an attempt at a useful word-wrap function.  It is given an array
**  of characters ( a string ) and it modifies the string, replacing any
**  new-lines found with spaces and placing new-lines where needed to make
**  lines of the width specified, placing them only where there was previously
**  white-space. ( ie. Words are not split across lines. )  At the present
**  time it is rather stupid. 1) It doesn't know enough to split a line at an
**  existing hyphen.  2) It has no clue about how to hyphenate words.  3) It
**  makes no attempt at dealing intelligently with a singly word longer than
**  the specified line length 4) It does not deal intelligently with multiple
**  spaces new-lines, etc. ( eg. has no clue of paragraph separation, etc. )
**  OTOH, it does deal well with unformatted, left justified text.
**
**  Tabs will be considered the size specified.  Note that word_wrap() does
**  not actually expand tabs.  This is only to inform it of the number of
**  spaces the output device will expand them to, so it will know how much
**  they expand a line.  The only time word_wrap does anything with tabs, is
**  if the tab size is set to zero, in which case each tab is replaced with a
**  single space character.  This often provides the most useful output, since
**  tabs will often be in the wrong places after re-formatting, and is
**  therefor the default.
**
**
**  Publicly available contents:
**
**      char *word_wrap(char *string,long line_len);
**          Does the actual word-wrapping, as described above;
**          Parameters:
**            string:     actual string to work with
**            line_len:   length of lines for output
**          Returns:        pointer to justified string.
**
**          void set_tab_size(int size);
**            Set the number of spaces that tabs will be expanded to on output
**            default tab size is zero. (each tab replaced with a space char )
**            word_wrap does not actually expand tabs.  This only lets it keep
**            track of how many spaces they take up.  If this is set to
**            zero, each tab will be replaced with a single space.
**
**  Other procedures:
**      int get_word(char *string);
**          returns the number of characters in the next word in string,
**          including leading white-space characters.
**
**  This compiles without warnings and runs with the following compilers:
**      MS Quick C 2.51:
**      Borland C++ 2.0:            either as C or C++
**      GNU C++ 1.39, DOS port:     either as C or C++
**  As far as I know, it uses only portable, standard C constructs.  It should
**  compile and run with little or no modification under nearly any C compiler
**  and environment.
**
**
**  This code was written Nov 16, 1991 by Jerry Coffin.
**  It is hereby placed in the public domain, for free use by any and
**  all who wish to do so, for any use, public, private, or commercial.
*/

#include <ctype.h>
#include "w_wrap.h"

static int tab_size = 0;                  /* size to consider tabs as */

static size_t get_word(char *string);     /* returns size of next word*/

void set_tab_size(size_t size)
{
      tab_size = size;
}

char *word_wrap(char *string, size_t line_len)
{
      size_t len,                         /* length of current word */
             current_len = 0;             /* current length of line */
      size_t start_line = 0;              /* index of beginning if line */

      while (0 != (len = get_word(&string[current_len + start_line])))
      {
            if (current_len + len < line_len)
                  current_len += len;
            else
            {
                  string[start_line+current_len] = '\n';
                  start_line += current_len + 1;
                  current_len = 0;
            }
      }
      return string;
}

static size_t get_word(char *string)
{
      register int i = 0, word_len = 0;

      if (!string[0])
            return 0;
      while (isspace(string[i]))
      {
            if ('\t' == string[i])
            {
                  if (0 == tab_size)
                        string[i] = ' ';
                  else  word_len += tab_size-1;
            }
            else if ('\n' == string[i])
                  string[i]=' ';
            word_len++;
            i++;
      }
      while (string[i] && !isspace(string[i++]))
            word_len++;
      return word_len;
}

#ifdef TEST

#include "w_wrap.h"

main()
{
      char *string =
            "This is a long line\nto be wrapped by the w_wrap function. "
            "Hopefully, things will work correctly and it will be wrapped "
            "between words.  On the other hand, maybe I should hope that it "
            "doesn't work well so I will have an opportunity\nto learn more "
            "about what I'm doing";

      printf("Here's a string wrapped to 40 columns:\n\n%s\n\n",
            word_wrap(string, 40));
      printf("And here it's wrapped to 72:\n\n%s\n\n",
            word_wrap(string,72));
      return 0;
}

#endif /* TEST */
