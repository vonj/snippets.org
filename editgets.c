/* editgets.c - line input w/editing                                        */
/* this code is released to the public domain                               */
/* written by Jon Burchmore                                                 */
/* modifications & enhancements by Bob Stout                                */

/* This is as close to ANSI compliant C that I could come, but it was made  */
/* on an IBM compatible computer, so I designed it for that platform.       */

/* EXT_KEYS.C in SNIPPETS provides the ext_getch() function, which returns  */
/* a single extended character from the KEYBOARD, not stdin, and waits if   */
/* it must. It is be possible to re-write this function for a computer      */
/* besides an IBM PC. To do so, just rewrite EXT_KEYS.C and using the same  */
/* key definitions in EXT_KEYS.H                                            */

/* Similarly, CURSOR.C in SNIPPETS is used to make the cursor larger when   */
/* in insert mode and smaller when in overwrite mode. To port to a non-PC   */
/* system, simply rewrite CURSOR.C using the same prototype and calling     */
/* parameters as specified in CURSOR.H.                                     */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "sniptype.h"
#include "ext_keys.h"
#include "cursor.h"
#include "editgets.h"
#include "minmax.h"

#define CTRL_CHARS_ALLOWED 0  /* Non-zero allows entry of Ctrl-characters   */

#define BAD_KEY() {fputc('\a', stderr); break;}
#define putch(x)   fputc((x), stderr)

int TabSize = 8;

/*
**  Password mode:
**
**  0 - Normal operation
**  1 - Conventional password mode - '*' is echoed for all characters,
**      only ESC, ENTER, BACKSPC, and CTLHOME are active.
**  2 - Secure password mode - same as above except random characters are
**      displayed rather than '*'s to distract snoops from watching the keys.
*/

int password_mode = 0;

static const char PWchars[] = /* Frequency approximates English usage   */
      "aaabbccddeeeeffgghhiiijklllmmmnnnoooppqrrrsssstttuuvwwxyz"
      "1234567890-|,@._"
      "aaabbccddeeeeffgghhiiijklllmmmnnnoooppqrrrsssstttuuvwwxyz";

static int pickPWchar(void);

/*
**  Arguments: 1) Buffer to receive string
**             2) Size of buffer
**             3) Default string
*/

int editgets(char *s, int maxlen, char *string)
{
      char temp[500];         /* Working buffer                         */
      int   insert,           /* Non-zero if in insert mode             */
            done = 0,         /* Non-Zero if done                       */
            pos,              /* Our position within the string         */
            len,              /* The current length of the string       */
            i, j, k,          /* Temporary variables                    */
            c,                /* Keyboard input                         */
            skip;             /* Spaces to skip when tabbing            */

      if (NULL == string)
            string = "";

      if (0 != (pos = len = strlen(string)))
            strncpy(temp, string, min(len, maxlen));

      for (i = 0; i < maxlen; ++i)
      {
            if (NUL == *string)
                  putch('_');
            else  putch(*string++);
      }
      for (i = 0; i < (maxlen - len); ++i)
            putch('\b');
      fflush(stderr);

      /* Set insert mode, save cursor, and use a big cursor */

      insert = 1;
      cursor('S');
      cursor('B');

      while (!done)
      {
            c = ext_getch();
            switch (c)
            {
            case Key_ESC :
                  if (len == 0)
                        break;
                  if (pos != len)
                  {
                        for (i = pos; i < len; i++)
                              putch('_');
                  }
                  for (i = len; i > 0; i--)
                  {
                        putch('\b');
                        putch('_');
                        putch('\b');
                  }
                  pos = len = 0;
                  break;

            case Key_LTARROW :
            case Key_PLTARROW :
                  if (password_mode)
                        BAD_KEY();
                  if (pos == 0)
                        break;
                  pos--;
                  putch('\b');
                  break;

            case Key_RTARROW :
            case Key_PRTARROW :
                  if (password_mode)
                        BAD_KEY();
                  if (pos == len)
                        break;
                  if (pos != maxlen)
                  {
                        putch(temp[pos]);
                        pos++;
                  }
                  break;

            case Key_HOME :
            case Key_PHOME :
                  if (password_mode)
                        BAD_KEY();
                  while (pos-- > 0)
                        putch('\b');
                  pos = 0;
                  break;

            case Key_END :
            case Key_PEND :
                  if (password_mode)
                        BAD_KEY();
                  while (pos < len)
                        putch(temp[pos++]);
                  break;

            case Key_INS :
            case Key_PINS :
                  if (password_mode)
                        BAD_KEY();
                  insert = (!(insert));
                  cursor(insert ? 'B' : 'N');
                  break;

            case Key_DEL :
            case Key_PDEL :
                  if (password_mode)
                        BAD_KEY();
                  if (pos == len)
                        break;
                  for (i = pos; i < len; i++)
                        temp[i] = temp[i + 1];
                  len--;
                  for (i = pos; i < len; i++)
                        putch(temp[i]);
                  putch('_');
                  for (i = len + 1; i > pos; i--)
                        putch('\b');
                  break;

            case Key_BACKSPC :
                  if (pos == 0)
                        break;
                  if (pos != len)
                  {
                        for (i = pos - 1; i < len; i++)
                              temp[i] = temp[i + 1];
                        pos--;
                        len--;
                        putch('\b');
                        for (i = pos; i < len; i++)
                              putch(temp[i]);
                        putch('_');
                        for (i = len; i >= pos; i--)
                              putch('\b');
                  }
                  else
                  {
                        putch('\b');
                        putch('_');
                        putch('\b');
                        pos = --len;
                  }
                  break;

            case Key_ENTER :
            case Key_PADENTER :
            case Key_NL :
                  done = 1;
                  break;

            case Key_CEND :
            case Key_CPEND :
                  if (password_mode)
                        BAD_KEY();
                  for (i = pos; i < len; ++i)
                        putch('_');
                  for (i = pos; i < len; ++i)
                        putch('\b');
                  len = pos;
                  break;

            case Key_CHOME :
            case Key_CPHOME :
                  if (pos == 0)
                        break;
                  if (pos != len)
                  {
                        while (0 != pos)
                        {
                              for (i = pos - 1; i < len; i++)
                                    temp[i] = temp[i + 1];
                              pos--;
                              len--;
                              putch('\b');
                              for (i = pos; i < len; i++)
                                    putch(temp[i]);
                              putch('_');
                              for (i = len; i >= pos; i--)
                                    putch('\b');
                        }
                  }
                  else
                  {
                        while (0 != pos)
                        {
                              putch('\b');
                              putch('_');
                              putch('\b');
                              pos = --len;
                        }
                  }
                  break;

            case Key_CRTARROW :
            case Key_CPRTARROW :
                  if (password_mode)
                        BAD_KEY();
                  do
                  {
                        if (pos == len)
                              break;
                        if (pos != maxlen)
                        {
                              putch(temp[pos]);
                              pos++;
                        }
                  } while (isspace(temp[pos]));
                  do
                  {
                        if (pos == len)
                              break;
                        if (pos != maxlen)
                        {
                              putch(temp[pos]);
                              pos++;
                        }
                  } while (!isspace(temp[pos]));
                  break;

            case Key_CLTARROW :
            case Key_CPLTARROW :
                  if (password_mode)
                        BAD_KEY();
                  do
                  {
                        if (pos == 0)
                              break;
                        pos--;
                        putch('\b');
                  } while (isspace(temp[pos]));
                  do
                  {
                        if (pos == 0)
                              break;
                        pos--;
                        putch('\b');
                  } while (!isspace(temp[pos]));
                  break;

            case Key_TAB :
                  if (password_mode)
                        BAD_KEY();
                  if (pos == maxlen)
                        break;

                  skip = TabSize - ((pos + TabSize) % TabSize);

                  if (insert)
                  {
                        if ((len + skip) > maxlen)
                              skip = maxlen - len;

                        for (i = pos, j = len + skip - 1, k = len - 1;
                              i < len ; ++i, --j, --k)
                        {
                              temp[j] = temp[k];
                        }
                  }
                  else  /* overwrite */
                  {
                        if ((pos + skip) > maxlen)
                              skip = maxlen - pos;
                  }
                  for (i = 0; i < skip; ++i)
                  {
                        temp[pos++] = ' ';
                        putch(' ');
                  }
                  len = insert ? len + skip : max(len, pos);
                  for (i = pos; i < len; ++i)
                        putch(temp[i]);
                  for (i = len; i > pos; --i)
                        putch('\b');
                  break;

            default :
                  if (pos == maxlen)
                        break;

                  if (c & 0xff00          /* Illegal extended character */

#if !CTRL_CHARS_ALLOWED

                  || iscntrl(c)
#endif
                  )
                  {
                        BAD_KEY();
                  }

                  if ((!(insert)) || pos == len)
                  {
                        temp[pos++] = (char)c;
                        if (pos > len) len++;
                        if (password_mode)
                        {
                              if (2 == password_mode)
                                    putch(pickPWchar());
                              else  putch('*');
                        }
                        else  putch(c);
                  }
                  else
                  {
                        if (len == maxlen)
                              break;
                        for (i = len++; i >= pos; i--)
                              temp[i + 1] = temp[i];
                        temp[pos++] = (char)c;
                        if (password_mode)
                        {
                              if (2 == password_mode)
                                    putch(pickPWchar());
                              else  putch('*');
                        }
                        else  putch(c);
                        for (i = pos; i < len; i++)
                              putch(temp[i]);
                        for (i = len; i > pos; i--)
                              putch('\b');
                  }
            }
      }
      temp[len] = '\0';
      strcpy(s, temp);
      cursor('Z');            /* Restore cursor size  */
      return len;
}

static int pickPWchar(void)
{
      static int inited = 0;

      if (!inited)
      {
            srand(((unsigned int)time(NULL)) | 1);
            inited = 1;
      }
      
      return PWchars[rand() % (sizeof(PWchars) - 1)];
}

#ifdef TEST

main()
{
      char mystring[60];

      memset(mystring, 0, 60);
      puts("                  "
            "0        1         2         3         4         5         6");
      puts("                  "
            "123456789012345678901234567890123456789012345678901234567890");
      fputs("Enter any string: ", stderr);
      editgets(mystring, 60, "This is a test");
      puts("");
      printf("editgets() returned:\n\"%s\"\n", mystring);

      password_mode = 1;
      memset(mystring, 0, 60);
      fputs("Enter any password: ", stderr);
      editgets(mystring, 50, NULL);
      puts("");
      printf("editgets() returned:\n\"%s\"\n", mystring);

      password_mode = 2;
      memset(mystring, 0, 60);
      fputs("Enter any password: ", stderr);
      editgets(mystring, 50, NULL);
      puts("");
      printf("editgets() returned:\n\"%s\"\n", mystring);

      return 0;
}

#endif /* TEST */
