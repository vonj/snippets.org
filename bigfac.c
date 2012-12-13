/*
**  bigfac.c -- put into the public domain by Carl Declerck
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFLEN 8192
#define BUFFER ((char *) malloc(BUFFLEN))

int  main (void);
void multiply (char *, char *, char *);
void zero_buffer (char *);
void minus_one (char *);
int  isnull (char *);
void factorial (char *);

main (void)
{
      char *g = BUFFER;

      printf ("Enter a number: ");
      scanf ("%s", g);
      printf ("Factorial of %s is: ", g);
      factorial (g);
      printf ("%s\n", g);
      free (g);
      return 0;
}

void multiply (char *g1, char *g2, char *g3)
{
      int gp1, gp2, cumpos, respos, mod, div;
      int cmod, cdiv, resoff, wdig1, wdig2, base;

      zero_buffer (g3);
      for (gp2 = strlen(g2) - 1; gp2 >= 0; gp2--)
      {
            wdig2 = *(g2 + gp2) - 48;
            resoff = strlen(g2) - gp2 - 1;
            respos = BUFFLEN - resoff - 2;
            for (gp1 = strlen(g1) - 1; gp1 >= 0; gp1--)
            {
                  wdig1 = *(g1 + gp1) - 48;
                  mod = (wdig1 * wdig2) % 10;
                  div = (wdig1 * wdig2) / 10;
                  base = *(g3 + respos) - 48;
                  cmod = (base + mod) % 10;
                  cdiv = (base + mod) / 10 + div;
                  *(g3 + respos) = (char)(cmod + 48);
                  cumpos = --respos;
                  while (cdiv > 0)
                  {
                        base = *(g3 + cumpos) - 48;
                        *(g3 + cumpos--) = (char)((base + cdiv) % 10 + 48);
                        cdiv = (base + cdiv) / 10;
                  }
            }
      }
      for (respos = 0; *(g3 + respos) == '0'; respos++)
            ;
      strcpy (g3, (char *) (g3 + respos));
      if (*g3 == 0)
            strcpy (g3, "0");
}

void zero_buffer (char *buff)
{
      int cnt;

      for (cnt= 0; cnt < BUFFLEN; cnt++)
            *(buff + cnt) = '0';
      *(buff + BUFFLEN - 1) = 0;
}

void minus_one (char *g)
{
      int p;
      char digit;

      p = strlen(g) - 1;
      digit = *(g + p);
      while (digit == '0')
      {
            *(g + p--) = '9';
            digit = *(g + p);
      }
      *(g + p) -= 1;
}

int isnull (char *g)
{
      int p, ok = 1;

      for (p = 0; p < (int)(strlen(g)); p++)
            if (*(g + p) != '0')
                  ok = 0;
      return (ok);
}

void factorial (char *g)
{
      char *h1 = BUFFER, *h2 = BUFFER;

      strcpy (h1, "1");
      while (!isnull(g))
      {
            multiply (h1, g, h2);
            strcpy (h1, h2);
            minus_one (g);
      }
      strcpy (g, h1);
      free (h1);
      free (h2);
}

/*
**  The principal function is multiply(), it 'multiplies' two
**  character-strings of arbitrary length and puts the result
**  into a third.  8192 bytes is enough for 1000!, beyond that
**  the buffer-size may need to be incremented.
*/
