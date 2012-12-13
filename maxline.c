/*
** maxline.c - returns the length of the longest line in text file
**
** Released to Public Domain by Phi Nguyen
**
** 03.22.96 - First release
** 03.26.96 - Chad Wallace, for use with FILECAT.C
**              Uses an array of pointers to strings (ala argv)
**              instead of a file.
*/

#include <stdio.h>
#include <string.h>

unsigned int max_line(char ** str_array)
{
      unsigned cur_len, max = 0;

      while (*str_array != NULL)
      {
            cur_len = strlen(*str_array);
            if (cur_len > max)
                  max = cur_len;

            str_array++;
      }

      return max;
}

#ifdef TEST

/*
** Will return the length of the largest parameter given on the
** command line, including argv[0], the program's .EXE file.
*/

main(int argc, char **argv)
{
      printf("%i\n", max_line(argv));

      return 0;
}

#endif

