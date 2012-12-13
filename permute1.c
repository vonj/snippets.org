#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*  chouse_n ( char *strng, int length) returns a pointer to a string of   */
/*  length characters chosen from "strng" , duplicate chars in "strng" are */
/*  significant.  Strings are generated in lexical order.                  */
/*  First call, call with *strng. each subsequent call, call with NULL,    */
/*  returns one combination. Calls after all combinations have been        */
/*  returned return NULL.  Will return NULL for errors.                    */
/*  not very defensive (i.e. WILL BREAK)                                   */

/*  dave chapman aug '91  released to public domain                        */

char *chouse_n( char *strng, int length);

char *chouse_n( char *strng, int length)
{
      static char *str;
      static char *curr;
      static char *pos;       /* for each char in curr(ent string),
                                 its pos in str                            */
      static int  counts[256];
      int i,j;

      if (0 >= length)
            return NULL;

      if (NULL != strng)
      {
            str = malloc(strlen(strng)); /* first call, prep string for use */
            curr = malloc(2 * length + 1);
            pos = curr + length +1;

            for (i = 0; i < 256; counts[i++] = 0)
                  ;
            for (i = 0; strng[i]; i++)
                  counts[strng[i]]++;

            for (i = 1, j = 0; i < 256; i++)
            {
                  if (counts[i])
                  {
                        str[j] = i;
                        counts[j++] = counts[i];
                  }
            }
            str[j] = '\0';      /* str is string of distinct chars in order */
                                /* counts[] holds count of each char        */

            /* take first length chars */

            for (i = 0,j = 0; i < length; i++)
            {
                  curr[i] = str[j];
                  pos[i] = j;
                  if (!(--counts[j]))
                        j++;
            }
            curr[i] = '\0';
            return curr;
      }
      /* if called with "mississippi",5;
         str -> "imps"
         curr -> "iiiim"
         counts -> 0,0,2,4;
         pos -> 0,0,0,0,1;   */
      
      /* go back to front */

      for (j = length; j > 0;)
      {
            counts[ pos[--j]]++;                      /* "replace" char */

            /* look for a new char for curr posit. */

            for ( i = ++pos[j]; str[i] && ! counts[i]; i++)
                  ;
            if (0 != (curr[j] = str[i]))              /* found a char   */
            {
                  --counts[i];
                  pos[j] = i;

                  /* placed char, fill out rest of string  */

                  for (++j, i = 0; j < length; j++)
                  {
                        for ( ; !counts[i]; i++)
                              ;
                        curr[j] = str[i];       /* first available char */
                        --counts[i];
                        pos[j] = i;
                  }
                  return curr;
            }
            /* no more chars for this pos ; go back one */
      }
      /*  done */
      return NULL;
}

main()
{
      char  *str = "aabbccdd";
      int i,j;

      j = 0;
      i =  5;
      puts(chouse_n( str, i));
      while (NULL != (str = chouse_n(NULL, i)))
      {
            ++j;
            printf(" %s  %d\n",str,j);
      }
      return 0;
}
