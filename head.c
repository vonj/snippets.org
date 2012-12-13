#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUL '\000'
#define BEL '\007'
#define LINE_LEN 132

void give_up(char *msg)
{
      putchar(BEL);
      puts(msg);
      exit(-1);
}

int main(int argc, char *argv[])
{
      FILE *infile;
      char line[LINE_LEN + 2];                  /* Allow for '\n' & NUL */
      int i, N = 0;

      if (2 > argc)
            give_up("Usage: HEAD file [number_of_lines]");
      if (NULL == (infile = fopen(argv[1], "r")))
            give_up("Unable to open input file");
      if (2 < argc)
            N = atoi(argv[2]);
      if (!N) N = 4;
      for (i = 0; i < N; ++i)
      {
            if (NULL == fgets(line, LINE_LEN + 1, infile))
                  break;
            line[LINE_LEN + 1] = NUL;           /* Allow too-long lines */
            fputs(line, stdout);
            if (!strrchr(line, '\n'))
                  i -= 1;                       /* More to read         */
      }
      fclose(infile);
      return EXIT_SUCCESS;
}
