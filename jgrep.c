/*
**  JGREP.C - A utility to search files for text.
**
**  public domain by Jerry Coffin
**
**  Link with wildargs.obj (Borland), setargv.obj (Microsoft), _mainX.obj 
**  (Symantec/Zortech), or wildargX.obj (Watcom) which allows you to pass 
**  wildcards on the command line.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "extkword.h"

#define LINELEN 1024
#define BUFSIZE 32767

#if defined(_QC) || defined(_MSC_VER)
    void CDECL _setenvp(void) {}
    void CDECL _nullcheck(void) {}
#endif

enum { FALSE, TRUE };

typedef unsigned char uchar;

static size_t table[UCHAR_MAX+1];
static size_t len;
static char *string=NULL;

void init_find(char *new_string)
{
      size_t i;

      if (NULL != string)
            free(string);
      string = strdup(new_string);
      len = strlen(string);

      for (i=0;i<=UCHAR_MAX;i++)
            table[i]=len;
      for (i=0;i<len;i++)
            table[string[i]]=len-i-1;
}

char *find_case(char *string2)
{
      size_t limit = strlen(string2);
      size_t shift;
      size_t pos=len-1;
      char *here;

      while (pos < limit)
      {
            while( pos < limit && (shift=table[(uchar)string2[pos]])>0)
                  pos+=shift;
            if (0==shift)
            {
                  if (!memcmp(string,here=string2+pos-len+1,len))
                        return(here);
                  else  pos++;
            }
      }
      return NULL;
}

char *find_no_case(char *string2)
{
      size_t limit = strlen(string2);
      size_t shift;
      size_t pos=len-1;
      char *here;

      while (pos < limit)
      {
            while( pos < limit &&
                  (shift=table[(uchar)toupper(string2[pos])])>0)
            {
                  pos+=shift;
            }
            if (0==shift)
            {
                  if (!memicmp(string,here=string2+pos-len+1,len))
                        return(here);
                  else  pos++;
            }
      }
      return NULL;
}

char *( *find)    (char *)=find_case;

int main(int argc, char **argv)
{
      int         k, first;
      unsigned    line;
      int line_numbers=FALSE, reverse=FALSE, print_file_name=FALSE;
      int no_case = TRUE;
      char line_buffer[LINELEN],*string,c;
      FILE *infile=NULL;
      static char buffer[BUFSIZE];

      while (argc > 1 && (c=argv[1][0])=='/' || c=='-')
      {
            switch(tolower(argv[1][1]))
            {
            case 'c':
            case 'y':
                  no_case=FALSE;
                  break;

            case 'f':
                  print_file_name=TRUE;

            case 'n':
                  line_numbers=TRUE;
                  break;

            case 'v':
                  reverse = TRUE;
                  break;

            default:
                  fprintf(stderr,"unknown switch -%c",argv[1][1]);
            }
            argv++;
            argc--;
      }
      if (argc < 3)
      {
            fprintf(stderr ,"\nsyntax: find [-c|y][-n][-v][-f] "
                  "string filename ..."
                  "\n\t-c | -y : make case significant ('c' != 'C')"
                  "\n\t-n      : number lines"
                  "\n\t-f      : place file name before lines ( forces -n)"
                  "\n\t-v      : print lines that don't match"
                  "\n - by itself to read from standard input");
            return(1);
      }

      string=argv[1];
      if (no_case)
      {
            strupr(string);
            find=find_no_case;
      }
      init_find(string);
      for (k=2;k<argc;k++)
      {
            if (infile != NULL)
                  fclose(infile);
            if ((infile=fopen(argv[k],"r"))==NULL)
                  continue;
            first=TRUE;
            setvbuf(infile,buffer,_IOFBF,BUFSIZE);
            line=0;
            while (fgets(line_buffer,LINELEN,infile)!=NULL)
            {
                  line ++;
                  if ((NULL!=find(line_buffer)) ^ reverse)
                  {
                        if (first && !print_file_name )
                        {
                              printf("\n\t%s:\n",argv[k]);
                              first = FALSE;
                        }
                        if (print_file_name)
                              printf(" %s ",argv[k]);
                        if (line_numbers)
                              printf("%d:",line);
                        printf("%s",line_buffer);
                  }
            }
      }
      return 0;
}
