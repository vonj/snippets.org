/********************************************************************
 *                         SETLEVEL                                 *
 *                                                                  *
 * Written by: Lynn R. Lively                                       *
 * Date:       05/15/90                                             *
 * Version:    1.0                                                  *
 * Written in TurboC V2.00                                          *
 *                                                                  *
 * Allows user input in a MSDOS .BAT file. This program asks a      *
 * specified question and sets the errorlevel according to valid    *
 * answer list. Since MSDOS provides no facility for getting user   *
 * input into a .BAT file, and manipulation of enviorinment strings *
 * seems to be unreliable and nonportable, I have written this      *
 * facility as a possible alternative.                              *
 *                                                                  *
 * I hereby place this program into the public domain.              *
 * Users of this utility do so at their own risk. I accept no       *
 * responsibility for the accuracy or useability of this software.  *
 * However, should you have any questions, suggestions, or problems *
 * I would be happy to talk with you (and help if I can).           *
 * My current work number is 713-591-6111                           *
 *  Your Servant,                                                   *
 *       Lynn R. Lively                                             *
 *                                                                  *
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/*
 * The following are bit mask values for the option_flg.
 */

#define CASE_SENSE       1
#define SINGLE_KEY_ENTRY 2

/*
 * The following is the structure deffinition for the valid answer
 * linked list.
 */

typedef struct v_ans {
      char *         ans_key;
      int            rtn_err_lev;
      struct v_ans * next_ans;
} VALID_ANS;

/*
 * Prototypes of functions defined in this module.
 */

void usage (char * progname);

int main (int argc, char * argv[])
{
      register int i = 0;
      register int j;

      int option_flg = 0;

      VALID_ANS * answer_tab = NULL;
      VALID_ANS * wk_ans;

      char * wk_ptr;
      char * ques_ptr = "?";
      char   wk_str[256];

      if (argc > 1)
      {
        /*
         * Step through and parse the argument list.
         */

            for (i = 1; i < argc; i++)
            {
                  j = 0;
                  switch (argv[i][j])
                  {
                  case '/':            /* Options are flagged by '/' */

                        j++;
                        switch (argv[i][j])
                        {
                        case 'c':
                        case 'C':

                              option_flg |= CASE_SENSE;
                              break;

                        case '1':

                              option_flg |= SINGLE_KEY_ENTRY;
                              break;

                        case 'q':
                        case 'Q':

                        /*
                         * The question is assumed to be the argument
                         * after the 'q'.
                         */

                              ques_ptr = strdup (argv[++i]);
                              break;

                        default:

                        /*
                         * Oops! Something wrong. Tell them how its
                         * supposed to work.
                         */

                              usage (argv[0]);
                              return (-1);
                        }
                        break;

                  case '-':         /* Answers are flagged by '-' */

                        /*
                         * The answer is assumed to be everything from the
                         * '-' to the '=' characters. This allows for
                         * single letter and word answers.
                         */

                        j++;
                        if ((wk_ptr = strtok (argv[i]+j, "=")) != NULL)
                        {
                              /*
                               * Allocate space for the new answer entry and
                               * link in into the list. Answers are linked
                               * into the list in reverse order from their
                               * command line specification since the logic
                               * is considerably simpler and it shouldn't
                               * matter much anyway.
                               */

                              if (answer_tab == NULL)
                              {
                                    answer_tab = wk_ans = (VALID_ANS *)
                                          calloc (1, sizeof(VALID_ANS));
                              }
                              else
                              {
                                    wk_ans = (VALID_ANS *)
                                             calloc (1, sizeof(VALID_ANS));
                                    wk_ans->next_ans = answer_tab;
                                    answer_tab = wk_ans;
                              }

                              /*
                               * Store the answer string and errorlevel value
                               * into the answer element.
                               */

                              wk_ans->ans_key = strdup (wk_ptr);
                              wk_ptr          = strtok (NULL, "=");
                              if (wk_ptr != NULL)
                              {
                                    /*
                                     * Allow only positive returns since -1 is
                                     * and error return from the program.
                                     */
                                    
                                    wk_ans->rtn_err_lev =
                                          abs (atoi (wk_ptr));
                              }
                              else  wk_ans->rtn_err_lev = 0;
                        }
                        }
                        break;

                  default:

                  /*
                   * Oops! Something wrong. Tell them how its
                   * supposed to work.
                   */

                        usage (argv[0]);
                        return (-1);
            }
      }
      else
      {

            /*
             * Oops! Something wrong. Tell them how its
             * supposed to work.
             */

            usage (argv[0]);
            return (-1);
      }

      /*
       * Ask the question and get their answer.
       */

      printf ("%s", ques_ptr);

      if ((option_flg & SINGLE_KEY_ENTRY) != 0)
      {
            wk_str[0] = getche ();
            wk_str[1] = '\0';
      }
      else  fgets (wk_str, sizeof (wk_str), stdin);

      /*
       * See if we can find their answer in our valid answer list.
       */

      wk_ans = answer_tab;
      while (wk_ans != (VALID_ANS *) NULL)
      {
            if ((option_flg & CASE_SENSE) != 0)
            {
                  if ((strncmp (wk_str, wk_ans->ans_key,
                        (strlen (wk_ans->ans_key)))) == 0)
                  {
                  /*
                   * If we found the answer return the associated
                   * errorlevel return number.
                   */

                        return (wk_ans->rtn_err_lev);
                  }
            }
            else
            {
                  if ((strnicmp (wk_str, wk_ans->ans_key,
                        (strlen (wk_ans->ans_key)))) == 0)
                  {
                  /*
                   * If we found the answer return the associated
                   * errorlevel return number.
                   */

                        return (wk_ans->rtn_err_lev);
                  }
            }

            wk_ans = wk_ans->next_ans;
      }

      /*
       * If the answer wasn't in our table return a -1.
       */

      return (-1);
}

void usage (char * progname)
{
      printf ("%s usage:\n", progname);
      printf ("%s /[options] -<valid answer>=<errorlevel>\n\n", progname);
      printf ("Where:\n");
      printf ("  options      = c    (Answers are case sensitive)\n");
      printf ("                 1    (Single key entry)\n");
      printf ("                 q\"Question Text\"\n\n");
      printf ("  valid answer = (Single letter or word response)\n");
      printf ("  errorlevel   = (errorlevel to return when answer seen)\n\n");
      printf ("Example:\n%s /q\"Answer Y or N: \" /1 -y=1 -n=2\n\n",
            progname);
      printf ("Note: -1 is returned if answer is not valid or syntax\n");
      printf ("      is incorrect.\n");
}
