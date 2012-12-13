/*
** FILECAT.C - Adds one file onto another vertically, as with a column
** block in QEdit.
**
** Must be compiled in Compact or Large memory models to use larger
** files.
**
** Public Domain by Chad Wallace, 1996
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filecat.h"

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

/*
** filecat() error codes
*/

enum {
      FC_SUCCESS = 0,         /* Success */
      FC_RDEST,               /* Error reading dest file */
      FC_WDEST,               /* Error writing dest file */
      FC_SRC,                 /* Error with src file */
      FC_MEM,                 /* Out of memory */
      FC_LINES,               /* Too many lines (increase max_lines) */
      FC_LINE                 /* Line too long (increase line_max) */
};

/*
** These are global to save overhead when calling cleanup function
*/

static char ** str_arr;
static char * temp_str;

/*
** Frees all allocated memory and closes specified file
*/

static void cleanup(int tot_lines, FILE * fp)
{
      if (str_arr)
      {
            for ( ; tot_lines >= 0; tot_lines--)
                  free(str_arr[tot_lines]);
            free(str_arr);
      }
      if (temp_str)
            free(temp_str);
      if (fp)
            fclose(fp);
}

int filecat(char * dest_file, char * src_file, int line_max, int max_lines)
{
      int i, tot_lines;
      unsigned int lines_len;
      FILE * fp;

      if ((temp_str = malloc(line_max + 1)) == NULL)
            return FC_MEM;

      /* Allocate memory for pointers to line strings */

      if ((str_arr = calloc(max_lines + 1, sizeof(char *))) == NULL)
      {
            free(temp_str);
            return FC_MEM;
      }

      /* Open destination file */

      if ((fp = fopen(dest_file, "rt")) == NULL)
      {
            free(temp_str);
            free(str_arr);
            return FC_RDEST;
      }

      /* Read destination file into string array line-by-line */

      for (i = 0;
           (fgets(temp_str, line_max + 1, fp) != NULL) && (i <= max_lines);
           i++)
      {
            /* Strip trailing newline from line read */

            if (temp_str[strlen(temp_str) - 1] == '\n')
                  temp_str[strlen(temp_str) - 1] = '\0';

            /* Allocate memory */

            if ((str_arr[i] = malloc(line_max + 1)) == NULL)
            {
                  /* Clean up and return memory error */

                  cleanup(i, fp);

                  return FC_MEM;
            }

            /* Copy the string to its new home */

            strcpy(str_arr[i], temp_str);
      }

      fclose(fp);

      if (i > max_lines)
      {
            /* Clean up and return too many lines error */

            cleanup(i, NULL);

            return FC_LINES;
      }

      /* Get length of longest line */

      lines_len = max_line(str_arr);

      /* Open source file */

      if ((fp = fopen(src_file, "rt")) == NULL)
      {
            /* Clean up and return source file error */

            cleanup(i, NULL);

            return FC_SRC;
      }

      tot_lines = i;

      /*
      ** Get each line from src file and append to corresponding line from
      ** dest file
      */

      for (i = 0;
           (fgets(temp_str, line_max + 1, fp) != NULL) && (i < max_lines);
           i++)
      {
            int j;

            /* Has this line been allocated yet? */

            if (str_arr[i] == NULL)
            {
                  /* Allocate memory */

                  if ((str_arr[i] = malloc(line_max + 1)) == NULL)
                  {
                        /* Clean up and return memory error */

                        cleanup(tot_lines, fp);

                        return FC_MEM;
                  }

                  /* Initialize string */

                  str_arr[i][0] = '\0';

                  tot_lines++;
            }

            /* Pad with spaces to make this line as long as longest */

            for (j = strlen(str_arr[i]); j < lines_len; j++)
                  str_arr[i][j] = ' ';
            str_arr[i][j] = '\0';

            /* Check size of resulting line when strcat'd */

            if (lines_len + strlen(temp_str) > line_max)
            {
                  cleanup(tot_lines, fp);

                  return FC_LINE;
            }

            strcat(str_arr[i], temp_str);
      }

      fclose(fp);

      if (i > max_lines)
      {
            /* Clean up and return too many lines error */

            cleanup(i, NULL);

            return FC_LINES;
      }

      /* Open dest file again, to write this time */

      if ((fp = fopen(dest_file, "wt")) == NULL)
      {
            /* Clean up and return source file error */

            cleanup(i, NULL);

            return FC_WDEST;
      }

      for (i = 0; i < tot_lines; i++)
      {
            if ((fputs(str_arr[i], fp) == EOF) /*|| (fputc('\n', fp) == EOF)*/)
            {
                  cleanup(i, fp);

                  return FC_WDEST;
            }
      }

      cleanup(i, fp);

      return FC_SUCCESS;
}

int main(int argc, char ** argv)
{
      printf("Filecat returned %d\n", filecat(argv[1], argv[2], 2048, 4096));

      return 0;
}
