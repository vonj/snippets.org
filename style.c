/***************************************************************************/
/*  Global (public) headers look like this                                 */
/*                                                                         */
/*  foo()                                                                  */
/*  Parameters: description of each passed datum                           */
/*  Returns: description of return value & significance                    */
/*  Side effects: complete details                                         */
/*  Notes: extra info                                                      */
/***************************************************************************/

/*
** Static (local) headers look like this
**
** Brief description
*/

int foo(void)   /* use ANSI prototypes on every compiler that supports 'em */
{
      /* First of all, block comments (like this one) are preferable to    */
      /* trailing (in-line) comments as in the following examples.         */

      int local1, local2;               /* local variable declarations are
                                           always followed by a blank line */

      do_stuff();
      if (bar(local1))
      {     /* long comments here     *//* this lines up with -----------+ */
            char *local3;               /* autos declared close to use   | */
                                        /*                               | */
            do_more_stuff();            /* (everything else indented)    | */
            local2 = strlen(local3);    /*                               | */
      }                                 /* this <------------------------+ */
      else  local2 = fubar();           /* using tab >= 6, else's line up  */
      return local2;
}                                       /* no question where functions end!*/
