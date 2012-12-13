/*
**  TEXTMOD.C - Demonstrates techniques for modifying text files
**
**  public domain demo by Bob Stout
*/

#include <string.h>
#include "snipfile.h"

void show_text_file(char *txt)
{
      FILE *in;
      char line[80];

      in = cant("test.txt", "r");
      printf("\n%s:\n\n", txt);

      while (!feof(in))
      {
            if (NULL != fgets(line, 80, in))
                  fputs(line, stdout);
      }
}

void create_text_file(void)
{
      FILE *out;

      out = cant("test.txt", "w");
      fputs("This is a test!\n", out);
      fputs("This is a dummy line to delete...\n", out);
      fputs("This is a dummy line to modify...\n", out);
      fputs("All done!\n", out);
      fclose(out);

      show_text_file("The file as written is");
}

main()
{
      FILE *in, *out;
      char line[80], *tmp, *ptr;

      /*
      **  Open the original file for reading and a temporary file for writing
      */

      create_text_file();
      in  = cant("test.txt", "r");
      tmp = tmpnam(NULL);
      out = cant(tmp, "w");

      /*
      **  Read the first line and copy it
      */

      fgets(line, 80, in);
      fputs(line, out);

      /*
      **  Discard the second line
      */

      fgets(line, 80, in);

      /*
      **  Add a new line
      */

      fputs("(Isn't it?)\n", out);

      /*
      **  Read the 3rd line, modify it, then write it out
      */

      fgets(line, 80, in);
      ptr = strrchr(line, 'm');
      strcpy(ptr, "edit...\n");
      fputs(line, out);

      /*
      **  Read the last line and copy it
      */

      fgets(line, 80, in);
      fputs(line, out);

      /*
      **  Close the files, delete the old, rename the temp
      */

      fclose(in);
      fclose(out);
      remove("test.txt");
      rename(tmp, "test.txt");

      /*
      **  Now let's see the results
      */

      show_text_file("The file as modified is");
      
      return 0;
}
