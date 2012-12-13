/*
**  PERMUTE.C - prints all permutations of an input string
**
**  public domain demo by Jon Guthrie
*/

#include    <string.h>
#include    <stdlib.h>
#include    <stdio.h>

int     charcmp(char *, char *);

void    permute(char *, int, int);

int     main(int argc, char *argv[])
{
      int length;

      if (2 != argc)	/* Bug fixed by Stephan Wilms	*/
      {
            puts("Usage: PERMUTE string");
            abort();
      }
      
      length = strlen(argv[1]);

      /* It only works if they're printed in order */

      qsort(argv[1], length, 1, (int(*)(const void *, const void *))charcmp);

      permute(argv[1], 0, length);
      return 0;
}


/*
**  This function prints all of the permutations of string "array"
**  (which has length "len") starting at "start."
*/

void    permute(char *array, int start, int len)
{
      int j;
      char    *s;

      if(start < len)
      {
            if(NULL == (s = malloc(len + 1)))	/* Bug fixed by Stephan Wilms	*/
            {
                  printf("\n\nMemory error!!!\a\a\n");
                  abort();
            }

            strcpy(s, array);
            for(j=start ; j<len ; ++j)
            {
                  int     temp;

                  if((j == start) || (s[j] != s[start]))
                  {     /* For each character that's different    */
                        /* Swap the next first character with...  */
                        /* the current first                      */
                        temp = s[j];
                        s[j] = s[start];
                        s[start] = temp;
                        permute(s, start+1, len);
                  }
            }
            free(s);
      }
      else  puts(array);
}



int charcmp(char *a, char *b)
{
      return(*a - *b);
}
