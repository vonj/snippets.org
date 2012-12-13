/* public domain by Jerry Coffin.  Tested with MSC 7.0  */
/* written primarily for clarity, not speed...          */
/* requires w_wrap.c and w_wrap.h from snippets.        */


#include <conio.h>
#include <string.h>
#include "w_wrap.h"

void center(FILE *file, char *string, size_t width)
{
      char *line,*end_line;
      size_t spaces;
      int last = 0;
      size_t len;

      word_wrap(string,width);
      line = string;
      while (!last)
      {
            end_line = strchr(line,'\n');
            if (NULL==end_line)
            {
                  last = 1;
                  end_line = strchr(line,'\0');
            }
            len = end_line - line;
            spaces = (width - len) / 2 ;
            fprintf(file,"\n%*c%*.*s",spaces,' ',len,len,line);
            line = end_line + 1;
      }
}

#ifdef TEST

int main(void)
{
      char *string = "This is a long string to see if it will be"
            " printed out correctly but I'm not sure whether it will work"
            " correctly or not.  I guess we'll see when we try it out.";

      printf("\nHere's the string centered in 50 columns.\n");
      center(stdout,string,50);
      printf("\n\nAnd here it's centered in 72 columns.\n");
      center(stdout,string,72);
      return 0;
}

#endif /* TEST */
